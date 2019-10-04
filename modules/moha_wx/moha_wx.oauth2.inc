<?php
/**
 * @file
 * The OAuth2 functions definition.
 */

/**
 * @param $error_code
 * @return string
 */
function _moha_wx_oauth2_errors($error_code) {
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

  return $error_mapping[$error_code];
}

/**
 * Redirect user to WeChat OAuth2 page for explicit authentication if user hasn't login.
 * If user has been login status, forward user to state associated URL directly.
 *
 * @example moha/wx/oauth2_explicit_login/@WECHAT_NAME/@STATE
 *
 * @param $state
 *
 * @throws \Exception if process can't continue.
 */
function _moha_wx_oauth2_request_explicit_authorization($wechat_name, $state) {

  try {
    $account = moha_wx_fetch_account_by_name($wechat_name);

    // Check if state is valid.
    $url = _moha_wx_get_state_url($state);

    if (user_is_logged_in()) {
      drupal_goto($url);
    }
    else {
      // When user hasn't logged in, redirect him/her to WeChat OAuth2 explicit authentication URL.
      drupal_goto(
        format_string(MOHA_WX__API__OAUTH2_WITH_USER_INFO,
          array(
            '@APPID' => $account['app_id'],
            '@REDIRECT_URI' => url(
              format_string(MOHA_WX__URL__OAUTH2_BACK_WITH_USER_INFO, array('@WECHAT_NAME' => $wechat_name)),
              array('absolute' => TRUE)
            ),
            '@STATE' => $state
          ))
      );
    }
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    moha_http_response_error($e);
  }
}

/**
 * Handles requests redirected from OAuth2 explicit authentication.
 * Assistant user login through code and state.
 *
 * WeChat account name is from URL, code and state are from URL parameters.
 *
 * @example moha/wx/oauth2_back_with_user_info/@WECHAT_NAME
 *
 * @see moha_wx_menu()
 * @see MOHA_WX__URL__OAUTH2_BACK_WITH_USER_INFO
 *
 * @param $wechat_name string
 *        WeChat account machine name.
 */
function _moha_wx_oauth2_back_with_user_info($wechat_name){

  $userid = $openid = '';
  $is_internal_user = TRUE;
  $wechat_name = check_plain($wechat_name);

  try {
    $state = isset($_GET['state'])?$_GET['state']:'';

    if (empty($state)){
      throw new Exception('No State.', MohaWeChat::$ERROR__OAUTH_NO_STATE);
    }

    $code = isset($_GET['code'])?$_GET['code']:'';

    if (empty($code)){
      throw new Exception('No Code.', MohaWeChat::$ERROR__OAUTH_NO_CODE);
    }

    $account = moha_wx_fetch_account_by_name($wechat_name);

    if ($account['is_enterprise']) {
      _moha_wx_refresh_access_token($account);

      $result_array = moha_wx_get_official(
        format_string(MOHA_WX__API__OAUTH2_GET_USER_ID_FROM_CODE_ENT, array(
          '@ACCESS_TOKEN' => $account['access_token'],
          '@CODE' => $code,
        ))
      );

      if (isset($result_array['UserId'])) {
        $userid = $result_array['UserId'];
        $oauth2_user = _moha_wx_moha_user_by_userid($userid, $account['app_id']);
      }
      elseif (isset($result_array['OpenId'])) {
        $openid = $result_array['OpenId'];
        $is_internal_user = FALSE;
        $oauth2_user = _moha_wx_moha_user_by_openid($openid, $account['app_id']);
      }

      //* Enterprise WeChat account logic. */
    }
    else {
      //** Service, Subscription WeChat account logic. */
      /* JSON array structure sample:
        { "access_token":"ACCESS_TOKEN",
          "expires_in":7200,
          "refresh_token":"REFRESH_TOKEN",
          "openid":"OPENID",
          "scope":"SCOPE"
        }
      */
      $oauth_access_tokon_json_array = moha_wx_get_official(
        format_string(MOHA_WX__API__OAUTH2_ACCESS_TOKEN, [
          '@APPID' => $account['app_id'],
          '@APPSECRET' => $account['app_secret'],
          '@CODE' => $code,
        ])
      );

      // Store access_token, expires_in, refresh_token, openid, scope, unionid.
      $oauth2_user = _moha_wx_moha_user_by_openid($oauth_access_tokon_json_array['openid'], $account['app_id']);
    }

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
        'name' => $user->uid . '@' . $wechat_name,
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

      // Adds connected WeChat accounts information.
      $wrapper = entity_metadata_wrapper('field_collection_item', $connected_wechat);
      $wrapper->moha_wx_appid->set($account['app_id']);

      if ($account['is_enterprise']) {
        if ($is_internal_user) {
          $wrapper->moha_wx_userid->set($userid);
        }
        else {
          $wrapper->moha_wx_openid->set($openid);
        }
      }
      else {
        $wrapper->moha_wx_openid->set($oauth_access_tokon_json_array['openid']);
        $wrapper->moha_wx_oauth2_access_token->set($oauth_access_tokon_json_array['access_token']);
        $wrapper->moha_wx_oauth2_token_updated->set(time());
        $wrapper->moha_wx_oauth2_token_valid_time->set(time()+$oauth_access_tokon_json_array['expires_in']);
        $wrapper->moha_wx_oauth2_refresh_token->set($oauth_access_tokon_json_array['refresh_token']);
      }

      $wrapper->save(TRUE);

      // Prepare to update user information of WeChat account.
      $wrapper = entity_metadata_wrapper('field_collection_item', $moha_wx_user_info);

      if ($account['is_enterprise']) {

      }
      else {
        //< Update user info of subscription or service account.
        $oauth_user_info_json_array = moha_wx_get_official(
          format_string(
            MOHA_WX__API__OAUTH2_REQUEST_USER_INFO,
            [
              '@ACCESS_TOKEN' => $oauth_access_tokon_json_array['access_token'],
              '@OPENID' => $oauth_access_tokon_json_array['openid'],
            ]
          )
        );

        if (!empty($oauth_access_tokon_json_array['unionid'])) {
          $wrapper->moha_wx_unionid[] = $oauth_access_tokon_json_array['unionid'];
        }
        $wrapper->moha_wx_nickname->set($oauth_user_info_json_array['nickname']);
        $wrapper->moha_wx_sex->set($oauth_user_info_json_array['sex']);
        $wrapper->moha_wx_province->set($oauth_user_info_json_array['province']);
        $wrapper->moha_wx_city->set($oauth_user_info_json_array['city']);
        $wrapper->moha_wx_country->set($oauth_user_info_json_array['country']);
        $wrapper->moha_wx_portrait->set($oauth_user_info_json_array['headimgurl']);

        //> Update user info of subscription or service account.
      }

      $wrapper->save(TRUE);
      profile2_save($profile);

      // Let user login.
      $form_state['uid'] = $user->uid;
      user_login_submit(array(), $form_state);
      drupal_session_regenerate();
      drupal_goto(_moha_wx_get_state_url($state));

    }
    catch (Exception $e) {
      watchdog_exception(__FUNCTION__, $e);
    }
    //> No existing user.
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }

}
