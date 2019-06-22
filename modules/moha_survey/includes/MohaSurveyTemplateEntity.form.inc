<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_survey_template entity edit form.
 *
 * @param $entity object
 *   moha_survey_template entity.
 *
 * @return mixed form array.
 */
function moha_survey_template_form($form, &$form_state, $entity = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($entity->id)?$entity->id:'',
  );

  $form['nid'] = array(
    '#title' => t('Webform Node ID'),
    '#type' => 'entityreference',
    '#description' => t('Specify Webform node ID of which need to be pinned as template.'),
    '#era_entity_type' => 'node',
    '#era_cardinality' => 1,
    '#era_bundles' => array('webform'),
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($entity->nid)?$entity->nid:'',
    '#required' => TRUE,
  );

  $form['cid'] = array(
    '#type' => 'textfield',
    '#title' => t('Component ID'),
    '#description' => t('Component ID in which select options are stored.'),
    '#size' => 40,
    '#default_value' => isset($entity->cid)?$entity->cid:'',
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Template name'),
    '#description' => t('Survey template will reuse survey name, options template could be specified.'),
    '#size' => 80,
    '#default_value' => isset($entity->name)?$entity->name:'',
    '#required' => TRUE,
  );

  $form['referred'] = array(
    '#type' => 'textfield',
    '#title' => t('Referred number'),
    '#description' => t('Reuse times of this template.'),
    '#size' => 40,
    '#default_value' => isset($entity->name)?$entity->name:'',
    '#required' => TRUE,
  );

  $form['oid'] = array(
    '#title' => t('User ID of owner'),
    '#type' => 'entityreference',
    '#description' => t('Specify who owns current Survey template.'),
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($entity->oid)?$entity->oid:'',
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($entity->status)?$entity->status:1,
    '#description' => t('Select status of current relationship.'),
  );


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
 */
function moha_survey_template_form_validate($node, &$form_state) {

}

/**
 * Save entity.
 */
function moha_survey_template_form_submit($form, &$form_state) {

  // Build entity from submission.
  $raci = entity_ui_form_submit_build_entity($form, $form_state);

  $raci->eid = $raci->eid['entity_id'];
  $raci->oid = $raci->oid['entity_id'];

  /** @see \MohaRACIEntityController::save()*/
  $raci->save();
  $form_state['redirect'] = MOHA__URL__RACI_ADMIN_UI;
}
