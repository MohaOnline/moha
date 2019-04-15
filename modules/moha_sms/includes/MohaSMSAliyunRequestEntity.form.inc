<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_its_effort entity edit form.
 *
 * @param $effort
 *
 * @return mixed form array.
 */
function moha_sms_aliyun_request_form($form, &$form_state, $effort = NULL) {

  $form['Help'] = array(
    '#markup' => 'No details.'
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_sms_aliyun_request_form_validate($node, &$form_state) {

}

/**
 * Save order status.
 */
function moha_sms_aliyun_request_form_submit($form, &$form_state) {

  // Build entity from submission.
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  $entity->title = format_string('@datatime | @user_name(@uid) | @duration', array(
    '@group' => $entity->gid['entity_label'],
    '@gid' => $entity->gid['entity_id'],
    '@service' => $entity->sid['entity_label'],
    '@sid' => $entity->sid['entity_id'],
    '@user_name' => $entity->oid['entity_label'],
    '@uid' => $entity->oid['entity_id'],
    '@datatime' => $entity->date,
    '@duration' => $entity->duration,
  ));

  $entity->oid = $entity->oid['entity_id'];
  $entity->gid = $entity->gid['entity_id'];
  $entity->sid = $entity->sid['entity_id'];

  /** @see \MohaITSEffortEntityController::save()*/
  $entity->save();

  field_attach_submit(__MOHA_ITS_EFFORT, $entity, $form['fields']['wrapper'], $form_state);

  $form_state['redirect'] = MOHA_ITS__URL__EFFORT_ADMIN_UI;
}
