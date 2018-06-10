<?php
/**
 * @file
 */

/**
 * Implements hook_form().
 *
 * moha_commerce_order entity edit form.
 */
function moha_commerce_order_form($form, &$form_state, $order = NULL) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => !empty($order->id)?$order->id:'',
  );

  $form['nid'] = array(
    '#title' => t('Product'),
    '#type' => 'entityreference',
    '#description' => 'Which product is ordered.',
    '#required' => TRUE,
    '#default_value' => isset($order->nid)?$order->nid:'',
    '#era_entity_type' => 'node',
    '#era_cardinality' => 1,
    '#era_bundles' => array(__MOHA_COMMERCE_PRODUCT),
  );

  $form['oid'] = array(
    '#title' => t('Owner'),
    '#type' => 'entityreference',
    '#description' => t('Who ordered this product.'),
    '#required' => TRUE,
    '#era_entity_type' => 'user',
    '#era_cardinality' => 1,
    '#era_query_settings' => array(
      'property_conditions' => array(
      ),
    ),
    '#default_value' => isset($order->oid)?$order->oid:'',
  );

  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => array(
      0 => 'Dropped',
      1 => 'In-use',
      2 => 'Returned',
      3 => 'Transferred',
    ),
    '#default_value' => isset($order->status)?$order->status:1,
    '#description' => t('Status of order.'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($order->id)?'Save order':'Add order',
    ),
  );

  return $form;
}

/**
 * Validate form input.
 */
function moha_commerce_order_form_validate($node, &$form_state) {



}

/**
 * Save order status.
 */
function moha_commerce_order_form_submit($form, &$form_state) {
  global $user;

  // Build entity from submission.
  $order = entity_ui_form_submit_build_entity($form, $form_state);
  $order->uid = $user->uid;
  if (empty($order->id)) {
    $order->created = time();
  }
  $order->updated = time();
  $order->save();
  $form_state['redirect'] = 'admin/moha/commerce/orders';
}


/**
 * Configure order properties and limitations.
 */
function _moha_commerce_order_config_form($form, &$form_state) {

  $configurations = variable_get(MOHA_COMMERCE_ORDER__VARIABLES, array());
  $repeated_order_limit = isset($configurations['repeated_order_limit'])?$configurations['repeated_order_limit']:'90';

  $form['repeated_order_limit'] = array(
    '#title' => t('Repeated order limitation'),
    '#type' => 'textfield',
    '#description' => t('Set duration in day(s) to limit user cannot repeat to order product in same category, set to 0 without limitation.'),
    '#default_value' => $repeated_order_limit,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save',
    ),
  );

  return $form;
}

/**
 *
 */
function _moha_commerce_order_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $configurations = variable_get(MOHA_COMMERCE_ORDER__VARIABLES, array());
  $configurations['repeated_order_limit'] = $values['repeated_order_limit'];
  variable_set(MOHA_COMMERCE_ORDER__VARIABLES, $configurations);

  drupal_set_message('Your configuration has been saved.');
}
