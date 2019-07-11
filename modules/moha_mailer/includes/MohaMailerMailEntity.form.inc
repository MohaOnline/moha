<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * Entity edit form.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 *
 * @param $entity MohaMailerMailEntity
 *
 * @return mixed form array.
 */
function moha_mailer_mail_form($form, &$form_state, $entity = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($entity->id)?$entity->id:'',
  );

  $mail = &moha_form_set_inline_container($form['mail']);

  $mail['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#size' => 100,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#description' => t('Mail subject, max 512 characters are allowed.'),
    '#default_value' => isset($entity->subject)?$entity->subject:'',
  );

  $mail['mail_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#size' => 60,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#description' => t('Please specify mail address of sender.'),
    '#default_value' => isset($entity->mail_from)?$entity->mail_from:'',
  );

  $mail['mail_to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#size' => 60,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#description' => t('Please specify mail address of recipients.'),
    '#default_value' => isset($entity->mail_to)?$entity->mail_to:'',
  );

  $mail['nid'] = array(
    '#title' => t('Node'),
    '#type' => 'entityreference',
    '#size' => 100,
    '#description' => t('Please select which node to be sent.'),
    '#era_entity_type' => 'node',
    '#required' => TRUE,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($entity->nid)?$entity->nid:'',
    '#attributes' => array('class' => array('container-inline')),
  );

  $mail['eid'] = array(
    '#title' => t('Entity ID'),
    '#type' => 'entityreference',
    '#size' => 30,
    '#description' => t('Please select recipient user ID.'),
    '#era_entity_type' => 'user',
    '#required' => TRUE,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($entity->eid)?$entity->eid:'',
    '#attributes' => array('class' => array('container-inline')),
  );


  moha_form_add_vertical_tab($form,'properties');

  $form['properties']['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($entity->status)?$entity->status:3,
    '#description' => t('Status of the mail.'),
  );
  moha_form_apply_inline_wrapper($form['properties']['status']);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($entity->is_new)?'Add':'Save',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_mailer_mail_form_validate($node, &$form_state) {

}

/**
 * Save order status.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_mailer_mail_form_submit($form, &$form_state) {

  // Build entity from submission.
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  $entity->nid = $entity->nid['entity_id'];
  $entity->eid = $entity->eid['entity_id'];
  $entity->eid_type = 'user';

  /** @see \MohaMailerMailEntityController::save()*/
  $entity->save();

  $form_state['redirect'] = MOHA_MAILER__URL__MAIL_ADMIN_UI;
}
