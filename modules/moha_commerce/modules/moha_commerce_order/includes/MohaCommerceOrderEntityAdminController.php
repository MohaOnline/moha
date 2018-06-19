<?php
/**
 * @file
 */

class MohaCommerceOrderEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_commerce_order on admin page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Orders Filter'),
      '#weight' => -99999,
      '#collapsible' => TRUE,
    );

    $form['filter']['user'] = array(
      '#title' => t('SSO'),
      '#type' => 'entityreference',
      '#description' => 'Specifies whose orders, enter SSO then choose from drop-down menu.',
      '#required' => FALSE,
      '#default_value' => isset($_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER'])?$_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER']:'',
      '#era_entity_type' => 'user',
      '#era_cardinality' => 1,
      '#era_bundles' => array('user'),
    );

    $form['filter']['actions'] = array(
      '#type' => 'actions',
    );

    $form['filter']['actions']['filter'] = array(
      '#type' => 'submit',
      '#value' => 'Filter',
    );

    $form['filter']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => 'Reset Filter',
    );

    $form['filter']['actions']['download'] = array(
      '#type' => 'submit',
      '#value' => 'Export All',
    );

    return parent::overviewForm($form, $form_state);
  }

  /**
   * Overview form submit callback.
   * Stores operation into session.
   *
   * @param $form
   *   The form array of the overview form.
   * @param $form_state
   *   The overview form state which will be used for submitting.
   */
  public function overviewFormSubmit($form, &$form_state) {
    $values = $form_state['values'];
    $op = $values['op'];
    $op_key = array_search($op, $values);

    if($op_key == 'reset'){
      $_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER'] = '';
    }
    else if ($op_key == 'filter') {
      $_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER'] = isset($values['user']['entity_id'])?$values['user']['entity_id']:'';
    }
    else if ($op_key == 'download') {

      $configurations = variable_get(MOHA_COMMERCE_ORDER__VARIABLES, array());
      $order_export_url = isset($configurations['order_export_url'])?$configurations['order_export_url']:'moha/commerce/order/export/csv';

      drupal_goto($order_export_url);
    }
  }

  /**
   * {@inheritdoc}
   *
   *  Adds property condition per user operation.
   */
  public function overviewTable($conditions = []) {
    if (!empty($_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER'])) {
      $conditions['oid'] = $_SESSION[__MOHA_COMMERCE_ORDER . '_FILTER_USER'];
    }

    $array = parent::overviewTable($conditions);

    return $array;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional column headers to entity admin list page.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = []) {

    $additional_header[] = t('SSO');
    $additional_header[] = t('Name');
    $additional_header[] = t('BU');
    $additional_header[] = t('Product');
    $additional_header[] = t('Status');
    $additional_header[] = t('Updated');
    $additional_header[] = t('Created');

    $header = $additional_header;

    // Replace label with ID.
    array_unshift($header, t('ID'));

    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }

    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount());

    return $header;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional columns to entity admin list page.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = []) {

    // SSO
    $owner = user_load($entity->oid);
    $additional_cols[] = $owner->name;  // SSO.

    // SAML name, and BU
    $profile = profile2_load_by_user($entity->oid, __MOHA_SAML);
    if ($profile) {
      // @see https://www.drupal.org/docs/7/api/entity-api/entity-metadata-wrappers.
      // entity.wrapper.inc.
      $wrapper = entity_metadata_wrapper('profile2', $profile);
      $additional_cols[] = $wrapper->moha_saml_formal_name->value();
      $additional_cols[] = $wrapper->moha_saml_business_unit->value();
    }
    else {
      $additional_cols[] = '';
      $additional_cols[] = '';
    }

    // Product name.
    $product = node_load($entity->nid);
    $additional_cols[] = $product->title;

    $additional_cols[] = moha_commerce_order_status_array()[$entity->status];

    // Order updated and created time.
    $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    return parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
  }
}
