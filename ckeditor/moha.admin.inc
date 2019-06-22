<?php
/**
 * @file
 *   Administrative page for handling moha root functions.
 */

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
function moha_db_update_form ($form, &$form_state) {

  //
  $form['hook_schema'] = array(
    '#type' => 'textfield',
    '#title' => t('hook_schema'),
    '#default_value' => '',
    '#description' => t('Specify your hook_schema function name.'),
    '#field_suffix' => '()',
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
  );
}

/**
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_db_update_form_submit($form, &$form_state) {
  try {
    $values = $form_state['values'];

    $hook_schema = $values['hook_schema'];
    $module_name = str_replace('_schema', '', $hook_schema);
    module_load_include('install', $module_name, $module_name);
    moha_schema_create_by_function($hook_schema);

    drupal_set_message("$hook_schema provided schemas have been deployed.");
  }
  catch (Exception $e){
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }
}

