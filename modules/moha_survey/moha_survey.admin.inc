<?php
/**
 * @file
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
          'href' => 'admin/moha/survey/component/',
          'query' => array('destination' => 'admin/moha/survey'),
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
  $values = $form_state['values'];

  $nid = $values['nid'];
  $node = node_load($nid);

  $weights = $values['components'];

  switch ($values['op']) {
    case $values['save']:
      if (empty($node)) {
        return;
      }

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

  if (empty($node)) {
    watchdog(__FUNCTION__, 'node does not exist.', array(
    ), WATCHDOG_WARNING);

    drupal_goto('admin/moha/survey');
  }

  if ($cid>0 && isset($node->webform['components'][$cid])) {
    $component = $node->webform['components'][$cid];
    $component['_type'] = moha_survey_question_type($component);
  }
  else {
    $component['name'] = '';
    $component['form_key'] = ctools_uuid_generate();
    $component['required'] = TRUE;
    $component['weight'] = 999;
    $component['_type'] = 'rating';
    $component['extra']['max_score'] = 5;
    $component['extra']['threshold_score'] = 3;
  }

  $form['type'] = array(
    '#title' => t('Question type'),
    '#type' => 'radios',
    '#default_value' => $component['_type'],
    '#options' => array(
      'rating' => t('Rating'),
      'radio' => t('Radio box'),
      'check' => t('Check box'),
    ),
  );

  $form['question'] = array(
    '#title' => t('Question'),
    '#type' => 'textarea',
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
    '#options' => array(
      'foo' => 'foo',
      'bar' => 'bar',
      'baz' => 'baz',
    ),
    '#key_type' => 'associative',
    '#default_value_allowed' => FALSE,
  );

  $form['actions'] = array('#type' => 'actions');
  if ($cid > 0) {
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }
  else {
    $form['actions']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add')
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
 */
function moha_survey_component_edit_form_submit($form, &$form_state) {

  return $form;
}


