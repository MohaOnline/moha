<?php
/**
 * @file
 * The functions as WeChat callback server for subscription, service account,
 * called by Tencent servers, these interface will not integrated with WeChat app directly.
 */
ctools_include('WXBizMsgCrypt', __MOHA_WX);

/**
 * Callback entry. All outputs print through echo() directly, only menu status
 * code is returned. Any exception occurs, http code 503 will be written to
 * client browser.
 *
 * @param $wechat_name string
 *    WeChat account machine name
 *
 * @link moha/wx/server/% @endlink
 *
 * @return int
 */
function _moha_wx_callback ($wechat_name) {

  try {
    /** @var $account MohaWeChatAccountEntity */
    $account = moha_wx_fetch_account_by_name($wechat_name);

    // Get $_GET contents to decide next logic
    $query = drupal_get_query_parameters();

    // Only verify token when verifying customized server.
    moha_wx_check_signature($account->token, $query);

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

    // Returns welcome message.
    if ($msg['MsgType'] == 'event' && $msg['Event'] == 'subscribe'){

      $welcome_response_template = <<<EOF
<xml><ToUserName><![CDATA[@toUser]]></ToUserName><FromUserName><![CDATA[@fromUser]]></FromUserName><CreateTime>@createTime</CreateTime><MsgType><![CDATA[news]]></MsgType><ArticleCount>1</ArticleCount><Articles><item><Title><![CDATA[@title]]></Title><Description><![CDATA[@description]]></Description><PicUrl><![CDATA[@picurl]]></PicUrl><Url><![CDATA[@url]]></Url></item></Articles></xml>
EOF;

      $response = format_string($welcome_response_template, array(
        '@toUser' => $msg['FromUserName'],
        '@fromUser' => $msg['ToUserName'],
        '@createTime' => REQUEST_TIME,
        '@title' => $account->news_title,
        '@description' => $account->news_digest,
        '@picurl' => $account->news_cover,
        '@url' => $account->news_link,
      ));

      echo $response;

      watchdog( __FUNCTION__,
        "return = %response",
        array('%response' => $response)
      );
    }
    // Returns contact message.
    else if ($msg['MsgType'] == 'event' && $msg['EventKey'] == 'MENU_CONTACT') {
      $text_template = <<<EOF
<xml><ToUserName><![CDATA[@toUser]]></ToUserName><FromUserName><![CDATA[@fromUser]]></FromUserName><CreateTime>@createTime</CreateTime><MsgType><![CDATA[text]]></MsgType><Content><![CDATA[@content]]></Content></xml>
EOF;
      $response = format_string($text_template, array(
        '@toUser' => $msg['FromUserName'],
        '@fromUser' => $msg['ToUserName'],
        '@createTime' => REQUEST_TIME,
        '@content' => $account->text,
      ));

      echo $response;

      watchdog( __FUNCTION__,
        "return = %response",
        array('%response' => $response)
      );
    }
    ////////

  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    header(MOHA_WX__MSG__NO_SERVICE, TRUE, 503);
  }

  return MENU_SITE_ONLINE;
}
