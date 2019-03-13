<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_its_group entity edit form.
 *
 * @param $group
 *
 * @return mixed form array.
 */
function moha_its_operation_form($form, &$form_state, $group = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($group->id)?$group->id:'',
  );

  $form['full_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full name'),
    '#default_value' => empty($group->full_name)?'':$group->full_name,
    '#description' => t('English full name of group.'),
    '#maxlength' => 128,
    '#size' => 120,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#description' => t('A unique name to construct the URL for the group. It must only contain lowercase letters, numbers and hyphens.'),
    '#default_value' => empty($group->name)?'':$group->name,
    '#machine_name' => array(
      'source' => array('full_name'), // Bind machine_name field to relative normal field.
      'label' => t('URI'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
      'exists' => 'moha_its_group_entity_exists',
    ),
  );

  $form['local_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Local name'),
    '#default_value' => empty($group->local_name)?'':$group->local_name,
    '#description' => t('The group\'s local full name.'),
    '#maxlength' => 128,
    '#size' => 120,
  );

  $form['pid'] = array(
    '#title' => t('Parent group / company'),
    '#type' => 'entityreference',
    '#description' => t('Specify parent group / company.'),
    '#era_entity_type' => __MOHA_ITS_GROUP,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($group->pid)?$group->pid:'',
  );

  $form['rid'] = array(
    '#title' => t('Representative'),
    '#type' => 'entityreference',
    '#description' => t('Specify who represents this group / company.'),
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($group->oid)?$group->oid:'',
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($group->status)?$group->status:1,
    '#description' => t('Status of group.'),
  );

  field_attach_form(__MOHA_ITS_GROUP, $group, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($group->is_new)?'Add group':'Save group',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_its_operation_form_validate($node, &$form_state) {

}

/**
 * Save order status.
 */
function moha_its_operation_form_submit($form, &$form_state) {

  // Build entity from submission.
  $order = entity_ui_form_submit_build_entity($form, $form_state);

  $order->rid = $order->rid['entity_id'];

  /** @see \MohaITSServiceEntityController::save()*/
  $order->save();
  $form_state['redirect'] = MOHA_ITS__URL__OPERATION_ADMIN_UI;
}
