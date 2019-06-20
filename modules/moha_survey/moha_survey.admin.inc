<?php
/**
 * @file
 *   Survey management page, component, webform component, question are same
 * meaning in this file.
 */

/**
 * Survey dashboard entry.
 *
 * @return mixed
 *   render array
 */
function moha_survey_dashboard() {

  $content['#theme'] = array('moha_survey_dashboard'); /** @see moha_survey_dashboard.tpl.php */

  $content['#attached'] = array(
    'css' => array(
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
 * @throws \Exception
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

      $form['components'][$index]['operations'] = array(
        '#theme' => 'links__ctools_dropbutton',
        '#links' => $operations,
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

  $advanced['template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Treat this survey as template.'),
    '#description' => 'Check to mark this survey as template.',
    '#default_value' => 0,
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

  $preview_url = url('node/' . $node->nid);
  $form['actions']['preview'] = array(
    '#markup' => "<a class='button' href='$preview_url' target='_blank'>" . t('Preview') . "</a>" ,
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

  $render_array = confirm_form($form,
    t('Are you sure to delete this survey?'),
    'admin/moha/survey/edit/' . $node->nid,
    format_string('Survey: !survey will be deleted, This action cannot be undone.',
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

  return $render_array;
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
    '#type' => 'fieldset',
    '#title' => t('Rating'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => 'rating'),
      ),
    ),
  );

  $rating = &$form['rating'];

  $rating['max_score'] = array(
    '#title' => t('Max score'),
    '#description' => t('Specify allowed max score.'),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['max_score'],
  );

  $rating['threshold_score'] = array(
    '#title' => t('Threshold score'),
    '#description' => t('Specify a number, when rating is less or equals than the value, details need be provided. '),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['threshold_score'],
  );

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

  $render_array = confirm_form($form,
    t('Are you sure to delete this survey?'),
    'admin/moha/survey/component/' . $node->nid . '/' . $cid,
    format_string('Question: !question will be deleted, This action cannot be undone.',
      array(
        '!question' => $node->webform['components'][$cid]['name'],
      )
    ),
    t('Delete'),
    t('Cancel'),
    $node->title
  );

  return $render_array;
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