<?php
/**
 * @file
 *   Management functions of the module.
 */

/**
 * Definition of the module configuration form.
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
function moha_mailer_config_form($form, &$form_state) {
  $config = &$form['config'];

  $config['threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron sending threshold'),
    '#description' => t('Max mails which will be sent through each cron job.'),
    '#size' => 10,
    '#default_value' => moha_variable_get(MOHA_MAILER__VARIABLE__THRESHOLD, 20),
    '#required' => TRUE,
  );

  $config['suspend'] = array(
    '#title' => t('Suspend queued mail sending'),
    '#description' => t('Choose Yes to suspend queued mail sending.'),
    '#required' => TRUE,
  );
  moha_form_radio($config['suspend'], moha_cache_get(MOHA_MAILER__VARIABLE__SUSPEND, moha_variable_get(MOHA_MAILER__VARIABLE__SUSPEND, FALSE), FALSE));

  $config['ignore_cc'] = array(
    '#title' => t('Ignore passed Cc recipients'),
    '#description' => t('Choose Yes to ignore passed Cc recipients when mail sending through cron.'),
    '#required' => TRUE,
  );
  moha_form_radio($config['ignore_cc'], moha_cache_get(MOHA_MAILER__VARIABLE__IGNORE_CC, moha_variable_get(MOHA_MAILER__VARIABLE__IGNORE_CC, FALSE), FALSE));

  moha_form_actions($form);

  return $form;
}

/**
 * Validation of the module configuration form.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_mailer_config_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!is_numeric($values['threshold'])) {
    form_error($form['config']['threshold'], t('Please specify a number for %field_title field.', array('%field_title' => $form['config']['threshold']['#title'])));
  }
}

/**
 * Submission of the module configuration form.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_mailer_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  moha_variable_set(MOHA_MAILER__VARIABLE__THRESHOLD, $values['threshold']);

  moha_form_radio_cache_save($values['suspend'], MOHA_MAILER__VARIABLE__SUSPEND);
  moha_form_radio_cache_save($values['ignore_cc'], MOHA_MAILER__VARIABLE__IGNORE_CC);

  drupal_set_message(t('Configurations have been saved.'));
}

/**
 * Collect mail information for sending mail one by one.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 *
 * @see \form_builder()
 */
function moha_mailer_manual_form($form, &$form_state) {

  moha_form_set_inline_container($form['mail']);

  $mail = &$form['mail'];

  $mail['from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#description' => t('Specify mail address of the sender, format: %format.', array('%format' => 'DISPLAY_NAME <MAIL@ADDRESS>')),
    '#size' => 40,
    '#default_value' => moha_variable_get(MOHA_MAILER__VARIABLE__MANUAL_FROM, ''),
    '#required' => TRUE,
  );

  $mail['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#description' => t('Specify mail address of recipients.'),
    '#size' => 40,
    '#default_value' => moha_variable_get(MOHA_MAILER__VARIABLE__MANUAL_TO, ''),
    '#required' => TRUE,
  );

  $mail['cc'] = array(
    '#type' => 'textfield',
    '#title' => t('Cc'),
    '#description' => t('Specify mail address of cc-ed recipients.'),
    '#size' => 40,
    '#default_value' => moha_variable_get(MOHA_MAILER__VARIABLE__MANUAL_CC, ''),
  );

  $mail['bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('Bcc'),
    '#description' => t('Specify mail address of bcc-ed recipients.'),
    '#size' => 40,
    '#default_value' => moha_variable_get(MOHA_MAILER__VARIABLE__MANUAL_BCC, ''),
  );

  $mail['node'] = array(
    '#title' => t('Node'),
    '#type' => 'entityreference',
    '#description' => t('Select basic page which will be sent as mail.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#era_entity_type' => 'node',
    '#era_cardinality' => 1,
    '#era_bundles' => array('page'),
  );

  if (module_exists(__MOHA_NEWSLETTER)) {
    $mail['node']['#era_bundles'] = array(__MOHA_NEWSLETTER);
    $mail['node']['#description'] = t("Select a %type which will be sent as mail.", array('%type' => MOHA_NEWSLETTER));
  }

  $mail['user'] = array(
    '#title' => t('User'),
    '#type' => 'entityreference',
    '#size' => 40,
    '#description' => t('Select who this mail will be sent to.'),
    '#default_value' => '',
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_bundles' => array('user'),
  );

  moha_form_actions($form, t('Send Mail'));

  return $form;
}

/**
 * Send mail per provided information.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_mailer_manual_form_submit($form, &$form_state) {

  global $language;
  $values = $form_state['values'];

  $mail_from = $values['from'];
  moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_FROM, $mail_from);

  $mail_to = $values['to'];
  moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_TO, $mail_to);

  $mail_cc = $values['cc'];
  if (!empty($mail_cc)) {
    $params['headers']['Cc'] = $mail_cc;
    moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_CC, $mail_cc);
  }

  $mail_bcc = $values['bcc'];
  if (!empty($mail_bcc)) {
    $params['headers']['Bcc'] = $mail_bcc;
    moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_BCC, $mail_bcc);
  }

  $mail_user = $values['user'];
  if ($mail_user) {
    $mail_user = user_load($mail_user['entity_id']);
    $params['context']['user'] = $mail_user;
  }
  $params['attachments'] = '';

  $node = node_load($values['node']['entity_id']);

  $params['subject'] = $node->title;
  $params['context']['subject'] = $node->title;

  $content = node_view($node);
  $body = drupal_render($content['body']);
  $params['body'][] = $body;
  $params['context']['body'] = $body;
  $params['plaintext'] = drupal_html_to_text($body);

  $message = drupal_mail(__MOHA_MAILER, 'formal', $mail_to, $language, $params, $mail_from);

  if ($message['send'] === TRUE && $message['result'] === TRUE) {
    drupal_set_message(t('Specified contents have been mailed out.'));
  }

}
