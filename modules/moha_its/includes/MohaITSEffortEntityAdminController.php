<?php
/**
 * @file
 */

class MohaITSEffortEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_its_group on admin UI page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Operation Filter'),
      '#weight' => -99999,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $form['filter']['user'] = array(
      '#title' => t('Owner name'),
      '#type' => 'entityreference',
      '#description' => 'Filter by group or company representative, enter representative\'s name then choose from drop-down menu.',
      '#required' => FALSE,
      '#default_value' => isset($_SESSION[__MOHA_ITS_GROUP . '_FILTER_USER'])?$_SESSION[__MOHA_ITS_GROUP . '_FILTER_USER']:'',
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
      $_SESSION[MOHA_ITS_GROUP__ADMIN_UI_FILTER__OWNER] = '';
    }
    else if ($op_key == 'filter') {
      $_SESSION[MOHA_ITS_GROUP__ADMIN_UI_FILTER__OWNER] = isset($values['user']['entity_id'])?$values['user']['entity_id']:'';
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

    if (!empty($_SESSION[MOHA_ITS_GROUP__ADMIN_UI_FILTER__OWNER])) {
      $conditions['rid'] = $_SESSION[MOHA_ITS_GROUP__ADMIN_UI_FILTER__OWNER];
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    $query->propertyOrderBy('updated','DESC');

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
    $additional_header[] = t('Title');
    $additional_header[] = t('Company');
    $additional_header[] = t('Service');
    $additional_header[] = t('Type');
    $additional_header[] = t('Date');
    $additional_header[] = t('Duration');
    $additional_header[] = t('Owner');
    $additional_header[] = t('Updated');
    $additional_header[] = t('Created');

    $additional_header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount()+1);

    return $additional_header;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional columns to entity admin list page.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = []) {

    // From 3rd property column.
    $group = entity_load_single(__MOHA_ITS_GROUP, $entity->gid);
    $additional_cols[] = $group->human_name;

    $service = entity_load_single(__MOHA_ITS_SERVICE, $entity->sid);
    $additional_cols[] = $service->label();

    $term = taxonomy_term_load($entity->tid);
    $additional_cols[] = $term->name;

    $additional_cols[] = $entity->date;
    $additional_cols[] = $entity->duration;

    $owner = user_load($entity->oid);
    $additional_cols[] = $owner->name;

    // Order updated and created time.
    $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    // Add $id to head of $row.
    array_unshift($row, $id);
    return $row;
  }
}
