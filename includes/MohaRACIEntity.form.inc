<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_raci entity edit form.
 *
 * @param $raci
 *
 * @return mixed form array.
 */
function moha_raci_form($form, &$form_state, $raci = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($raci->id)?$raci->id:'',
  );

  $form['eid'] = array(
    '#title' => t('Parent group / company'),
    '#type' => 'entityreference',
    '#description' => t('Specify parent group / company.'),
    '#era_entity_type' => __MOHA_ITS_GROUP,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($raci->pid)?$raci->pid:'',
  );

  $form['oid'] = array(
    '#title' => t('Representative'),
    '#type' => 'entityreference',
    '#description' => t('Specify who represents this group / company.'),
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($raci->oid)?$raci->oid:'',
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($raci->status)?$raci->status:1,
    '#description' => t('Status of group.'),
  );

  field_attach_form(__MOHA_ITS_GROUP, $raci, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($raci->is_new)?'Add':'Save',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_raci_form_validate($node, &$form_state) {

}

/**
 * Save entity.
 */
function moha_raci_form_submit($form, &$form_state) {

  // Build entity from submission.
  $order = entity_ui_form_submit_build_entity($form, $form_state);

  $order->rid = $order->rid['entity_id'];

  /** @see \MohaRACIEntityController::save()*/
  $order->save();
  $form_state['redirect'] = MOHA__URL__RACI_ADMIN_UI;
}
