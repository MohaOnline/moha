<?php
/**
 * @file
 * Administrative page for the module.
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
 *
 * @return array
 *   Form definition array.
 *
 * @link /sites/all/modules/custom/moha/modules/moha_ui/bower_components/admin-lte/starter.html @endlink
 */
function moha_ui_dashboard_overview_form($form, &$form_state) {

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
function moha_ui_dashboard_overview_form_validate($form, &$form_state) {
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
function moha_ui_dashboard_overview_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  drupal_set_message(t('Configurations have been saved.'));
}

/**
 * Configure Moha Dashboard.
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
function moha_ui_admin_dashboard_form($form, &$form_state) {

  // Fetch all available hook implements which generate dashboard sidebar tab.
  foreach (module_implements('moha_ui_dashboard_sidebar') as $module) {
    $function = $module . '_moha_ui_dashboard_sidebar';
    $options[$module] = $function;
  }
  
  $default = moha_cache_variable_get(MOHA_UI__VARIABLE__DASHBOARD_SIDEBAR, array());
  if (!empty($options)) {
    $form['sidebar'] = moha_form_element_checks(
      'Dashboard tabs',
      'Select which tab(s) generating hook implements are active.',
      $default,
      $options
    );
  }
  
  moha_form_actions($form);
  
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
function moha_ui_admin_dashboard_form_validate($form, &$form_state) {
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
function moha_ui_admin_dashboard_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (isset($values['sidebar'])) {
    moha_cache_set(MOHA_UI__VARIABLE__DASHBOARD_SIDEBAR, $values['sidebar']);
    moha_variable_set(MOHA_UI__VARIABLE__DASHBOARD_SIDEBAR, $values['sidebar']);
    drupal_set_message(t('Configurations have been saved.'));
  }
}
