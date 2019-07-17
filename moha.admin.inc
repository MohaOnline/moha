<?php
/**
 * @file
 *   Administrative page for handling moha root functions.
 */

/**
 * Collect schema function name.
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

  $form['hook_schema'] = array(
    '#type' => 'textfield',
    '#title' => t('hook_schema'),
    '#default_value' => '',
    '#size' => 40,
    '#description' => t('Specify your hook_schema function name to re-deploy or DB update function name to execute.'),
    '#field_suffix' => '()',
  );

  moha_form_actions($form, t('Execute'));

  return $form;
}

/**
 * Execute DB Update or re-deploy schema, only missing structure will be deployed.
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

    if (function_exists($hook_schema)) {
      $hook_schema();
      drupal_set_message("$hook_schema provided DB Update have been performed.");
    }
    else {
      $module_name = str_replace('_schema', '', $hook_schema);

      if (!module_exists($module_name)) {
        drupal_set_message("$hook_schema is not a function, or $module_name is not enabled.", "error");
      }
      module_load_include('install', $module_name, $module_name);
      moha_schema_create_by_function($hook_schema);

      drupal_set_message("$hook_schema provided schemas have been deployed.");
    }

  }
  catch (Exception $e){
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }
}

