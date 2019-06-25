<?php
/**
 * @file
 *   Survey management page, component, webform component, question are same
 * meaning in this file.
 */

/**
 * Moha Survey config form.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 *
 * @throws Exception
 */
function moha_survey_config_form($form, &$form_state) {

  $form['uid'] = array(
    '#title' => t('User ID'),
    '#description' => t('Specify uid who is assumed to preview survey. 0 represents anonymous.'),
    '#size' => '20',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => moha_array_variable_get(MOHA_SURVEY__VARIABLES, MOHA_SURVEY__VARIABLE__PREVIEW_UID, 0),
    '#attributes' => array(
      'class' => array(),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration')
  );

  return $form;
}

/**
 * Moha Survey config submit handling.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 *
 * @throws Exception
 */
function moha_survey_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  moha_array_variable_set(MOHA_SURVEY__VARIABLES, MOHA_SURVEY__VARIABLE__PREVIEW_UID, $values['uid']);
}

/**
 * Survey dashboard entry.
 *
 * @return mixed
 *   render array
 */
function moha_survey_dashboard() {

  // We can't use views_embed_view() as we have exposed filter, and we need to
  // make sure the path is overriden to the current URL.
  // @see http://drupal.org/node/525592#comment-1968604
  $view = views_get_view('moha_survey_template', 'survey_dashboard_templates');
  // $view->set_arguments(array($group_type, $gid));
  $view->override_url = $_GET['q'];
  $content['templates'] = $view->preview();

  $view = views_get_view('moha_survey_list', 'survey_dashboard_lists');
  $view->override_url = $_GET['q'];
  $content['surveys'] = $view->preview();

  $content['#theme'] = array('moha_survey_dashboard'); /** @see moha_survey_dashboard.tpl.php */
  $content['#attached'] = array(
    'css' => array(
      MOHA_SURVEY__PATH . '/css/moha_survey.admin.css',
    ),
    'js' => array(
    ),
  );

  if (defined('__MOHA_UI')) {
    $content['#attached']['library'][] = array(__MOHA_UI, __MOHA_UI);
    $content['#attached']['library'][] = array(__MOHA_UI, __MOHA_UI_ADMIN);
  }

  return $content;
}

/**
 * Moha Survey edit form.
 *
 * @param $node
 *   A Webform node.
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 *
 * @throws Exception
 */
function moha_survey_edit_form($form, &$form_state, $node) {
  if (!isset($node) || $node->type != 'webform') {
    $form['invalid'] = array(
      '#markup' => t('Invalid survey.'),
    );
  }
  else {

    // Survey title.
    $form['title'] = array(
      '#title' => t('Survey name'),
      '#description' => t('Specify survey name.'),
      '#size' => '60',
      '#type' => 'textfield',
      '#required' => TRUE,
      '#default_value' => $node->title,
      '#attributes' => array(
        'class' => array(),
      ),
    );

    $form['nid'] = array(
      '#type' => 'hidden',
      '#default_value' => $node->nid,
    );

    $form['components'] = array(
      '#tree' => TRUE,
      '#theme' => 'moha_admin_form_2_draggable_table',
    );

    $components = $node->webform['components'];
    foreach ($components as $component) {
      $index = $component['cid'];
      $form['components'][$index]['Question'] = array(
        'name' => array(
          '#markup' => $component['name'],
        ),
      );

      $form['components'][$index]['type'] = array(
        '#markup' => moha_survey_question_type($component),
      );

      $form['components'][$index]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 255,
        '#default_value' => $component['weight'],
        '#attributes' => array('class' => array('row-weight')),
      );

      $operations = array(
        array(
          'title' => t('Configure'),
          'href' => 'admin/moha/survey/component/' . $node->nid . '/' . $index,
          'query' => array('destination' => 'admin/moha/survey/edit/' . $node->nid),
        ),
        array(
          'title' => t('Delete'),
          'href' => 'admin/moha/survey/component-delete/' . $node->nid . '/' . $index,
          'query' => array('destination' => 'admin/moha/survey/edit/' . $node->nid),
        ),
      );
      $operation_html ='';
      foreach ($operations as $operation) {
        $operation_html .= l($operation['title'], $operation['href'], array('query'=>$operation['query'], 'attributes' => array('class' => array('button'))));
      }

      $form['components'][$index]['operations'] = array(
        '#markup' => $operation_html,
//        '#theme' => 'links__ctools_dropbutton',
//        '#links' => $operations,
      );
    }
  }

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $advanced = &$form['advanced'];

  $advanced['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep survey active.'),
    '#description' => 'Uncheck to close survey and stop collecting data.',
    '#default_value' => $node->webform['status'],
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', __MOHA_SURVEY__ENTITY__TEMPLATE)
    ->propertyCondition('nid', $node->nid, '=')
    ->propertyCondition('cid', 0, '=')
    ->propertyCondition('status', moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY), '=');

  $result = $query->execute();

  if (!empty($result[__MOHA_SURVEY__ENTITY__TEMPLATE])) {
    $template = 1;
  }
  else {
    $template = 0;
  }

  $advanced['template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pin this survey as template.'),
    '#description' => 'Check to mark this survey as template.',
    '#default_value' => $template,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Survey')
  );

  $component_url = url('admin/moha/survey/component/' . $node->nid . '/0', array('query' => array('destination' => 'admin/moha/survey/edit/' . $node->nid)));
  $form['actions']['component'] = array(
    '#markup' => "<a class='button' href='$component_url'>" . t('Add Question') . "</a>" ,
  );

  $delete_url = url('admin/moha/survey/delete/' . $node->nid);
  $form['actions']['delete'] = array(
    '#markup' => "<a class='button' href='$delete_url'>" . t('Delete') . "</a>" ,
  );

  $preview_url = l(t('Preview'), 'node/' . $node->nid, array(
      'query' => array(
        'preview' => moha_array_variable_get(MOHA_SURVEY__VARIABLES, MOHA_SURVEY__VARIABLE__PREVIEW_UID, 0),
      ),

      'attributes' => array(
        'class' => array('button'),
        'target' => '_blank',
      )
    )
  );
  $form['actions']['preview'] = array(
    '#markup' => $preview_url,
  );

  return $form;
}

/**
 * Moha Survey edit form submission handling.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @throws Exception
 */
function moha_survey_edit_form_submit($form, &$form_state) {

  if (empty($form_state['values'] || empty($form_state['values']['nid']))) {
    throw new Exception('No submit values or invalid nid.');
  }

  $values = $form_state['values'];
  $node = node_load($values['nid']);

  if (empty($node)) {
    throw new Exception('Invalid nid.');
  }

  switch ($values['op']) {
    case $values['save']:

      // It's to save order of questions.
      $weights = $values['components'];

      $node->title = $values['title'];
      $node->webform['status'] = $values['status'];
      $node->revision = TRUE;
      $node->is_current = TRUE;

      foreach ($weights as $cid => $weight) {
        $node->webform['components'][$cid]['weight'] = $weight['weight'];
      }

      node_save($node);
      drupal_set_message(t('Your survey has been saved.'));

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', __MOHA_SURVEY__ENTITY__TEMPLATE)
        ->propertyCondition('nid', $values['nid'], '=')
        ->propertyCondition('cid', 0, '=');

      $result = $query->execute();

      global $user;

      if (empty($result[__MOHA_SURVEY__ENTITY__TEMPLATE])) {
        if ($values['template']) {
          $entity['nid'] = $values['nid'];
          $entity['oid'] = $user->uid;
          $entity['uid'] = $user->uid;
          $entity['status'] = moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY);
          $entity['referred'] = 0;
          $entity['name'] = $values['title'];
          $entity['cid'] = 0;

          $entity = entity_create(__MOHA_SURVEY__ENTITY__TEMPLATE, $entity);
          if ($entity) {
            /** @var $entity MohaSurveyTemplateEntity */
            $entity->save();
          }
        }
      }
      else { // If record exists.
        $result = array_shift($result[__MOHA_SURVEY__ENTITY__TEMPLATE]);
        $entity = entity_load_single(__MOHA_SURVEY__ENTITY__TEMPLATE, $result->id);

        if ($entity->status != moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY) && $values['template']) {
          $entity->status = moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY);

          $entity->is_new_revision = TRUE;
          /** @var $entity MohaSurveyTemplateEntity */
          $entity->save();
        }
        elseif ($entity->status == moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY) && !$values['template']) {
          $entity->status = moha_array_key_by_value(MOHA__TERM__DISABLED, MOHA__STATUS__ENTITY);

          $entity->is_new_revision = TRUE;
          /** @var $entity MohaSurveyTemplateEntity */
          $entity->save();
        }
      }

      break;
    default:
      break;
  }

  return;
}

/**
 * Moha Survey delete confirm page.
 *
 * @param $node
 *   A Webform node.
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 */
function moha_survey_delete_form($form, &$form_state, $node) {

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form = confirm_form($form,
    t('Are you sure to delete this survey?'),
    'admin/moha/survey/edit/' . $node->nid,
    format_string('Survey: !survey will be deleted, this action cannot be undone.',
      array(
        '!survey' => l($node->title, 'node/' . $node->nid,
          array(
            'attributes' => array('target' => '_blank')
          )
        )
      )
    ),
    t('Delete'),
    t('Cancel'),
    $node->title
  );

  return $form;
}

/**
 * Moha Survey delete confirm page submission handling.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 */
function moha_survey_delete_form_submit($form, &$form_state) {
  try {
    $values = $form_state['values'];
    $nid = $values['nid'];

    $node = node_load($nid);
    $node->status = 0;
    node_save($node);

    drupal_set_message(t('Survey has been removed.'));
    watchdog(__FUNCTION__, 'nid: %nid is unpublished.', array(
      '%nid' => $values['nid'],
    ), WATCHDOG_INFO);

    // Template disable.

    drupal_goto('admin/moha/survey/');
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }
}

/**
 * Moha Survey component edit form.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 * @param $node
 *   A Webform node.
 * @param $cid
 *   Component ID in webform_component table from 1.
 *
 * @return mixed
 *
 * @throws Exception
 *
 * @see drupal_build_form
 */
function moha_survey_component_edit_form($form, &$form_state, $node, $cid = 0) {

  if (empty($node) || $node->type != 'webform') {
    watchdog(__FUNCTION__, 'node does not exist, or node is not webform.', array(
    ), WATCHDOG_WARNING);

    drupal_goto('admin/moha/survey');
  }

  if ($cid>0 && isset($node->webform['components'][$cid])) {
    $component = $node->webform['components'][$cid];
    $component['_type'] = moha_survey_question_type($component, FALSE);

    if (!isset($component['extra']['max_score'])) {
      $component['extra']['max_score'] = 5;
    }

    if (!isset($component['extra']['threshold_score'])) {
      $component['extra']['threshold_score'] = 3;
    }

    if (isset($component['extra']['items'])){
      module_load_include('inc', 'webform', 'components/select');
      $component['extra']['_options'] = _webform_select_options_from_text($component['extra']['items'], FALSE);
    }
    else {
      $component['extra']['_options'] = array();
    }
  }
  else {
    $weight = 0;
    if (count($node->webform['components'])) {
      foreach($node->webform['components'] as $component) {
        if ($weight < $component['weight']) {
          $weight = $component['weight'];
        }
      }
    }
    $component['name'] = '';
    $component['form_key'] = str_replace('-', '_', ctools_uuid_generate());
    $component['required'] = TRUE;
    $component['weight'] = ++$weight;
    $component['_type'] = 'rating';
    $component['extra']['max_score'] = 5;
    $component['extra']['threshold_score'] = 3;
    $component['extra']['nps'] = 3;
    $component['extra']['_options'] = array();
  }

  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['weight'] = array(
    '#type' => 'hidden',
    '#value' => $component['weight'],
  );

  $form['form_key'] = array(
    '#type' => 'hidden',
    '#value' => $component['form_key'],
  );

  $form['type'] = array(
    '#title' => t('Question type'),
    '#type' => 'radios',
    '#default_value' => $component['_type'],
    '#options' => array(
      'rating' => t('Rating'),
      'radio' => t('Radio box'),
      'check' => t('Check box'),
      'textfield' => t('Text field'),
    ),
  );

  $form['name'] = array(
    '#title' => t('Question'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#default_value' => $component['name'],
  );

  $form['required'] = array(
    '#title' => t('Required'),
    '#type' => 'checkbox',
    '#default_value' => $component['required'],
    '#description' => t('Check when visitor must answer this question.'),
  );

  $form['rating'] = array(
    '#title' => t('Rating Options'),
    '#type' => 'fieldset',
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => 'rating'),
      ),
    ),
  );

  $rating = &$form['rating'];

  module_load_include('inc', __MOHA_UI, 'webform/components/moha_ui_rating');
  $form_items = _webform_edit_moha_ui_rating($component, array(), array());
  foreach (element_children($form_items['extra']) as $key) {
    $rating[$key] = $form_items['extra'][$key];
  }

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array(
          array('value' => 'radio'),
          array('value' => 'check'),
        ),
      ),
    ),
  );

  $options = &$form['options'];
  $options['options'] = array(
    '#type' => 'options',
    '#multiple' => FALSE,
    '#options' => $component['extra']['_options'],
    '#key_type' => 'mixed',
    '#default_value_allowed' => FALSE,
    '#states' => array(
      'required' => array(
        ':input[name="type"]' => array(
          array('value' => 'radio'),
          array('value' => 'check'),
        ),
      ),
    ),
  );

  $options['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $advanced = &$options['advanced'];

  $advanced['template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pin these options as template for reuse.'),
    '#default_value' => 0,
  );

  $advanced['template_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Options template name',
    '#states' => array(
      'required' => array(
        ':input[name="is_enterprise"]' => array('checked' => TRUE),
      ),
    ),
  );

  hide($advanced);

  $form['actions'] = array('#type' => 'actions');
  if ($cid > 0) {
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save Question')
    );

    $delete_url = url('admin/moha/survey/component-delete/' . $node->nid . '/' . $cid);
    $form['actions']['delete'] = array(
      '#markup' => "<a class='button' href='$delete_url'>" . t('Delete') . "</a>" ,
    );
  }
  else {
    $form['actions']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Question')
    );
  }

  $form['#attached'] = array(
    'css' => array(
      MOHA_SURVEY__PATH . '/css/moha_survey.admin.css',
    ),
    'js' => array(
      MOHA_UI__PATH. '/webform/components/moha_ui_rating.js'
    ),
  );

  return $form;
}

/**
 * Moha Survey component update handling.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @throws Exception
 */
function moha_survey_component_edit_form_submit($form, &$form_state) {

  if (empty($form_state['values'] || empty($form_state['values']['nid']))) {
    throw new Exception('No submit values or invalid nid.');
  }

  $values = $form_state['values'];
  $node = node_load($values['nid']);

  if (empty($node)) {
    throw new Exception('Invalid nid.');
  }

  // Build webform component.
  $component['name'] = $values['name'];
  $component['form_key'] = $values['form_key'];
  $component['required'] = $values['required'];
  $component['weight'] = $values['weight'];
  $component['pid'] = 0;

  if (isset($values['options']['options'])) {
    foreach ($values['options']['options'] as $key => $value) {
      if (is_numeric($key)) {
        unset($values['options']['options'][$key]);
        $values['options']['options'][$value] = $value;
      }
    }
  }

  switch($values['type']) {
    default:
      throw new Exception('Invalid question type.');
      break;
    case 'rating':
      $component['type'] = 'moha_ui_rating';
      $component['extra']['max_score'] = $values['max_score'];
      $component['extra']['threshold_score'] = $values['threshold_score'];
      break;
    case 'radio':
      $component['type'] = 'select';
      $component['extra']['multiple'] = FALSE;
      $component['extra']['aslist'] = FALSE;
      module_load_include('inc', 'webform', 'components/select');
      $component['extra']['items'] = _webform_select_options_to_text($values['options']['options']);
      break;
    case 'check':
      $component['type'] = 'select';
      $component['extra']['multiple'] = TRUE;
      $component['extra']['aslist'] = FALSE;
      module_load_include('inc', 'webform', 'components/select');
      $component['extra']['items'] = _webform_select_options_to_text($values['options']['options']);
      break;
    case 'textfield':
      $component['type'] = 'textfield';
      break;
  }

  if (!isset($values['save'])) {
    $values['save'] = 'save';
  }

  switch ($values['op']) {
    case $values['save']:

      $node->revision = TRUE;
      $node->is_current = TRUE;

      $component['cid'] = $values['cid'];
      $node->webform['components'][$values['cid']] = $component;

      node_save($node);
      drupal_set_message(t('Your question has been saved.'));
      break;

    case $values['add']:

      $node->revision = TRUE;
      $node->is_current = TRUE;

      $node->webform['components'][] = $component;

      node_save($node);
      drupal_set_message(t('Your question has been added.'));
      break;
    default:
      break;
  }

  return;
}

/**
 * Moha Survey delete confirm page.
 *
 * @param $node
 *   A Webform node.
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 */
function moha_survey_component_delete_form($form, &$form_state, $node, $cid) {

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $cid,
  );

  $form = confirm_form($form,
    t('Are you sure to delete this survey?'),
    'admin/moha/survey/component/' . $node->nid . '/' . $cid,
    format_string('Question: !question will be deleted, this action cannot be undone.',
      array(
        '!question' => $node->webform['components'][$cid]['name'],
      )
    ),
    t('Delete'),
    t('Cancel'),
    $node->title
  );

  return $form;
}

/**
 * Moha Survey component delete confirm page submission handling.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 */
function moha_survey_component_delete_form_submit($form, &$form_state) {
  try {
    $values = $form_state['values'];
    $nid = $values['nid'];

    $node = node_load($nid);
    unset($node->webform['components'][$values['cid']]);

    $node->revision = TRUE;
    $node->is_current = TRUE;
    node_save($node);

    drupal_set_message(t('Question has been removed.'));
    watchdog(__FUNCTION__, 'nid: %nid, cid: %cid is removed.', array(
      '%nid' => $values['nid'],
      '%cid' => $values['cid'],
    ), WATCHDOG_INFO);

    drupal_goto('admin/moha/survey/edit/' . $nid);
  }
  catch (Exception $e) {
    watchdog_exception(__FUNCTION__, $e);
  }
}

/**
 * Create a table containing all submitted values for a survey.
 */
function moha_survey_results_submissions($node, $pager_count = 0) {
  module_load_include('inc', 'webform', 'includes/webform.report');
  return webform_results_table($node, $pager_count);
}

/**
 * Menu callback; Download an exported file.
 */
function moha_survey_results_download_callback($node){
  module_load_include('inc', 'webform', 'includes/webform.report');
  return webform_results_download_callback($node);
}

/**
 * Form to configure the download of CSV files.
 */
function moha_survey_results_download_form($form, &$form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.report');
  $form = webform_results_download_form($form, $form_state, $node);
  $form['#submit'][] = 'webform_results_download_form_submit';

  return $form;
}

/**
 * Survey templates clone form.
 */
function moha_survey_clone_form($form, &$form_state, $node = NULL) {

  // Prompt user clone depends on node clone module.
  if (!module_exists('clone')) {
    $node_clone_link = l('Node Clone', 'https://www.drupal.org/project/node_clone', array(
      'attributes' => array(
        'target' => '_blank',
      )
    ));

    $enable_link = l(t('enable'), 'admin/modules', array(
      'query' => array(
        'filter' => 'clone',
      ),
      'attributes' => array(
        'target' => '_blank',
      )
    ));

    drupal_set_message(t("Survey Clone feature needs $node_clone_link module, please install and $enable_link it."), 'error');
  }

  if (empty($node)) {
    $node = new stdClass();
    $node->nid = '';
  }

  $form = array();

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Survey name'),
    '#description' => t('Please provides survey name which you could use to identify different survey.'),
    '#size' => 80,
    '#default_value' => '',
    '#required' => TRUE,
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', __MOHA_SURVEY__ENTITY__TEMPLATE)
    ->propertyCondition('cid', 0, '=')
    ->propertyCondition('status', moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY), '=');

  $result = $query->execute();

  if (!empty($result[__MOHA_SURVEY__ENTITY__TEMPLATE])) {
    foreach ($result[__MOHA_SURVEY__ENTITY__TEMPLATE] as $template) {
      $template = entity_load_single(__MOHA_SURVEY__ENTITY__TEMPLATE, $template->id);
      $options[$template->nid] = $template->name;
    }
  }
  else {
    $options = array();
  }

  $form['template'] = array(
    '#type' => 'select',
    '#title' => t('Template'),
    '#options' => $options,
    '#empty_value' => 0,
    '#default_value' => $node->nid,
    '#description' => t('Select template you want to copy.'),
  );

  $form['actions'] = array('#type' => 'actions');

  if (empty($node->title)) {
    $form['actions']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Survey')
    );
  }
  else {
    $form['actions']['clone'] = array(
      '#type' => 'submit',
      '#value' => t('Clone Survey')
    );
  }

  return $form;
}

/**
 * Survey templates clone submit handling.
 */
function moha_survey_clone_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  if ($values['template'] == 0) {

    global $user;

    //https://www.drupal.org/node/2030697  Programmatically creating a webform
    $node = new stdClass();
    $node->type = 'webform';
    node_object_prepare($node);
    $node->title = $values['title'];
    $node->language = LANGUAGE_NONE;
    $node->body[LANGUAGE_NONE][0]['value']   = '';
    $node->body[LANGUAGE_NONE][0]['format']  = 'full_html';
    $node->uid = $user->uid;
    $node->promote = 0;
    $node->comment = 0;

    // Attach the webform to the node.
    $node->webform = array(
      'confirmation' => '',
      'confirmation_format' => NULL,
      'redirect_url' => 'moha/survey/success',
      'status' => '1',
      'block' => '0',
      'teaser' => '0',
      'allow_draft' => '0',
      'auto_save' => '0',
      'submit_notice' => '1',
      'submit_text' => '',
      'submit_limit' => '1', // User can submit only once.
      'submit_interval' => '-1',
      'total_submit_limit' => '-1',
      'total_submit_interval' => '-1',
      'record_exists' => TRUE,
      'roles' => array(
        0 => '2', // Only allow login user take survey.
      ),
      'progressbar_bar' => FALSE,
      'components' => array(),
    );

    $node->revision = TRUE;
    $node->is_current = TRUE;
    // Save the node.
    node_save($node);

    $form_state['redirect'] = 'admin/moha/survey/edit/'. $node->nid;
  }
  else {
    module_load_include('inc', 'clone', 'clone.pages');
    $original_node = node_load($values['template']);
    if (isset($original_node->nid) && clone_is_permitted($original_node->type)) {
      $node = _clone_node_prepare($original_node);
      // Let other modules do special fixing up.
      $context = array(
        'method' => 'save-edit',
        'original_node' => $original_node
      );
      drupal_alter('clone_node', $node, $context);
      $node->title = $values['title'];
      $node->revision = TRUE;
      $node->is_current = TRUE;

      node_save($node);

      if (module_exists('rules')) {
        rules_invoke_event('clone_node', $node, $original_node);
      }

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', __MOHA_SURVEY__ENTITY__TEMPLATE)
        ->propertyCondition('nid', $values['template'], '=')
        ->propertyCondition('cid', 0, '=')
        ->propertyCondition('status', moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY), '=');

      $result = $query->execute();

      if (!empty($result[__MOHA_SURVEY__ENTITY__TEMPLATE])) {
        $result = array_shift($result[__MOHA_SURVEY__ENTITY__TEMPLATE]);
        $entity = entity_load_single(__MOHA_SURVEY__ENTITY__TEMPLATE, $result->id);

        $entity->referred += 1;

        /** @var $entity MohaSurveyTemplateEntity */
        $entity->save();
      }

      $form_state['redirect'] = 'admin/moha/survey/edit/'. $node->nid;
    }
  }

  return;
}

/**
 * Survey templates delete form.
 *
 * @throws Exception
 *   when provided template is in invalid.
 */
function moha_survey_template_delete_form($form, &$form_state, $template_id) {
  $form = array();

  $template = entity_load_single(__MOHA_SURVEY__ENTITY__TEMPLATE, $template_id);

  if (empty($template)) { throw new Exception('Invalid survey template id, or template has been removed thoroughly'); }

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $template_id,
  );

  $form = confirm_form($form,
    t('Are you sure to delete this template?'),
    'admin/moha/survey/',
    format_string('Template: %template_name will be deleted, this action cannot be undone.',
      array(
        '%template_name' => $template->name,
        )
      ),
    t('Delete'),
    t('Cancel')
  );

  return $form;
}


/**
 * Survey templates delete form.
 *
 * @throws Exception
 *   when provided template is in invalid.
 */
function moha_survey_template_delete_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $template_id = $values['id'];
  $entity = entity_load_single(__MOHA_SURVEY__ENTITY__TEMPLATE, $template_id);
  if (empty($entity)) { throw new Exception('Invalid survey template id, or template has been removed thoroughly'); }

  if ($entity->status == moha_array_key_by_value(MOHA__TERM__ACTIVE, MOHA__STATUS__ENTITY)) {
    $entity->status = moha_array_key_by_value(MOHA__TERM__DELETED, MOHA__STATUS__ENTITY);

    $entity->is_new_revision = TRUE;
    /** @var $entity MohaSurveyTemplateEntity */
    $entity->save();

    drupal_set_message(t('Template: @template has been deleted.', array('@template' => $entity->name)));
  }
}
