<?php
/**
 * @file
 * Entity create / edit form file.
 */

/**
 * Implements hook_form().
 *
 * Entity edit form.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 *
 * @param $entity MohaUserOAuth2ClientEntity
 *
 * @see _moha_wx_account_form()
 * @return mixed form array.
 */
function moha_wx_account_form($form, &$form_state, $entity = NULL) {

  $form = &moha_entity_form_default($form, $form_state, $entity);
  $container = &$form['container'];

  $container['type'] = moha_form_element_select(
    'Account type',
    'If account is an enterprise WeChat account.',
    empty($entity->type) ? 0 : $entity->type,
    MOHA_WX__ACCOUNT_TYPE
  );

  $container['agent_id'] = moha_form_element_text(
    'Agent ID',
    'Agent ID of the WeChat enterprise account.',
    empty($entity->agent_id) ? '' : $entity->agent_id,
    64, 64
  );

  $container['app_id'] = moha_form_element_text(
    'App ID',
    'Application ID, WeChat account auto generated.',
    empty($entity->app_id) ? '' : $entity->app_id,
    64, 64, TRUE
  );

  $container['app_secret'] = moha_form_element_text(
    'App Secret',
    'Application password, WeChat account auto generated.',
    empty($entity->app_secret) ? '' : $entity->app_secret,
    64, 64, TRUE
  );

  $container['token'] = moha_form_element_text(
    'Token',
    'User generated random string, used as verification factor of communication signature from WeChat service. Same need be configured in WeChat account.',
    empty($entity->token) ? '' : $entity->token,
    40, 32
  );

  $container['scope'] = moha_form_element_text(
    'OAuth2 scope',
    'snsapi_login for Open Platform; snsapi_userinfo and snsapi_base for WeChat subscription and service account.',
    empty($entity->scope) ? '' : $entity->scope,
    40, 32, TRUE
  );

  $container['encoding_aes_key'] = moha_form_element_text(
    'EncodingAESKey',
    '',
    empty($entity->encoding_aes_key) ? '' : $entity->encoding_aes_key
  );

  $container['news_title'] = moha_form_element_text(
    'Title',
    'Specify the title of the news which will be used as welcome message.',
    empty($entity->news_title) ? '' : $entity->news_title
  );

  $container['news_digest'] = moha_form_element_text(
    'Abstract',
    'Specify the brief of the news which will be used as welcome message.',
    empty($entity->news_digest) ? '' : $entity->news_digest
  );

  $container['news_cover'] = moha_form_element_text(
    'Cover URL',
    'URL of the news cover image.',
    empty($entity->news_cover) ? '' : $entity->news_cover,
    120, 255
  );

  $container['news_link'] = moha_form_element_text(
    'Article URL',
    'Welcome message: URL of article itself, leave blank to fetch news URL from WeChat backend',
    empty($entity->news_link) ? '' : $entity->news_link,
    120, 255
  );

  $container['text'] = moha_form_element_textarea(
    'Contact us text',
    'Put text contact us message here.',
    empty($entity->text) ? '' : $entity->text
  );

  $accounts = moha_wx_fetch_account();
  $options = array();
  foreach ($accounts as $account) {
    $options[$account->id] = check_plain($account->human_name) . '(' . $account->name . ')';
  }

  $container['wx_fallback'] = moha_form_element_select(
    'WeChat fallback',
    'Select an account to fallback when user OAuth2 Open Platform account through WeChat browser, Or OAuth2 WeChat account through non-WeChat browser.',
    !isset($entity->wx_fallback) ? '' : $entity->wx_fallback,
    $options
  );

  moha_form_element_inline_2_table($container);
  moha_form_actions($form, MOHA__ACTION__REFRESH);
  return $form;
}

/**
 * Validates form input.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_wx_account_form_validate($form, &$form_state) {

}

/**
 * Save WeChat account information.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_wx_account_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  // Build entity from submission.
  /** @var \MohaWeChatAccountEntity $entity */
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  if ($values['op'] == t(MOHA__ACTION__REFRESH)) {
    $entity->refreshAccessToken(TRUE);
    $entity->save();
    drupal_set_message(t('Access Token was triggered to be refreshed, and the account has been saved.'));
    return;
  }

  /** @see \MohaCommonEntityController::save()*/
  # $entity->is_new_revision = FALSE;
  $entity->save();
  $form_state['redirect'] = MOHA_WX__URL__ACCOUNT_ADMIN_UI;
}
