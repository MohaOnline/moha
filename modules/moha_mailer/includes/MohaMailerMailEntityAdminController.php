<?php
/**
 * @file
 */

class MohaMailerMailEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_its_group on admin UI page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mail Filter'),
      '#weight' => -99999,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $filter = &moha_form_set_inline_container($form['filter']['container']);

    $filter['mail_to'] = array(
      '#title' => t('Mail to'),
      '#type' => 'textfield',
      '#description' => t("Filter by recipient's mail address"),
      '#required' => FALSE,
      '#default_value' => isset($_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO]) ? $_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO] : '',
    );

    $form['filter']['actions'] = array(
      '#type' => 'actions',
    );

    moha_form_actions($form['filter'], array('Filter', 'Reset Filter'));

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

    switch ($op_key) {
      case 'Filter':
        $_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO] = isset($values['mail_to']) ? trim($values['mail_to']) : '';
        break;
      case 'Reset Filter':
        $_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO] = '';
        break;
      default:
        break;
    }
  }

  /**
   * Renders whole management page independently.
   *
   * @inheritdoc
   */
  public function overviewTable($conditions = []) {

    if (!empty($_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO])) {
      $conditions['mail_to'] = $_SESSION[MOHA_MAILER_MAIL__ADMIN_UI_FILTER__TO];
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    $query->propertyOrderBy('updated','DESC');

    // Add all conditions to query.
    foreach ($conditions as $key => $condition) {
      if (is_array($condition)) {
        $query->propertyCondition($key, $condition['value'], $condition['operator']);
      }
      else {
        $query->propertyCondition($key, $condition);
      }
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
      '#empty' => t('No data.'),
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
    $additional_header[] = t('Subject');
    $additional_header[] = t('From');
    $additional_header[] = t('To');
    $additional_header[] = t('Cc');
    $additional_header[] = t('Node');
    $additional_header[] = t('User');
    $additional_header[] = t('Status');
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
    $additional_cols[] = $entity->subject;
    $additional_cols[] = check_plain($entity->mail_from);
    $additional_cols[] = check_plain($entity->mail_to);
    $additional_cols[] = check_plain($entity->mail_cc);
    $node = node_load($entity->nid);
    $additional_cols[] = l($node->title, 'node/'. $node->nid);
    if ($entity->eid_type == 'user') {
      $user = user_load($entity->eid);
      $additional_cols[] = l($user->name, 'user/' . $user->uid);
    }
    else {
      $additional_cols[] = check_plain($entity->eid) . ':' . $entity->eid;
    }
    $additional_cols[] = MOHA__STATUS__ENTITY[$entity->status];

    // Order updated and created time.
    $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    return $row;
  }
}
