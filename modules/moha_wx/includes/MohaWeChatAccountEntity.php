<?php
/**
 * @file
 * The entity definition file.
 */

/**
 * Class MohaWeChatAccountEntity
 * 
 */
class MohaWeChatAccountEntity extends Entity {

  /**
   * @var $id integer
   *   Entity ID.
   */
  public $id;

  /**
   * WeChat account machine name.
   */
  public $name;
  public $app_id;
  public $app_secret;
  
  /**
   * Verifying factor of WeChat message signature.
   */
  public $token;
  
  public $type;
  public $access_token_updated;
  public $access_token_expires_in;
  public $access_token;
  public $wx_menu;
  public $wx_fallback;

  /**
   * Returns TRUE if current entity is an Enterprise account.
   * 
   * @return bool
   */
  function isEnterpriseAccount(){
    return $this->type == moha_array_key_by_value(MOHA_WX__ACCOUNT_TYPE__ENTERPRISE, MOHA_WX__ACCOUNT_TYPE);
  }
  
  /**
   * If access token no existed or expired, fetch access token from wechat server,
   * otherwise return stored access token.
   *
   * @param $force boolean
   *   True force to refresh Access Token.
   *
   * @throws Exception: no access token.
   */
  function refreshAccessToken($force = FALSE) {
    // expired time of access_token is in future.
    if (!empty($this->access_token) && ($this->access_token_updated + $this->access_token_expires_in) > REQUEST_TIME && !$force){
      return;
    }

    // Adjust URL of access token API per account type.
    $access_token_api = MOHA_WX__API__ACCESS_TOKEN;
    if ($this->isEnterpriseAccount()) {
      $access_token_api = MOHA_WX__API__ACCESS_TOKEN__ENT;
    }

    $json = moha_wx_get_official(format_string($access_token_api, array(
      '@APPID' => $this->app_id,
      '@APPSECRET' => $this->app_secret,
    )));

    // Save access_token into DB.
    $this->access_token = $json['access_token'];
    $this->access_token_updated = REQUEST_TIME;
    $this->access_token_expires_in = $json['expires_in'];

    $this->save();
  }


  /**
   * Fetch JSON configuration of WeChat menu.
   * If there is no menu customized at WeChat end,
   * then make sure relative menu configuration in DB is empty string.
   *
   * @param $force boolean
   *  True to from get menu from WeChat account.
   *
   * @return string
   *   WeChat menu JSON configuration.
   *
   * @throws Exception
   */
  function getMenu($force = FALSE) {
    $this->refreshAccessToken();
    
    if (!empty($this->wx_menu) && !$force) {
      return $this->wx_menu;
    }
    
    try {
      if ($this->isEnterpriseAccount()) {
        $response_array = moha_wx_get_official(
          format_string(MOHA_WX__API__GET_MENU_ENT, array(
            '@ACCESS_TOKEN' => $this->access_token,
            '@AGENT_ID' => $this['agent_id'],
          ))
        );
      }
      else {
        $response_array = moha_wx_get_official(
          format_string(MOHA_WX__API__GET_MENU, array('@ACCESS_TOKEN' => $this->access_token))
        );
      }
    }
    catch (Exception $e) {
      if ($e->getCode() == 46003) {
        // If menu does not exist.
        watchdog_exception(__FUNCTION__, $e, NULL, array(), WATCHDOG_WARNING);
        moha_show_exception($e, 'warning');
      }
      else {
        throw $e;
      }
    }

    if (empty($response_array) || !empty($response_array['errcode'])){
      $wx_menu = '';
    }
    elseif (isset($response_array['menu'])) {
      $wx_menu = json_encode($response_array['menu'], MOHA__JSON_ENCODING_OPTIONS_BIT_MASK);
    }
    else {
      // erase "errcode" => 0 and "errmsg" => "ok" from response of enterprise account menu.
      if ($this->isEnterpriseAccount()) {
        unset($response_array["errcode"]);
        unset($response_array["errmsg"]);
      }

      $wx_menu = json_encode($response_array, MOHA__JSON_ENCODING_OPTIONS_BIT_MASK);
    }

    if($this->wx_menu != $wx_menu) {
      $this->wx_menu = $wx_menu;

      $this->save();
    }

    if ($this) {
      drupal_set_message(t("JSON configuration of WeChat menu is fetched."));
    }
    else {
      // This function is invoked only when wx_menu column is empty.
      drupal_set_message(t("No WeChat menu, or customized WeChat menu hasn't been reflected on WeChat account."));
    }

    return $this->wx_menu;
  }


  /**
   * Delete WeChat menu.
   *
   * @throws Exception
   *   Error when communicate with Tecent server.
   */
  function deleteMenu(){
    $this->refreshAccessToken();

    if ($this->isEnterpriseAccount()) {
      $result = moha_wx_get_official(
        format_string(MOHA_WX__API__DELETE_MENU_ENT, array(
          '@ACCESS_TOKEN' => $this->access_token,
          '@AGENT_ID' => $this->agent_id,
        ))
      );
    }
    else {
      $result = moha_wx_get_official(
        format_string(MOHA_WX__API__DELETE_MENU, array(
          '@ACCESS_TOKEN' => $this->access_token
        ))
      );
    }

    if ($result['errcode'] == 0 ) {
      drupal_set_message('WeChat Menu has been deleted.');
    }
  }

  /**
   * Deploy WeChat Menu through creation API.
   *
   * @throws Exception
   *
   * @see https://mp.weixin.qq.com/wiki?t=resource/res_main&id=mp1421141013
   */
  function deployMenu(){
    $this->refreshAccessToken();

    if ($this->isEnterpriseAccount()) {
      $response_array = moha_wx_post_official(
        format_string(MOHA_WX__API__CREATE_MENU_ENT, array(
          '@ACCESS_TOKEN' => $this->access_token,
          '@AGENT_ID' => $this->agent_id,
        )),
        $this->wx_menu
      );
    }
    else {
      $response_array = moha_wx_post_official(
        format_string(MOHA_WX__API__CREATE_MENU, array('@ACCESS_TOKEN' => $this->access_token)),
        $this->wx_menu );
    }

    if ($response_array['errcode'] == 0) {
      drupal_set_message('WeChat Menu has been deployed.');
    }
  }
  
  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => MOHA_WX__URL__ACCOUNT_ADMIN_UI . '/manage/' . $this->identifier());
  }

  /**
   * @return string
   */
  protected function defaultLabel() {
    if (isset($this->is_new) && $this->is_new) {
      return '';
    }
    else {
      return parent::defaultLabel();
    }
  }
}
