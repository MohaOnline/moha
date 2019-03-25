<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_its_infrastructure entity edit form.
 *
 * @param $infrastructure
 *
 * @return mixed form array.
 */
function moha_its_infrastructure_form($form, &$form_state, $infrastructure = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($infrastructure->id)?$infrastructure->id:'',
  );

  $form['full_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full name'),
    '#default_value' => empty($infrastructure->full_name)?'':$infrastructure->full_name,
    '#description' => t('Human name of the service.'),
    '#maxlength' => 128,
    '#size' => 120,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#description' => t('A unique name from infrastructure platform.'),
    '#default_value' => empty($infrastructure->name)?'':$infrastructure->name,
    '#machine_name' => array(
      'source' => array('full_name'), // Bind machine_name field to relative normal field.
      'label' => t('URI'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
      'exists' => 'moha_its_group_entity_exists',
    ),
  );

  $options = moha_form_options_from_vocabulary(MOHA_ITS__VOCABULARY__SERVICE_TYPES);

  $form['tid'] = array(
    '#title' => t('Service type'),
    '#type' => 'select',
    '#options' => $options,
    '#empty_value' => '',
    '#default_value' => isset($infrastructure->tid)?$infrastructure->tid:'',
    '#required' => TRUE,
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => MOHA__STATUS__ENTITY,
    '#default_value' => isset($infrastructure->status)?$infrastructure->status:1,
    '#description' => t('Status of this service.'),
  );

  // Attach entity fields.
  moha_form_add_entity_fields_wrapper($form);
  field_attach_form(__MOHA_ITS_SERVICE, $infrastructure, $form['fields']['wrapper'], $form_state);

  moha_form_add_vertical_tab($form,'properties');

  $form['properties']['gid'] = array(
    '#title' => t('Group / company'),
    '#type' => 'entityreference',
    '#description' => t('Specify which group / company this service belong to.'),
    '#era_entity_type' => __MOHA_ITS_GROUP,
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($infrastructure->gid)?$infrastructure->gid:'',
  );

  $form['properties']['oid'] = array(
    '#title' => t('Owner'),
    '#type' => 'entityreference',
    '#description' => t('Specify who owns this service.'),
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($infrastructure->oid)?$infrastructure->oid:'',
  );

  if (isset($infrastructure->is_new)) {
    moha_form_add_vertical_tab($form,'RACI');

    $form['RACI']['new'] = array(
      '#type' => 'fieldset',
      '#title' => 'Associate new user',
      '#description' => 'Associate new user with this service.',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    moha_form_set_inline_container($form['RACI']['new']['container']);

    $raci_new_container = &$form['RACI']['new']['container'];

    $raci_new_container['user'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
    );

    // fix input width in vertical tab.
    moha_form_input_fix_on_vertical_tab($raci_new_container['user']);

    $raci_new_container['role'] = array(
      '#title' => t('Role'),
      '#type' => 'select',
      '#empty_value' => '',
      '#options' => MOHA__STATUS__RACI,
    );

    $raci_new_container['submit'] = array(
      '#value' => t('Associate'),
      '#type' => 'submit',
    );

  } // RACI tab end.

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($infrastructure->is_new)?'Add':'Save',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_its_infrastructure_form_validate($node, &$form_state) {

}

/**
 * Save moha_its_service entity.
 */
function moha_its_infrastructure_form_submit($form, &$form_state) {

  // Build entity from submission.
  $service = entity_ui_form_submit_build_entity($form, $form_state);

  $service->oid = $service->oid['entity_id'];

  /** @see \MohaITSInfrastructureEntityController::save()*/
  $service->save();

  field_attach_submit(__MOHA_ITS_SERVICE, $service, $form['fields']['wrapper'], $form_state);

  $form_state['redirect'] = MOHA_ITS__URL__SERVICE_ADMIN_UI;
}
