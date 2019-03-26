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

  $entity_options = array();
  $entities = entity_get_info();

  foreach ($entities as $entity_name => $entity) {
    if ($entity_name == __MOHA_RACI) {
      continue;
    }
    $entity_options[$entity_name] = $entity['label'];
  }

  if (!empty($form_state['values']['entity'])) {
    $entity_type = $form_state['values']['entity'];
  }
  elseif (!empty($raci->entity)) {
    $entity_type = $raci->entity;
  }
  else {
    $entity_type = 'node';
  }

  $form['entity'] = array(
    '#title' => t('Entity Type'),
    '#type' => 'select',
    '#options' => $entity_options,
    '#empty_value' => '',
    '#default_value' => $entity_type,
    '#description' => t('Select entity type of which need to be assigned an owner.'),
    '#ajax' => array(
      // #ajax has two required keys: callback and wrapper.
      // 'callback' is a function that will be called when this element changes.
      'callback' => '_moha_form_entity_raci_form_entity_type_change_ajax_callback',
      // 'wrapper' is the HTML id of the page element that will be replaced.
      // Can't use form ID as after ajax process, ID will suffixed by '--2'.
      'wrapper' => 'moha-raci-form-eid-wrapper',
      'effect' => 'fade',
    ),
  );

  $form['eid'] = array(
    '#title' => t('Entity ID'),
    '#type' => 'entityreference',
    '#description' => t('Specify entity ID of which need to be assigned an owner.'),
    '#era_entity_type' => $entity_type,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($raci->eid)?$raci->eid:'',
    '#prefix' => '<div id="moha-raci-form-eid-wrapper">',
    '#suffix' => '</div>',
  );

  if (!empty($form_state['values']['owner_entity'])) {
    $owner_entity_type = $form_state['values']['owner_entity'];
  }
  elseif (!empty($raci->owner_entity)) {
    $owner_entity_type = $raci->owner_entity;
  }
  else {
    $owner_entity_type = 'user';
  }

  $form['owner_entity'] = array(
    '#title' => t('Entity Type of owner'),
    '#type' => 'select',
    '#options' => $entity_options,
    '#empty_value' => '',
    '#default_value' => $owner_entity_type,
    '#description' => t("Select current RACI relationship owner's entity type."),
    '#ajax' => array(
      // #ajax has two required keys: callback and wrapper.
      // 'callback' is a function that will be called when this element changes.
      /** @see _moha_form_entity_raci_form_owner_entity_type_change_ajax_callback() */
      'callback' => '_moha_form_entity_raci_form_owner_entity_type_change_ajax_callback',
      // 'wrapper' is the HTML id of the page element that will be replaced.
      // Can't use form ID as after ajax process, ID will suffixed by '--2'.
      'wrapper' => 'moha-raci-form-oid-wrapper',
      'effect' => 'fade',
    ),
  );

  $form['oid'] = array(
    '#title' => t('Entity ID of owner'),
    '#type' => 'entityreference',
    '#description' => t('Specify who owns current RACI relationship.'),
    '#era_entity_type' => $owner_entity_type,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($raci->oid)?$raci->oid:'',
    '#prefix' => '<div id="moha-raci-form-oid-wrapper">',
    '#suffix' => '</div>',
  );

  $form['raci'] = array(
    '#title' => t('RACI Relationship'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__RACI,
    '#empty_value' => '',
    '#default_value' => isset($raci->raci)?$raci->raci:0,
    '#description' => t('Select RACI relationship between entity and owner.'),
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($raci->status)?$raci->status:1,
    '#description' => t('Select status of current relationship.'),
  );

  field_attach_form(__MOHA_RACI, $raci, $form, $form_state);

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
  $raci = entity_ui_form_submit_build_entity($form, $form_state);

  $raci->eid = $raci->eid['entity_id'];
  $raci->oid = $raci->oid['entity_id'];

  /** @see \MohaRACIEntityController::save()*/
  $raci->save();
  $form_state['redirect'] = MOHA__URL__RACI_ADMIN_UI;
}
