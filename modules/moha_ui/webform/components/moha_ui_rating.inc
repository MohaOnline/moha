<?php
/**
 * @file
 *
 *
 */

/**
 * Implements _webform_defaults_component().
 *
 * @see _webform_defaults_component
 */
function _webform_defaults_moha_ui_rating() {
  $defaults = array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => 0,
    'extra' => array(
      /* Allowed max rating score. */
      'max_score' => 5,
      /* When rating value is less than this value, details need be provided. */
      /* Use 0 when the low score is not concerned. */
      'threshold_score' => 3,
      'nps' => FALSE,
      /* Concrete survey question, if empty then use label. */
      'question' => '',
      'analysis' => TRUE,
    ),
  );

  return $defaults;
}

/**
 * Implements _webform_edit_component().
 *
 * @inheritdoc
 * @see _webform_edit_component
 */
function _webform_edit_moha_ui_rating(array $component, array $form, array $form_state) {

  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field options'),
    '#collapsible' => true,
    '#collapsed' => false,
  );

  $form['extra']['analysis'] = array(
    '#type' => 'value',
    '#value' => isset($component['extra']['analysis'])?$component['extra']['analysis']:FALSE,
  );

  $form['extra']['nps'] = array(
    '#title' => t('NPS'),
    '#description' => t('Check if this is an NPS survey item.'),
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#default_value' => isset($component['extra']['nps'])?$component['extra']['nps']:FALSE,
  );

  $form['extra']['max_score'] = array(
    '#title' => t('Max score'),
    '#description' => t('Specify allowed max score.'),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['max_score'],
  );

  $form['extra']['threshold_score'] = array(
    '#title' => t('Threshold score'),
    '#description' => t('Specify a number, when rating is less or equals than the value, low score reason need be provided. Specify 0 if you do not care low score reason.'),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['threshold_score'],
  );

  $form['extra']['#attached'] = array(
    'js' => array(
      MOHA_UI__PATH. '/webform/components/js/moha_ui_rating.js'
    ),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 *
 * @inheritdoc
 *
 * @see _webform_render_component
 */
function _webform_render_moha_ui_rating($component, $value = NULL, $filter = TRUE, $submission = NULL) {

  $element['#title'] = $component['name'];
  $element['#type'] = 'moha_ui_rating';
  $element['#id'] = $element['#name'] = $component['form_key'];
  $element['#moha_ui_rating_max_score'] = $component['extra']['max_score'];
  $element['#moha_ui_rating_threshold_score'] = $component['extra']['threshold_score'];
  $element['#theme_wrapper'] = array('webform_element');
  $element['#weight'] = $component['weight'];
  $element['#required'] = $component['required'];

  return $element;
}

/**
 * Implements _webform_submit_component().
 *
 * @see _webform_submit_component
 */
function _webform_submit_moha_ui_rating($component, $value) {

  $value['rating'] = $value['rating']['value'];

  return $value;
}

/**
 * Implements _webform_display_component().
 *
 * @inheritdoc
 * @see _webform_display_component
 */
function _webform_display_moha_ui_rating($component, $value, $format = 'html', $submission = array()) {

}

/**
 * Implements _webform_delete_component().
 *
 * @see _webform_delete_component
 */
function _webform_delete_moha_ui_rating() {

}

/**
 * Implements _webform_help_component().
 *
 * @see _webform_help_component
 */
function _webform_help_moha_ui_rating() {

}

/**
 * Implements _webform_theme_component().
 *
 * @see _webform_theme_component.
 */
function _webform_theme_moha_ui_rating() {

}

/**
 * Implements _webform_analysis_component().
 *
 * @see _webform_analysis_component
 */
function _webform_analysis_moha_ui_rating($component, $sids = array(), $single = FALSE, $join = NULL) {
  if (!$single) {
    if ($component['extra']['nps']) {
      /** @link https://jegtnes.com/blog/calculating-a-net-promoter-score-nps-in-sqlite-postgresql-and-mysql/ @endlink */
      $query = <<<QUERY
SELECT CAST( (SUM(CASE WHEN Score BETWEEN 9 AND 10 THEN 1 ELSE 0 END) * 1.0 - SUM(CASE WHEN Score BETWEEN 0 AND 6 THEN 1 ELSE 0 END)) / COUNT(Score) * 100 AS signed ) score FROM (
	SELECT CAST(`data` as signed) Score FROM `webform_submitted_data` WHERE `nid` = :nid and `cid` = :cid and `no` = 'rating'
) as Rating;
QUERY;
      $header[] = 'NPS';
    }
    else {
      $query = <<<QUERY
SELECT CAST(AVG(`Score`) as unsigned) score FROM (
	SELECT CAST(`data` as signed) Score FROM `webform_submitted_data` WHERE `nid` = :nid and `cid` = :cid and `no` = 'rating'
) as Rating;
QUERY;
      $header[] = 'Rating';
    }

    $record = db_query($query,
      array(
        ':nid' => $component['nid'],
        ':cid' => $component['cid'],
      )
    )->fetchAssoc();

    if ($record && $record['score'] != NULL) {
      $rows[] = array($record['score']);

      return array(
        'table_header' => $header,
        'table_rows' => $rows,
      );
    }
  }
  else { // not single analysis.

  }

  return array(
    'table_header' => array(),
    'table_rows' => array(),
  );
}

/**
 * Implements _webform_table_component().
 *
 * @inheritdoc
 *
 * @see _webform_table_component
 */
function _webform_table_moha_ui_rating($component, $value) {
  if (isset($value['rating'])) {
    $return = $value['rating'] . '<br/>' . $value['details'];
  }
  else {
    $return = '-';
  }
  return $return;
}

/**
 * Implements _webform_csv_headers_component().
 *
 * @inheritdoc
 * @see _webform_csv_headers_component
 */
function _webform_csv_headers_moha_ui_rating($component, $export_options) {
  $header[0] = array('');
  $header[1] = array($component['name'], '');
  $header[2] = array('Score', 'Reason');

  return $header;
}

/**
 * Implements _webform_csv_data_component().
 *
 * @inheritdoc
 * @see _webform_csv_data_component
 */
function _webform_csv_data_moha_ui_rating($component, $export_options, $value) {
  if (isset($value['rating'])) {
    $values[] = $value['rating'];
    $values[] = $value['details'];
  }
  else {
    $values[] = '';
    $values[] = '';
  }

  return $values;
}

