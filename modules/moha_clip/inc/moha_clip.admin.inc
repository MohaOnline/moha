<?php
/**
 * @file
 */

function _moha_clip_manager_form($form, &$form_state) {

  drupal_add_css(drupal_get_path('module', __MOHA_CLIP) . '/css/moha_clip_admin.css', array(
    'group' => 200,
    'weight' => 999,
  ));

  $form['enable'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable'),
    '#attributes' => array('class' => array('container-inline')),
    '#collapsible' => TRUE,
  );

  $form['enable']['enable_comment_new_moha_clip'] = array(
    '#type' => 'submit',
    '#value' => t('Enable comment of new Moha Clip'),
  );

  $form['enable']['enable_comment_existing_moha_clip'] = array(
    '#type' => 'submit',
    '#value' => t('Enable comment of existing Moha Clip'),
  );

  $form['disable'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disable'),
    '#attributes' => array('class' => array('container-inline')),
    '#collapsible' => TRUE,
  );

  $form['disable']['disable_comment_moha_clip'] = array(
    '#type' => 'submit',
    '#value' => t('Disable comment for Moha Clip'),
  );

  $form['disable']['disable_promote_new_moha_clip'] = array(
    '#type' => 'submit',
    '#value' => t('Disable promotion for Moha Clip'),
  );

  return $form;
}

function _moha_clip_manager_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    default:
      break;
    case t('Disable comment for Moha Clip');
      _moha_clip_disable_moha_clip_comment();
      break;
    case t('Disable promotion for Moha Clip');
      _moha_clip_disable_moha_clip_promote();
      break;
  }

}

/**
 * Disable comment function for new Moha Clip.
 */
function _moha_clip_disable_moha_clip_comment() {

  $num_master_updated = db_update('node')
    ->fields(array('comment' => COMMENT_NODE_HIDDEN))  // Hide comment for Moha Clip.
    ->condition ('node.type', __MOHA_CLIP)
    ->execute();

  $sub_query = db_select('node');
  $sub_query->addField('node', 'nid');
  $sub_query->condition('node.type', __MOHA_CLIP, '=');

  $num_revision_updated = db_update('node_revision')
    ->fields(array('comment' => COMMENT_NODE_HIDDEN))
    ->condition ('node_revision.nid', $sub_query, 'IN')
    ->execute();

  drupal_set_message(t('All Moha Clips are not discussable now.'));
  watchdog(__MOHA_CLIP, 'All Moha Clips are not discussable now. %num_master master and %num_revision revision record(s) updated.',
    array(
      '%num_master' => $num_master_updated,
      '%num_revision' => $num_revision_updated,
    ));
}

function _moha_clip_disable_moha_clip_promote() {

  $num_updated = db_update('node')
    ->fields(array('promote' => FALSE))
    ->condition ('node.type', __MOHA_CLIP)
    ->execute();

  drupal_set_message(t('New Moha Clip will not be promoted.'));
  watchdog(__MOHA_CLIP, 'New Moha Clip will not be promoted. %num_updated record(s) updated.', array('%num_updated' => $num_updated));

  $sub_query = db_select('node');
  $sub_query->addField('node', 'nid');
  $sub_query->condition('node.type', __MOHA_CLIP, '=');

  $num_updated = db_update('node_revision')
    ->fields(array('promote' => '0',))
    ->condition ('node_revision.nid', $sub_query, 'IN')
    ->execute();

  drupal_set_message(t('All Moha Clip are not promoted now.'));
  watchdog(__MOHA_CLIP, 'All Moha Clip are not promoted now. %num_updated record(s) updated.', array('%num_updated' => $num_updated));
}



