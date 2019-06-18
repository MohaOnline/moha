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
  if (isset($node) && $node->type == 'webform') {
    $headers[] = '#';
    $headers[] = t('Type');
    $headers[] = t('Question');
    $headers[] = 'Weight';
    $headers[] = t('Operations');

    $components = $node->webform['components'];
    foreach ($components as $component ) {
      $row = array();
      $row[] = $component['cid'];
      $row[] = $component['type'];
      $row[] = $component['name'] ;
      $row[] = $component['weight'] ;
      // Operations.
      $row[] = '' ;
      $rows[] = $row;
    }

    if (count($rows) == 0 ) {
      $rows[] = 'No question in survey, please click Add question button to add one.';
    }

    $form['components'] = array(
      '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 1, 'class' => array()))),
    );

  }
  else {
    $form['not_webform'] = array(
      '#markup' => 'Specified node does not exist or not webform.',
    );
  }
  return $form;
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


