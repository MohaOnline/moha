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
function moha_its_group_form($form, &$form_state, $group = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($group->id)?$group->id:'',
  );

  $form['human_name'] = moha_form_element_text(
    'Human name',
    'Human readable name of the group.',
    empty($group->human_name) ? '' : $group->human_name,
    120, 128, TRUE
  );

  $form['name'] = moha_form_element_machine(
    'human_name',
    empty($group->name)?'':$group->name,
    /** @see moha_entity_exists() */
    'moha_entity_exists',
    TRUE
  );

  $form['local_name'] = moha_form_element_text(
    'Local name',
    'Local Human name of the group.',
    empty($group->local_name) ? '' : $group->local_name,
    120, 128
  );

  $form['pid'] = moha_form_element_entity_ref(
    'Parent group / company',
    'Specify parent group / company.',
    isset($group->pid) ? $group->pid : '',
    __MOHA_ITS_GROUP
  );

  $form['rid'] = moha_form_element_entity_ref(
    'Representative',
    'Specify who represents this group / company.',
    isset($group->rid) ? $group->rid : '',
    'user'
  );

  $form['status'] = moha_form_element_select(
    'Status',
    'Status of the entity.',
    isset($group->status) ? $group->status : 1,
    MOHA__STATUS__ENTITY
  );

  moha_form_add_entity_fields(__MOHA_ITS_GROUP, $group, $form, $form_state);

  moha_form_actions($form, isset($group->is_new) ? 'Add group' : 'Save group');

  return $form;
}

/**
 * Validate form input.
 */
function moha_its_group_form_validate($node, &$form_state) {

}

/**
 * Save order status.
 */
function moha_its_group_form_submit($form, &$form_state) {

  // Build entity from submission.
  $order = entity_ui_form_submit_build_entity($form, $form_state);

  $order->rid = $order->rid['entity_id'];

  /** @see \MohaITSInfrastructureEntityController::save()*/
  $order->save();
  $form_state['redirect'] = MOHA_ITS__URL__GROUP_ADMIN_UI;
}
