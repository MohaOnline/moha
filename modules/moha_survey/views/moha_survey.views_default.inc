<?php
/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 *
 * Defines views for Moha Survey.
 */
function moha_survey_views_default_views() {

  // Moha Survey Template.
  $view = new view();
  $view->name = 'moha_survey_template';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'moha_survey_template';
  $view->human_name = 'Moha Survey Template';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Moha Survey Templates Block';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'template_name' => 'template_name',
    'name' => 'name',
    'referred' => 'referred',
  );
  $handler->display->display_options['style_options']['class'] = '';
  $handler->display->display_options['style_options']['default'] = 'referred';
  $handler->display->display_options['style_options']['info'] = array(
    'template_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-right',
      'separator' => '',
      'empty_column' => 0,
    ),
    'referred' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Moha Survey Template: Owner */
  $handler->display->display_options['relationships']['oid']['id'] = 'oid';
  $handler->display->display_options['relationships']['oid']['table'] = 'moha_survey_template';
  $handler->display->display_options['relationships']['oid']['field'] = 'oid';
  /* Field: Moha Survey Template: Name */
  $handler->display->display_options['fields']['template_name']['id'] = 'template_name';
  $handler->display->display_options['fields']['template_name']['table'] = 'moha_survey_template';
  $handler->display->display_options['fields']['template_name']['field'] = 'name';
  $handler->display->display_options['fields']['template_name']['label'] = 'Template name';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'oid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Moha Survey Template: Referred */
  $handler->display->display_options['fields']['referred']['id'] = 'referred';
  $handler->display->display_options['fields']['referred']['table'] = 'moha_survey_template';
  $handler->display->display_options['fields']['referred']['field'] = 'referred';
  /* Field: Moha Survey Template: Node ID */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'moha_survey_template';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Moha Survey Template: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'moha_survey_template';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a class="button" href="/admin/moha/survey/clone/[nid]">Clone</a>
<a class="button" href="/admin/moha/survey/template-delete/[id]?destination=admin/moha/survey">Delete</a>';
  /* Filter criterion: Moha Survey Template: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'moha_survey_template';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'survey_dashboard_templates');
  $handler->display->display_options['block_caching'] = '2';
  $translatables['moha_survey_template'] = array(
    t('Master'),
    t('Moha Survey Templates Block'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('User'),
    t('Template name'),
    t('Referred'),
    t('.'),
    t(','),
    t('Operations'),
    t('<a class="button" href="/admin/moha/survey/clone/[nid]">Clone</a>
<a class="button" href="/admin/moha/survey/template-delete/[id]?destination=admin/moha/survey">Delete</a>'),
    t('Block'),
  );




  // Combine all defined views, don't touch from here.
  $views[$view->name] = $view;

  return $views;
}
