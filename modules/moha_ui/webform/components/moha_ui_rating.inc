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
      /* Concrete survey question, if empty then use label. */
      'question' => '',
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

  $form['extra']['max_score'] = array(
    '#title' => t('Max score'),
    '#description' => t('Specify allowed max score.'),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['max_score'],
  );

  $form['extra']['threshold_score'] = array(
    '#title' => t('Threshold score'),
    '#description' => t('Specify a number, when rating is less or equals than the value, details need be provided. '),
    '#size' => '10',
    '#type' => 'textfield',
    '#default_value' => $component['extra']['threshold_score'],
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

  $component_label = $component['name'];
  $form_key = $component['form_key'];
  $wrapper_class = 'moha_ui_rating_' . $form_key;

  $score = empty($value['rating']) ? 0 : $value['rating'];

  $max_score = $component['extra']['max_score'];

  $rating_items_placeholder = '';
  for ($i = 0; $i < $max_score; $i++) {
    if ($i < $score) {
      $rating_items_placeholder .= <<<RATING_ITEM
        <div class="rating-star is--active">
           <i class="fa fa-star"></i>
        </div>
RATING_ITEM;
    }
    else {
      $rating_items_placeholder .= <<<RATING_ITEM
        <div class="rating-star">
           <i class="fa fa-star"></i>
        </div>
RATING_ITEM;
    }
  }

  // Pass weight information to render function for correct element order.
  $elements['#weight'] = $component['weight'];
  $elements['#required'] = $component['required'];

  if ($elements['#required']) {
    $component_label .= ' <span class="form-required" title="This field is required.">*</span>';
  }

  $elements['rating']['rating_area'] = array(
    '#markup' => <<< RATING_CONTAINER
      
      <div class="rating-stars-container">
      $rating_items_placeholder
      </div>
RATING_CONTAINER
  ,
  );

  $elements['rating']['#prefix'] = "<label>$component_label</label><div class='rating-stars $wrapper_class'>";
  $elements['rating']['#suffix'] = "</div>";

  $elements['rating']['value'] = array(
    '#type' => 'hidden',
    '#default_value' => $score,
  );

  $details = empty($value['details'])?'':$value['details'];

  $elements['details'] = array(
    '#title' => t('Reason'),
    '#type' => 'textfield',
    '#default_value' => $details,
  );

  $threshold_score = $component['extra']['threshold_score'];

  /** @see https://www.drupal.org/docs/7/api/javascript-api/managing-javascript-in-drupal-7 */
  $elements['#attached'] = array(
    'js' => array(
      MOHA_UI__PATH . '/webform/components/moha_ui_rating.js',
      array(
        'type' => 'inline',
        'data' => <<<RATINGJS
jQuery(document).ready(function () {
  var ratingOptions = {
      selectors: {
          starsSelector: '.$wrapper_class',
          starSelector: '.$wrapper_class .rating-star',
          starActiveClass: 'is--active',
          starHoverClass: 'is--hover',
          starNoHoverClass: 'is--no-hover',
          targetFormElementSelector: 'input[name="submitted[$form_key][rating][value]"]'
      }
  };

  jQuery('.$wrapper_class').ratingStars(ratingOptions);

  // Submission editing.
  if ($score <= 0 || $threshold_score <= 0 || $score > $threshold_score) {
    jQuery('.rating-stars.$wrapper_class + .form-type-textfield').css('display', 'none');
  }
  
  if ($threshold_score > 0) {
    jQuery('.$wrapper_class').on("ratingChanged", function (ev, data) {
      if (data.ratingValue <= $threshold_score) {
        jQuery('.rating-stars.$wrapper_class + .form-type-textfield').css('display', 'inherit');
      }
      else {
        jQuery('.rating-stars.$wrapper_class + .form-type-textfield').css('display', 'none');
      }
    });
  }
  
});
RATINGJS
  ,
        'scope' => 'footer',
        'group' => JS_THEME,
        'weight' => 5,
      ),
    ),
    'css' => array(
      MOHA_UI__PATH . '/webform/components/moha_ui_rating.css',
    ),
    'library' => array(
      array(__MOHA_UI, __MOHA_UI),
    ),
  );

  $elements['#theme_wrapper'] = array('webform_element');
  $elements['#element_validate'] = array('moha_ui_rating_validate');

  return $elements;
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
function _webform_analysis_moha_ui_rating() {

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

