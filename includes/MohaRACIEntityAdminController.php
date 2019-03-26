<?php
/**
 * @file
 */

class MohaRACIEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_raci on admin UI page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('RACI Filter'),
      '#weight' => -99999,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $form['filter']['user'] = array(
      '#title' => t('Representative name'),
      '#type' => 'entityreference',
      '#description' => 'Filter by group or company representative, enter representative\'s name then choose from drop-down menu.',
      '#required' => FALSE,
      '#default_value' => isset($_SESSION[__MOHA_RACI . '_FILTER_USER'])?$_SESSION[__MOHA_RACI . '_FILTER_USER']:'',
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
      '#value' => 'Exports All',
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
      $_SESSION[MOHA_RACI] = '';
    }
    else if ($op_key == 'filter') {
      $_SESSION[MOHA_RACI] = isset($values['user']['entity_id'])?$values['user']['entity_id']:'';
    }
    else if ($op_key == 'download') {

      $configurations = variable_get(MOHA_ITS__VARIABLES, array());
      $order_export_url = isset($configurations['order_export_url'])?$configurations['order_export_url']:'moha/commerce/order/export/csv';

      drupal_goto($order_export_url);
    }
  }

  /**
   * {@inheritdoc}
   *
   *  Renders whole management page independently.
   */
  public function overviewTable($conditions = []) {

    if (!empty($_SESSION[MOHA_RACI])) {
      $conditions['rid'] = $_SESSION[MOHA_RACI];
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    $query->propertyOrderBy('updated','DESC');
    $query->propertyOrderBy('status');

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();

    $rows = array();
    foreach ($entities as $entity) {
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity);
    }

    $render = array(
      '#theme' => 'table',
      '#header' => $this->overviewTableHeaders($conditions, $rows),
      '#rows' => $rows,
      '#empty' => t('None.'),
    );

    return $render;

  }

  /**
   * {@inheritdoc}
   *
   * Add additional column headers to entity admin list page.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = []) {


    $additional_header[] = t('ID');
    $additional_header[] = t('Entity Type');
    $additional_header[] = t('Entity');
    $additional_header[] = t('Owner Type');
    $additional_header[] = t('Owner');
    $additional_header[] = t('RACI');
    $additional_header[] = t('Status');
    $additional_header[] = t('Updated');
    $additional_header[] = t('Created');

    $header = $additional_header;


    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }

    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount()+1);

    return $header;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional columns to entity admin list page.
   */
  protected function overviewTableRow($conditions, $id, $raci, $additional_cols = []) {

    // Entity machine name.

    // Status
    $additional_cols[] = $raci->entity;

    $entity = entity_load_single($raci->entity, $raci->eid);
    $additional_cols[] = array('data' => array(
      '#theme' => 'entity_ui_overview_item',
      '#label' => entity_label($raci->entity, $entity),
      '#name' => !empty($entity->exportable) ? entity_id($raci->entity, $entity) : FALSE,
      '#url' => entity_uri($raci->entity, $entity),
      '#entity_type' => $raci->entity,
    ));

    $additional_cols[] = $raci->owner_entity;

    $owner = entity_load_single($raci->owner_entity, $raci->oid);
    $additional_cols[] = array('data' => array(
      '#theme' => 'entity_ui_overview_item',
      '#label' => entity_label($raci->owner_entity, $owner),
      '#name' => !empty($owner->exportable) ? entity_id($raci->owner_entity, $owner) : FALSE,
      '#url' => entity_uri($raci->owner_entity, $owner),
      '#entity_type' => $raci->owner_entity,
    ));

    $additional_cols[] = MOHA__STATUS__RACI[$raci->raci];
    $additional_cols[] = MOHA__STATUS__ENTITY[$raci->status];

    // Order updated and created time.
    $additional_cols[] = format_date($raci->updated, 'short');
    $additional_cols[] = format_date($raci->created, 'short');

    if ($raci->status == moha_array_key_by_value(MOHA__TERM__DISABLED, MOHA__STATUS__ENTITY)) {
      $additional_cols[] = l('Deliver', 'admin/moha/commerce/order/deliver/' . $raci->id, ['attributes' => ['class' => ['button']]]);
    }
    else {
      $additional_cols[] = '';
    }

    $row = parent::overviewTableRow($conditions, $id, $raci, $additional_cols);

    return $row;
  }
}
