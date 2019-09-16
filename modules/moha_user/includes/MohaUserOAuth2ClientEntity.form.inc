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
 * @return mixed form array.
 */
function moha_user_oauth2_client_form($form, &$form_state, $entity = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => empty($entity->id) ? '' : $entity->id,
  );

  $form['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Human name'),
    '#default_value' => empty($entity->human_name) ? '' : $entity->human_name,
    '#description' => t('English full name of the OAuth2 client.'),
    '#maxlength' => 255,
    '#size' => 100,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => empty($entity->name) ? '' : $entity->name,
    '#machine_name' => array(
      'source' => array('human_name'), // Bind machine_name field to relative normal field.
      'label' => t('name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
      /** @see moha_entity_exists() */
      'exists' => 'moha_entity_exists',
    ),
  );

  $container = &moha_form_set_inline_container($form['container']);

  $container['tenant'] = array(
    '#type' => 'textfield',
    '#title' => t('Tenant'),
    '#default_value' => empty($entity->tenant) ? '' : $entity->tenant,
    '#description' => t('Tenant ID, domain name or alias name.'),
    '#maxlength' => 255,
    '#size' => 100,
  );

  $container['authorize'] = array(
    '#type' => 'textfield',
    '#title' => t('Authorize'),
    '#default_value' => empty($entity->authorize) ? '' : $entity->authorize,
    '#description' => t('URL: authorization endpoint.'),
    '#maxlength' => 255,
    '#size' => 100,
  );

  $container['token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => empty($entity->token) ? '' : $entity->token,
    '#description' => t('URL: access token endpoint.'),
    '#maxlength' => 255,
    '#size' => 100,
  );

  $container['openid'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenID'),
    '#default_value' => empty($entity->openid) ? '' : $entity->openid,
    '#description' => t('URL: OpenID endpoint.'),
    '#maxlength' => 255,
    '#size' => 100,
  );

  $container['client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => empty($entity->client_id) ? '' : $entity->client_id,
    '#description' => t('Client ID.'),
    '#maxlength' => 60,
    '#size' => 40,
  );

  $container['client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => empty($entity->client_secret) ? '' : $entity->client_secret,
    '#description' => t('Client Secret.'),
    '#maxlength' => 60,
    '#size' => 40,
  );

  $container['response_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Response Type'),
    '#default_value' => empty($entity->response_type) ? '' : $entity->response_type,
    '#description' => t('Specifies what can be returned to client.'),
    '#maxlength' => 60,
    '#size' => 40,
  );

  $container['response_mode'] = array(
    '#type' => 'textfield',
    '#title' => t('Response Mode'),
    '#default_value' => empty($entity->response_mode) ? '' : $entity->response_mode,
    '#description' => t('Specifies the method that should be used to send the resulting token back to client.'),
    '#maxlength' => 60,
    '#size' => 40,
  );

  $container['resource'] = array(
    '#type' => 'textfield',
    '#title' => t('Resource'),
    '#default_value' => empty($entity->resource) ? '' : $entity->resource,
    '#description' => t('The external resource of the target web API (secured resource).'),
    '#maxlength' => 255,
    '#size' => 100,
  );

  $container['scope'] = array(
    '#type' => 'textfield',
    '#title' => t('Scope'),
    '#default_value' => empty($entity->scope) ? '' : $entity->scope,
    '#description' => t('A space-separated list of scopes that you want the user to consent to.'),
    '#maxlength' => 60,
    '#size' => 40,
  );

  $container['domain_hint'] = moha_form_element_text(
    t('Domain Hint'),
    t('Hint AAD redirects to the specified tenant federation server.'),
    empty($entity->domain_hint) ? '' : $entity->domain_hint,
    40, 60
  );

  $container['integration'] = moha_form_element_radio('Integration', 'Boolean if let user auto login.');
  
  $container['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => t('Status of the entity.'),
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($entity->status)?$entity->status:1,
  );
  
  moha_form_actions($form, isset($entity->is_new) ? 'Add' : 'Save');
  
  if (!empty($entity->name)) {
    global $base_url;
    $code_login_uri = $base_url . url("moha/user/oauth2/login/$entity->name");
    $container['code_login_url'] = array(
      '#markup' => "<div class='form-item'><label>Login URL</label>$code_login_uri</div>",
    );

    $code_post_uri = $base_url . url("moha/user/oauth2/post/$entity->name");
    $container['code_post_url'] = array(
      '#markup' => "<div class='form-item'><label>Redirect URL</label>$code_post_uri</div>",
    );
  }
  return $form;
}

/**
 * Validate form input.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_client_form_validate($node, &$form_state) {

}

/**
 * Save OAuth2 client information.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_client_form_submit($form, &$form_state) {

  // Build entity from submission.
  /** @var MohaUserOAuth2ClientEntity $entity */
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  /** @see \MohaCommonEntityController::save()*/
  $entity->save();

  $form_state['redirect'] = MOHA_USER__URL__OAUTH2_CLIENT_ADMIN_UI;
}
