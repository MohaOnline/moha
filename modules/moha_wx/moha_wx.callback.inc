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

  $accounts['tanan'] = array(
    'AppID' => 'wx3404eff047f7b59f',
    'AppSecret' => '5f44ae3f11d27ba8df627c28bfb3bc20',
    'Token' => 'ware13(spend',
    'EncodingAESKey' => 'RyIQoohwGjH3dJZxqj5H887U88pArANPUBO0yT8Z0FT',
  );

  $accounts['das'] = array(
    'AppID' => 'wx9cf625046378ca9c',
    'AppSecret' => '3c21ee4891ace6a878e6525fd292703b',
    'Token' => 'pleonastically300824_demanders',
    'EncodingAESKey' => '4G146PjmOBpQs8JCThZa6NevghiVEO4xDAWfmi18o8j',
  );

  try {
    if ($account_name != 'das' && $account_name != 'tanan') {
      throw new Exception("Invalid WeChat account name.");
    }

    // Get $_GET contents to decide next logic
    $query = drupal_get_query_parameters();

    if(!moha_wx_check_signature($accounts[$account_name]['Token'], $query)){
      throw new Exception("Wrong signature.");
    }

    // Open development mode.
    if (isset($query['echostr'])) {

      watchdog( MOHA_WX,
        __FUNCTION__ . " echostr: %echostr",
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
      watchdog( MOHA_WX,
        __FUNCTION__ . " Post Data = %postData",
        array('%postData' => $postData)
      );
    }

    // Analysis WeChat message.
    $msg = (array)simplexml_load_string($postData, 'SimpleXMLElement', LIBXML_NOCDATA);

    // Return welcome message.
    if ($msg['MsgType'] == 'event' && $msg['Event'] == 'subscribe'){
      $welcome['das'] = array(
        'picurl' => 'https://mmbiz.qlogo.cn/mmbiz_jpg/s6bWmmoF06jbPjmribJlLzRxsicSv8bxlLTtmt2KKW6Gc0P4ZbgINSqEIYNe6gTW7zAzZLffwO3xX1AJAk3wImRg/640?wx_fmt=jpeg',
        'title' => '欢迎关注DAS戴思环保',
        'description' => '戴思环保设备贸易 (上海) 有限公司专为特定客户生产废气处理设施。',
        'url' => 'https://mp.weixin.qq.com/s/WMl8UkAd_CpL-19kXdQURw',
      );

      $welcome_response_template = <<<EOF
<xml><ToUserName><![CDATA[@toUser]]></ToUserName><FromUserName><![CDATA[@fromUser]]></FromUserName><CreateTime>@createTime</CreateTime><MsgType><![CDATA[news]]></MsgType><ArticleCount>1</ArticleCount><Articles><item><Title><![CDATA[@title]]></Title><Description><![CDATA[@description]]></Description><PicUrl><![CDATA[@picurl]]></PicUrl><Url><![CDATA[@url]]></Url></item></Articles></xml>
EOF;

      $response = format_string($welcome_response_template, array(
        '@toUser' => $msg['FromUserName'],
        '@fromUser' => $msg['ToUserName'],
        '@createTime' => time(),
        '@title' => $welcome[$account_name]['title'],
        '@description' => $welcome[$account_name]['description'],
        '@picurl' => $welcome[$account_name]['picurl'],
        '@url' => $welcome[$account_name]['url'],
      ));

      echo $response;

      watchdog( MOHA_WX,
        __FUNCTION__ . " res = %response",
        array('%response' => $response)
      );
    }

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
