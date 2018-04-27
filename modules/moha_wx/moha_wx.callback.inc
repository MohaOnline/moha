<?php
/**
 * @file
 * @Created: 20/03/2018 1:54 AM
 */

ctools_include('WXBizMsgCrypt', __MOHA_WX);

/**
 * @param $account_name WeChat account machine name
 */
function _moha_wx_callback ($account_name) {

  try {

    $account = db_query("SELECT * FROM {moha_wx_accounts} moha_wx_accounts WHERE wechat_name = :wechat_name",
      array(
        ':wechat_name' => $account_name,
      )
    )->fetchAssoc();

    if (!$account) {
      throw new Exception("Invalid WeChat account name.");
    }

    // Get $_GET contents to decide next logic
    $query = drupal_get_query_parameters();

    // Only verify token when verifying customized server.
    if(!moha_wx_check_signature($account['token'], $query)){
      throw new Exception("Wrong signature.");
    }

    // Open development mode.
    if (isset($query['echostr'])) {

      watchdog( __FUNCTION__,
        "echostr: %echostr",
        array('%echostr' => $query['echostr'])
      );

      echo $query['echostr'];

      return MENU_SITE_ONLINE;
    }

    // Log post data first.
    $postData = file_get_contents("php://input");

    if (empty($postData)){
      throw new Exception("No post data.");
    }
    else {
      watchdog( __FUNCTION__,
        "receive = %postData",
        array('%postData' => $postData)
      );
    }

    // Analysis WeChat message.
    $msg = (array)simplexml_load_string($postData, 'SimpleXMLElement', LIBXML_NOCDATA);

    // Return welcome message.
    if ($msg['MsgType'] == 'event' && $msg['Event'] == 'subscribe'){

      $welcome_response_template = <<<EOF
<xml><ToUserName><![CDATA[@toUser]]></ToUserName><FromUserName><![CDATA[@fromUser]]></FromUserName><CreateTime>@createTime</CreateTime><MsgType><![CDATA[news]]></MsgType><ArticleCount>1</ArticleCount><Articles><item><Title><![CDATA[@title]]></Title><Description><![CDATA[@description]]></Description><PicUrl><![CDATA[@picurl]]></PicUrl><Url><![CDATA[@url]]></Url></item></Articles></xml>
EOF;

      $response = format_string($welcome_response_template, array(
        '@toUser' => $msg['FromUserName'],
        '@fromUser' => $msg['ToUserName'],
        '@createTime' => time(),
        '@title' => $account['news_title'],
        '@description' => $account['news_digest'],
        '@picurl' => $account['news_cover'],
        '@url' => $account['news_link'],
      ));

      echo $response;

      watchdog( __FUNCTION__,
        "return = %response",
        array('%response' => $response)
      );
    }
    // Return contact message.
    else if ($msg['MsgType'] == 'event' && $msg['EventKey'] == 'MENU_CONTACT') {
      $text_template = <<<EOF
<xml><ToUserName><![CDATA[@toUser]]></ToUserName><FromUserName><![CDATA[@fromUser]]></FromUserName><CreateTime>@createTime</CreateTime><MsgType><![CDATA[text]]></MsgType><Content><![CDATA[@content]]></Content></xml>
EOF;
      $response = format_string($text_template, array(
        '@toUser' => $msg['FromUserName'],
        '@fromUser' => $msg['ToUserName'],
        '@createTime' => time(),
        '@content' => $account['text'],
      ));

      echo $response;

      watchdog( __FUNCTION__,
        "return = %response",
        array('%response' => $response)
      );
    }
    ///

  }
  catch (Exception $e) {
    watchdog( MOHA_WX,
      __FUNCTION__ . " catch: %msg.",
      array('%msg' => $e->getMessage()),
      WATCHDOG_ERROR
    );

    header(t(MOHA_WX__MSG__NO_SERVICE), TRUE, 503);
  }

  return MENU_SITE_ONLINE;
}
