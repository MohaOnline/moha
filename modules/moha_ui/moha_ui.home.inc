<?php
/**
 * @file
 * The homepage control file from the Moha UI module, Moha Online package.
 */

/**
 * Homepage.
 *
 * @return array
 *   Page element array.
 */
function moha_ui_homepage() {

  $view = views_get_view('moha_clip_list');
  $view->override_url = $_GET['q'];
  $element['moha_clip_list'] = $view->preview('default');

  // Include Moha Block Service Conversion.
  if (module_exists('moha_block')) {
    $block = module_invoke(__MOHA_BLOCK, 'block_view', MOHA_BLOCK__SERVICE_CONVERSION);
    if (!empty($block['content'])) {
      $element['blocks'][] = render($block['content']);
    }
  }

  $element['#theme'] = array('moha_ui__tpl__homepage_contents');

  $element['#attached'] = array(
    'css' => array(),
    'js' => array(),
    'library' => array(
      array(__MOHA_UI, __MOHA_UI),
    ),
  );

  return $element;
}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_ui_homepage_form_validate($form, &$form_state) {
  $values = $form_state['values'];

}

/**
 *
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 *
 * @param $form_state
 *   A keyed array containing the current state of the form. The arguments
 *   that drupal_get_form() was originally called with are available in the
 *   array $form_state['build_info']['args'].
 */
function moha_ui_homepage_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  drupal_set_message(t('Configurations have been saved.'));
}