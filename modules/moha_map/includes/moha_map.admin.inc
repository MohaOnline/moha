<?php
/**
 * @file
 */

/**
 * Provides management form for Moha Map modules:
 *  - Baidu Open Platform App Key;
 *
 * @return array
 */
function _moha_map_manager_form($form, &$form_state) {

  $baidu_access_key = moha_array_variable_get(MOAH_MAP__VARIABLES, 'baidu_access_key', '');

  $form['baidu_access_key'] = array(
    '#title' => t('Baidu access key'),
    '#type' => 'textfield',
    '#description' => t('Input Baidu LBS Cloud Service Access Key, click <a href="https://lbsyun.baidu.com/apiconsole/key" target="_blank">here</a> to check.'),
    '#default_value' => $baidu_access_key,
  );

  $form = system_settings_form($form);
  $form['#submit'] = array('_moha_map_manager_form_submit');

  return $form;
}

/**
 *
 *
 * @param $form
 * @param $form_state
 */
function _moha_map_manager_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (isset($values['baidu_access_key'])) {
    moha_array_variable_set(MOAH_MAP__VARIABLES, 'baidu_access_key', $values['baidu_access_key']);
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
