<?php
/**
 * @file
 * Administrative page for handling ckeditor upload server end configuration.
 */

/**
 * Configure storage for uploaded files by CKEditor.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_ckeditor_upload_form($form, &$form_state) {

  $ckeditor_destination = moha_array_variable_get(MOHA__VARIABLES, 'ckeditor_destination', 'public');
  $ckeditor_directory = moha_array_variable_get(MOHA__VARIABLES, 'ckeditor_directory', 'upload');

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['ckeditor_destination'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $ckeditor_destination,
    '#description' => t('Public files upload destination is dangerous for forms that are available to anonymous and/or untrusted users. For more information, see <a target="_blank" href="@psa">DRUPAL-PSA-2016-003</a>.', array('@psa' => 'https://www.drupal.org/psa-2016-003')),
    '#access' => count($scheme_options) > 1,
  );
  $form['ckeditor_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload directory'),
    '#default_value' => $ckeditor_directory,
    '#description' => t('You may optionally specify a sub-directory to store your files.') . ' ' . theme('webform_token_help'),
    '#field_prefix' => 'moha/ckeditor/',
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
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
function moha_ckeditor_upload_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $ckeditor_destination = $values['ckeditor_destination'];
  $ckeditor_directory = $values['ckeditor_directory'];

  moha_array_variable_set(MOHA__VARIABLES, 'ckeditor_destination', $ckeditor_destination);
  moha_array_variable_set(MOHA__VARIABLES, 'ckeditor_directory', $ckeditor_directory);

  drupal_get_messages(t('Configurations have been saved.'));
}
