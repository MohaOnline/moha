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

