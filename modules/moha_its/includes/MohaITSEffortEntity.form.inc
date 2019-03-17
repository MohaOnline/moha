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
function moha_its_effort_form($form, &$form_state, $effort = NULL) {
  global $user;

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($effort->id)?$effort->id:'',
  );

  $form['title'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );

  $form['oid'] = array(
    '#title' => t('Owner'),
    '#type' => 'entityreference',
    '#size' => 30,
    '#description' => t('Please select who spent this effort.'),
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($effort->oid)?$effort->oid:$user->uid,
    '#attributes' => array('class' => array('container-inline')),
  );
  moha_form_apply_inline_wrapper($form['oid']);

  $options = moha_form_options_from_vocabulary(MOHA_ITS__VOCABULARY__EFFORT_TYPES);

  $form['tid'] = array(
    '#title' => t('Effort type'),
    '#type' => 'select',
    '#options' => $options,
    '#empty_value' => '',
    '#default_value' => isset($effort->tid)?$effort->tid:'',
    '#required' => TRUE,
  );
  moha_form_apply_inline_wrapper($form['tid']);

  $form['date_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date'),
    '#description' =>t('Specify from when and how long this effort was spent:'),
    '#collapsible' => TRUE,
  );

  $date_form = &$form['date_wrapper'];

  $now = (new DateTime())->format(MOHA__DATETIME_FORMAT__SHORT);

  $date_form['date'] = array(
    '#type' => 'date_popup',
    '#default_value' => isset($effort->date)?$effort->date:$now,
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => MOHA__DATETIME_FORMAT__SHORT,
    '#date_year_range' => '-2:+3',
    '#date_increment' => 15,
  );

  $date_form['duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 10,
    '#description' => t('hrs.'),
    '#default_value' => isset($effort->duration)?$effort->duration:'',
  );
  moha_form_apply_inline_wrapper($date_form['duration']);

  $form['target_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Target'),
    '#description' =>t('Specify service information, on which service current effort was spent on:'),
    '#collapsible' => TRUE,
  );

  moha_form_set_inline_container($form['target_wrapper']['target']);
  $target_form = &$form['target_wrapper']['target'];

  $target_form['gid'] = array(
    '#title' => t('Group / company'),
    '#type' => 'entityreference',
    '#size' => 20,
    '#era_entity_type' => __MOHA_ITS_GROUP,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($effort->gid)?$effort->gid:'',
  );

  $target_form['sid'] = array(
    '#title' => t('Service'),
    '#type' => 'entityreference',
    '#size' => 40,
    '#era_entity_type' => __MOHA_ITS_SERVICE,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($effort->sid)?$effort->sid:'',
  );

  moha_form_add_entity_fields_wrapper($form);
  field_attach_form(__MOHA_ITS_EFFORT, $effort, $form['fields']['wrapper'], $form_state);

  moha_form_add_vertical_tab($form,'properties');

  $form['properties']['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($effort->status)?$effort->status:1,
    '#description' => t('Status of effort.'),
  );
  moha_form_apply_inline_wrapper($form['properties']['status']);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($effort->is_new)?'Add':'Save',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_its_effort_form_validate($node, &$form_state) {

}

/**
 * Save order status.
 */
function moha_its_effort_form_submit($form, &$form_state) {

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
