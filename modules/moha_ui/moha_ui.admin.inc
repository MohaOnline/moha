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
function moha_ui_dashboard_form($form, &$form_state) {

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
function moha_ui_dashboard_form_validate($form, &$form_state) {
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
function moha_ui_dashboard_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  drupal_set_message(t('Configurations have been saved.'));
}
