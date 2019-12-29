<?php
/**
 * @file
 * The Admin UI definition file.
 */

class MohaWeChatAccountEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  function hook_menu() {
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items = parent::hook_menu();

    $items[$this->path . '/manage/' . $wildcard]['type'] = MENU_LOCAL_TASK;

    // Menu form, a special case for the edit form.
    $items[$this->path . '/manage/' . $wildcard . '/menu'] = array(
      'title' => 'WeChat Menu',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('_moha_wx_menu_form', 5),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType),
      'file path' => MOHA_WX__PATH,
      'file' => 'inc/moha_wx.admin.inc',
      'type' => MENU_LOCAL_TASK,
    );

    return $items;
  }

  /**
   * {@inheritdoc}
   *
   * Provides filter form for moha_its_group on admin UI page.
   */
  public function overviewForm($form, &$form_state) {

    $form['filter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filter'),
      '#weight' => -99999,
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $filter = &moha_form_set_inline_container($form['filter']['container']);

    moha_attach_css($form, MOHA_WX__PATH_ADMIN_CSS);

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
    $additional_header[] = t('Name');
    $additional_header[] = t('WeChat Service Endpoints');
    $additional_header[] = t('Type');
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

    $human_name = check_plain($entity->human_name);
    $name = check_plain($entity->name);
    $additional_cols[] = "$human_name ($name)";

    static $endpoint_tpl = <<<ENDPOINT
<div class="form-group row">
  <label for="" class="col-sm-3 control-label">Callback</label>
  <div class="col-sm-9">
    <input type="text" class="form-control input-sm" value="@callback"></div>
</div>

<div class="form-group row">
  <label for="" class="col-sm-3 control-label">Login</label>
  <div class="col-sm-9">
    <input type="text" class="form-control input-sm" value="@login"></div>
</div>
ENDPOINT;
    $additional_cols[] = format_string($endpoint_tpl, array(
      '@callback' => moha_url("moha/wx/server/$name"),
      '@login' => moha_url("moha/wx/oauth2/login/$name"),
    ));

    $additional_cols[] = MOHA_WX__ACCOUNT_TYPE[$entity->type];
    $additional_cols[] = MOHA__STATUS__ENTITY[$entity->status];

    // Order updated and created time.
    $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    return $row;
  }
}
