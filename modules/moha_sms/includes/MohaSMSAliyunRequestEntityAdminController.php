<?php
/**
 * @file
 */

class MohaSMSAliyunRequestEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_its_group on admin UI page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Request Filter'),
      '#weight' => -99999,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $form['filter']['phone'] = array(
      '#title' => t('Phone#'),
      '#type' => 'textfield',
      '#description' => 'Enter phone number.',
      '#required' => FALSE,
      '#default_value' => !empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE])?$_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE]:'',
    );

    $form['filter']['receipt_no'] = array(
      '#title' => t('Receipt#'),
      '#type' => 'textfield',
      '#description' => 'Enter express receipt number.',
      '#default_value' => !empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO])?$_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO]:'',
      '#required' => FALSE,
    );

    if (!empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE]) || !empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO])) {
      $form['filter']['#collapsed'] = FALSE;
    }

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
      $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE] = '';
      $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO] = '';
    }
    else if ($op_key == 'filter') {
      $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE] = isset($values['phone'])?$values['phone']:'';
      $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO] = isset($values['receipt_no'])?$values['receipt_no']:'';
    }
  }

  /**
   * {@inheritdoc}
   *
   *  Renders whole management page independently.
   */
  public function overviewTable($conditions = []) {

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    $query->propertyOrderBy('updated','DESC');

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if (!empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE])) {
      $query->propertyCondition('phone', $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__PHONE], 'CONTAINS');
    }

    if (!empty($_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO])) {
      $query->propertyCondition('params', $_SESSION[MOHA_SMS_ALIYUN_REQUEST__ADMIN_UI_FILTER__RECEIPT_NO], 'CONTAINS');
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

    // $additional_header[] = t('ID');
    // $additional_header[] = t('Updated');
    $additional_header[] = t('Created');
    $additional_header[] = t('Phone');
    $additional_header[] = t('Params');
    // $additional_header[] = t('Template');
    $additional_header[] = t('Code');
    $additional_header[] = t('Message');
    $additional_header[] = t('RequestId');
    $additional_header[] = t('BizId');

    // $additional_header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount()+1);

    return $additional_header;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional columns to entity admin list page.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = []) {

    // Order updated and created time.
    // $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    $additional_cols[] = $entity->phone;
    $additional_cols[] = $entity->params;
    // $additional_cols[] = $entity->template;
    $additional_cols[] = $entity->Code;
    $additional_cols[] = $entity->Message;
    $additional_cols[] = $entity->RequestId;
    $additional_cols[] = $entity->BizId;

    return $additional_cols;

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    return $row;
  }
}
