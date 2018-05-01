<?php

/**
 * @param $error_code
 */
function _moha_oauth2_errors($error_code) {
  $error_mapping = array(
    '10003' => 'redirect_uri域名与后台配置不一致',
    '10004' => '此公众号被封禁',
    '10005' => '此公众号并没有这些scope的权限',
    '10006' => '必须关注此测试号',
    '10009' => '操作太频繁了，请稍后重试',
    '10010' => 'scope不能为空',
    '10011' => 'redirect_uri不能为空',
    '10012' => 'appid不能为空',
    '10013' => 'state不能为空',
    '10015' => '公众号未授权第三方平台，请检查授权状态',
    '10016' => '不支持微信开放平台的Appid，请使用公众号App',
  );
}

/**
 * @param $state string
 *
 * @return string
 *
 * @throws \Exception
 */
function _moha_wx_get_state_url($state) {
  $mapping = array (
    MOHA_WX__OAUTH2_STATE__HOME => 'moha/wx/home',
  );

  if (!array_key_exists($state, $mapping)){
    throw new Exception('State doesn\'t exist: ' . check_plain($state), MohaWeChat::$ERROR__OAUTH_INVALID_STATE);
  }
  else {
    return $mapping[$state];
  }
}

/**
 * Request user information through WeChat OAuth2, then login on redirect page.
 * If user has been login status already, forward user to state's related URL directly.
 *
 * @param $state
 */
function _moha_wx_oauth2_request_authorization($wechat_name, $state) {

  try {
    global $base_url;

    $account = _moha_wx_moha_account($wechat_name);

    // Check if state is valid.
    $url = _moha_wx_get_state_url($state);

    //
    if (user_is_logged_in()) {
      drupal_goto($url);
    }
    else {
      drupal_goto(
        format_string(MOHA_WX__API__OAUTH2_REQUEST,
          [
            '@APPID' => $account['app_id'],
            '@REDIRECT_URI' => $base_url . base_path() . format_string(MOHA_WX__URL__OAUTH2_REDIRECT_USER_INFO, ['@WECHAT_NAME' => $wechat_name]),
            '@STATE' => $state
          ])
      );
    }
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }
}

/**
 * Assistant user login through WeChat OAuth2 redirect.
 *
 *
 * @param $wechat_name string
 *        WeChat account machine name.
 */
function _moha_wx_oauth2_redirect_user_info($wechat_name){

  try {
    $account = _moha_wx_moha_account($wechat_name);

    $state = isset($_GET['state'])?$_GET['state']:'';

    if (empty($state)){
      throw new Exception('No OAuth State.', MohaWeChat::$ERROR__OAUTH_NO_STATE);
    }

    $code = isset($_GET['code'])?$_GET['code']:'';

    if (empty($code)){
      throw new Exception('No OAuth Code.', MohaWeChat::$ERROR__OAUTH_NO_CODE);
    }

    /* JSON array structure sample:
      { "access_token":"ACCESS_TOKEN",
        "expires_in":7200,
        "refresh_token":"REFRESH_TOKEN",
        "openid":"OPENID",
        "scope":"SCOPE"
      } */
    $oauth_access_tokon_json_array = moha_wx_get_official(
      format_string(MOHA_WX__API__OAUTH_ACCESS_TOKEN, array(
        '@APPID' => $account['app_id'],
        '@APPSECRET' => $account['app_secret'],
        '@CODE' => $code,
      ))
    );

    // Store access_token, expires_in, refresh_token, openid, scope, unionid.
    $oauth2_user = _moha_wx_moha_user_by_openid($oauth_access_tokon_json_array['openid'], $account['app_id']);

    global $user;

    if (!user_is_logged_in() || (user_is_logged_in() && $user->uid != $oauth2_user->uid)) {
      $form_state = array();
      $form_state['uid'] = $oauth2_user->uid;

      user_login_submit(array(), $form_state);
      drupal_session_regenerate();
    }

    drupal_goto(_moha_wx_get_state_url($state));

  }
  catch (MohaWXNoUserException $e) {
    try {
      // Create new user.
      $password = user_password(16);
      $user_fields = array(
        'name' => drupal_random_key(32),
        'pass' => $password,
        'status' => 1,
        'init' => __FUNCTION__,
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        ),
      );

      $user = user_save('', $user_fields);

      user_save($user, array(
        'name' => $user->uid . '@moha.online',
        'mail' => $user->uid . '@moha.online',
      ));

      $profile = profile2_create(array(
        'type' => __MOHA,
        'uid' => $user->uid
      ));

      $moha_wx_user_info = entity_create('field_collection_item', array('field_name' => 'moha_wx_user_info'));
      $moha_wx_user_info->setHostEntity('profile2', $profile);

      $connected_wechat = entity_create('field_collection_item', array('field_name' => 'moha_wx_connected_accounts'));
      $connected_wechat->setHostEntity('profile2', $profile);
      $wrapper = entity_metadata_wrapper('field_collection_item', $connected_wechat);
      $wrapper->moha_wx_appid->set($account['app_id']);
      $wrapper->moha_wx_openid->set($oauth_access_tokon_json_array['openid']);
      $wrapper->moha_wx_oauth2_access_token->set($oauth_access_tokon_json_array['access_token']);
      $wrapper->moha_wx_oauth2_token_updated->set(time());
      $wrapper->moha_wx_oauth2_token_valid_time->set(time()+$oauth_access_tokon_json_array['expires_in']);
      $wrapper->moha_wx_oauth2_refresh_token->set($oauth_access_tokon_json_array['refresh_token']);
      $wrapper->save(TRUE);

      $oauth_user_info_json_array = moha_wx_get_official(
        format_string(
          MOHA_WX__API__OAUTH2_REQUEST_USER_INFO,
          array(
            '@ACCESS_TOKEN' => $oauth_access_tokon_json_array['access_token'],
            '@OPENID' => $oauth_access_tokon_json_array['openid'],
          )
        )
      );

      // Get user basic information from WeChat.
      $wrapper = entity_metadata_wrapper('field_collection_item', $moha_wx_user_info);

      if (!empty($oauth_access_tokon_json_array['unionid'])) {
        $wrapper->moha_wx_unionid[] = $oauth_access_tokon_json_array['unionid'];
      }
      $wrapper->moha_wx_nickname->set($oauth_user_info_json_array['nickname']);
      $wrapper->moha_wx_sex->set($oauth_user_info_json_array['sex']);
      $wrapper->moha_wx_province->set($oauth_user_info_json_array['province']);
      $wrapper->moha_wx_city->set($oauth_user_info_json_array['city']);
      $wrapper->moha_wx_country->set($oauth_user_info_json_array['country']);
      $wrapper->moha_wx_portrait->set($oauth_user_info_json_array['headimgurl']);
      $wrapper->save(TRUE);
      profile2_save($profile);

      // Let user login.
      $form_state['uid'] = $user->uid;
      user_login_submit(array(), $form_state);
      drupal_session_regenerate();
      drupal_goto(_moha_wx_get_state_url($state));

    }
    catch (Exception $e) {
      watchdog(__FUNCTION__, $e);
    }
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }

}