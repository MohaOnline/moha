<?php
/**
 * @file
 * Administrative page for the module.
 */

/**
 * Collect schema/install/enable or executable function name.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_db_update_form($form, &$form_state) {

  $form['hook_schema'] = array(
    '#type' => 'textfield',
    '#title' => t('hook_schema / function'),
    '#default_value' => '',
    '#size' => 40,
    '#description' => t('Specify hook_schema function name to re-deploy missing DB structure, or DB update function name to execute.'),
    '#field_suffix' => '()',
  );

  moha_form_actions($form, 'Execute');

  return $form;
}

/**
 * Execute DB Update or re-deploy schema, only missing structure will be deployed.
 * If specified function exists, execute directly.
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
      if (substr($hook_schema, -20) == 'fields_and_instances') {
        moha_create_fields_and_instances($hook_schema);
        watchdog( __FUNCTION__,
          "$hook_schema provided fields and instances have been redeployed.",
          array(
          ),
          WATCHDOG_INFO
        );
        drupal_set_message("$hook_schema provided fields and instances have been redeployed.");
      }
      else {
        $hook_schema();
        watchdog( __FUNCTION__,
          "$hook_schema provided DB Update have been performed.",
          array(
          ),
          WATCHDOG_INFO
        );
        drupal_set_message("$hook_schema provided DB Update have been performed.");
      }
    }
    else {
      $module_name = '';
      if (substr($hook_schema, -7) == '_schema' || substr($hook_schema, -7) == '_enable') {
        $module_name = substr($hook_schema, 0, -7);
      }
      else {
        if (substr($hook_schema, -8) == '_install') {
          $module_name = substr($hook_schema, 0, -8);
        }
      }

      if (module_exists($module_name)) {
        module_load_include('install', $module_name, $module_name);
        if (substr($hook_schema, -7) == '_schema') {
          moha_schema_create($hook_schema);
          watchdog( __FUNCTION__,
            "$hook_schema defined schemas have been deployed.",
            array(
            ),
            WATCHDOG_INFO
          );
          drupal_set_message("$hook_schema defined schemas have been deployed.");
        }
        else {
          $hook_schema();
          watchdog( __FUNCTION__,
            "$hook_schema provided statements have been performed.",
            array(
            ),
            WATCHDOG_INFO
          );
          drupal_set_message("$hook_schema provided statements have been performed.");
        }
      }
      else {
        watchdog(__FUNCTION__,
          "Nothing was executed, $hook_schema is not a function, or $module_name is not enabled.",
          array(),
          WATCHDOG_WARNING
        );
        drupal_set_message("Nothing was executed, $hook_schema is not a function, or $module_name is not enabled.", "warning");
        return;
      }
    }
  }
  catch (Exception $e){
    watchdog_exception(__FUNCTION__, $e);
    moha_show_exception($e);
  }
}

