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
        '#markup' => $component['type'],
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
          'href' => 'admin/moha/survey/component/',
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
  $form['actions']['question'] = array(
    '#type' => 'submit',
    '#value' => t('New Question'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $preview_url = url('node/' . $node->nid);

  $form['actions']['preview'] = array(
    '#markup' => "<a class='button' href='$preview_url' target='_blank'>Preview</a>" ,
  );

  return $form;
}


/**
 * Moha Survey save function.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @throws
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
      $node->is_new_revision = TRUE;

      foreach ($weights as $cid => $weight) {
        $node->webform['components'][$cid]['weight'] = $weight['weight'];
      }

      node_save($node);
      drupal_set_message(t('Your survey has been saved.'));

      break;
    case $values['question']:
      break;
    case $values['delete']:
      break;
    default:
      break;
  }

  return;
}

/**
 * Moha Survey component edit form.
 *
 * @param $form
 *   The form array of the overview form.
 * @param $form_state
 *   The overview form state which will be used for submitting.
 *
 * @see drupal_build_form
 */
function moha_survey_component_edit_form($form, &$form_state, $node, $cid) {

  return $form;
}


