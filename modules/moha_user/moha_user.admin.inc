<?php
/**
 * @file
 * The admin file of the module.
 */

/**
 * Configure how OAuth2 clients integrated with Drupal site.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 *
 * @return array
 *   Form definition array.
 */
function moha_user_oauth2_admin_form($form, &$form_state) {
  try {
    $integrations = moha_cache_variable_get(MOHA_USER__VARIABLE__OAUTH2_INTEGRATION);

    $form['tips'] = moha_form_element_container('Tips');
    $form['tips']['paths'] = array(
      '#type' => 'item',
      '#title' => 'Path of Pages',
      '#description' => 'If more than one page need be configured, please list path of pages in each own line in textarea.',
    );

    $rows = array();
    $entities = moha_user_fetch_oauth2_client();
    foreach ($entities as $entity) {
      $integration = moha_an2e($integrations, $entity->name, array());

      $rows[$entity->name]['Name'] = l($entity->human_name, MOHA_USER__URL__OAUTH2_CLIENT_ADMIN_UI . '/manage/' . $entity->id);
      $rows[$entity->name]['Integration Approach'] = moha_form_element_radio(
        '', '',
        moha_an2e($integration, 'integration_approach', 0),
        MOHA_USER__OAUTH2__INTEGRATION_APPROACHES
      );
      $rows[$entity->name]['Paths of pages'] = moha_form_element_textarea('', '', moha_an2e($integration, 'paths_of_pages', ''));
      $rows[$entity->name]['Weight'] = moha_form_element_weight(moha_an2e($integration, 'weight', 0));
    }

    $form['oauth2_integration'] = moha_form_element_table($rows);

    moha_form_actions($form);
  }
  catch (Exception $e) {
    moha_show_exception($e);
    watchdog_exception(__FUNCTION__, $e);
  }
  
  return $form;
}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $oauth2_integration = $values['oauth2_integration'];
  $oauth2_integration = moha_sort_per_weight($oauth2_integration);
  moha_variable_set(MOHA_USER__VARIABLE__OAUTH2_INTEGRATION, $values['oauth2_integration']);
  moha_cache_set(MOHA_USER__VARIABLE__OAUTH2_INTEGRATION, $values['oauth2_integration']);

  drupal_set_message(t('Configurations have been saved.'));
}

/**
 * Mapping OAuth2 attribute 
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 * 
 * @param $client_name
 *   OAuth2 client name.
 *
 * @return array
 *   Form definition array.
 *
 * @linke admin/moha/user/oauth2/mapping/% @endlink
 */
function moha_user_oauth2_mapping_form($form, &$form_state, $client_name) {
  try {
    if (is_object($client_name)) {
      $client_name = $client_name->name;
    }

    $entity = moha_user_fetch_oauth2_client($client_name, array('status' => array()));
    $mapping = $entity->mapping;
    if (empty($mapping)) {
      $mapping = array();
    }
    $profile_json = moha_an2e($_SESSION, MOHA_USER__SESSION__OAUTH2_USER, array());

    if (empty($profile_json)) {
      moha_user_oauth2_login($client_name, "admin/moha/user/oauth2/mapping/$client_name", TRUE);
      return $form;
    }
    
    $options = array_keys($profile_json);
    $options = drupal_map_assoc($options);

    $form['client_name'] = array(
      '#type' => 'hidden',
      '#value' => $client_name,
    );

    $container = &moha_form_set_inline_container($form['container']);

    $container['name'] = moha_form_element_select(
      'Name',
      'Which attribute of OAuth2 profile will be used as username.',
      moha_an2e($mapping, 'name'),
      $options,
      TRUE
    );

    $container['mail'] = moha_form_element_select(
      'Mail',
      'Which attribute of OAuth2 profile will be used as mail address.',
      moha_an2e($mapping, 'mail'),
      $options,
      TRUE
    );

    $form['hint'] = moha_form_element_container('OAuth2 profile');
    $form['hint']['profile'] = array(
      '#markup' => format_string('<pre>!profile_json</pre>', array('!profile_json' => print_r($profile_json, true))),
    );

    moha_form_actions($form, 'Save');
    moha_form_actions_link($form, array('All OAuth2 Client' => 'admin/moha/entities/oauth2-clients'));
  }
  catch (Exception $e) {
    moha_show_exception($e);
    watchdog_exception(__FUNCTION__, $e);
  }

  return $form;
}

/**
 * Validate if selected fields could be used as user name, mail address, etc.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_mapping_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $name = moha_an2e($values, 'name');
  $mail = moha_an2e($values, 'mail');
  
  if (valid_email_address($mail)) {
    form_error($form['container']['mail'], t('Format of e-mail address is not correct. Please choose a suitable field to map as e-mail address.'));
  }
}

/**
 * Save relation ship between drupal user attributes and OAuth2 profile attributes.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_oauth2_mapping_form_submit($form, &$form_state) {
  try {
    $values = $form_state['values'];

    $mapping['name'] = moha_an2e($values, 'name');
    $mapping['mail'] = moha_an2e($values, 'mail');
    $client_name = moha_an2e($values, 'client_name');

    /** @var MohaUserOAuth2ClientEntity $entity */
    /** @see MohaCommonEntityController::save() */
    $entity = moha_user_fetch_oauth2_client($client_name, array('status' => array()));
    $entity->mapping = $mapping;
    $entity->save();

    drupal_set_message(t('Configurations have been saved.'));
  }
  catch (Exception $e) {
    moha_show_exception($e);
    watchdog_exception(__FUNCTION__, $e);
  }
}


/**
 * Become to another user information user provides.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 *
 * @return array
 *   Form definition array.
 */
function moha_user_simulation_form($form, &$form_state) {
  $form['uid'] = moha_form_element_text('User ID', 'Specify UID whom to become to.'); 
  $form['mail'] = moha_form_element_text('Mail', 'Specify user\'s mail address whom to become to.');
  moha_form_actions($form, 'Simulate');

  return $form;
}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_simulation_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_user_simulation_form_submit($form, &$form_state) {
  try {
    $values = $form_state['values'];
    if (isset($values['uid']) && trim($values['uid']) != ''){
      $user_info = trim($values['uid']);
    }
    else if (isset($values['mail']) && trim($values['mail']) != '') {
      $user_info = trim($values['mail']);
    }

    if (isset($user_info)) {
      moha_login($user_info, TRUE);
      drupal_set_message(t('User change action is performed, check current user please.'));
    }
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }
}

/**
 * Centralized logout page.
 */
function moha_user_logout() {
  try {
    // Prepare links to logout OAuth2 Platform.
    $entities = moha_user_fetch_oauth2_client();
    foreach ($entities as $entity) {
      if (isset($_SESSION[MOHA_USER__SESSION__OAUTH2_USER][$entity->name])) {
        $contents[] = array(
          '#markup' => '<p>' . moha_link("Logout $entity->human_name", $entity->logout) . '</p>',
        );
      }
    }
    $contents[] = array(
      '#markup' => l('Drupal Logout', 'user/logout'),
    );

    return $contents;
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }

}
