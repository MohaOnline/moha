<?php
/**
 * @file
 */

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
    '#description' => t('Specify mail address of the sender, format: DISPLAY_NAME MAIL_ADDRESS.'),
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

  $mail['node'] = array(
    '#title' => t('Node'),
    '#type' => 'entityreference',
    '#description' => t('Select node which will be sent as mail.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#era_entity_type' => 'node',
    '#era_cardinality' => 1,
    '#era_bundles' => array('page'),
  );

  if (module_exists(__MOHA_NEWSLETTER)) {
    $mail['node']['#era_bundles'][] = __MOHA_NEWSLETTER;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Mail'),
  );

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

  $mail_to = $values['to'];
  moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_TO, $mail_to);

  $mail_from = $values['from'];
  moha_variable_set(MOHA_MAILER__VARIABLE__MANUAL_FROM, $mail_from);


  $node = node_load($values['node']['entity_id']);
  $params['subject'] = $node->title;
  $content = node_view($node);
  $params['body'][] = drupal_render($content['body']);
  libraries_load(__MOHA_LIBS);
  $message = drupal_mail(__MOHA_MAILER, __FUNCTION__, $mail_to, $language, $params, $mail_from);

  if ($message['send'] === TRUE && $message['result'] === TRUE) {
    drupal_set_message(t('Specified node has been mailed out.'));
  }

}