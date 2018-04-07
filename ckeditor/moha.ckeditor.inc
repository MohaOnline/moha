<?php
/**
 * @file
 * @Created: 23/02/2018 3:38 AM
 */

/**
 * Hook to register the CKEditor plugin - it would appear in the plugins list on the profile setting page.
 */
function moha_ckeditor_plugin() {

  $plugins = array();

  /* Define spotlight widget to make spotlight area from template as widget and draggable. */
  $plugins['moha_spotlight'] = array(
    // Name of the plugin used to write it.
    'name' => 'moha_spotlight',
    // Description of the plugin - it would be displayed in the plugins management section of profile settings.
    'desc' => t('Moha customized widgets.'),
    // The full URL to the CKEditor plugins directory, with the trailing slash.
    'path' => MOHA__PATH. '/ckeditor/moha_spotlight/',
    // All toolbar buttons need be declared here.
    'buttons' => array(
      'moha_spotlight' => array(
        // Path to the button icon. Relative to the plugin directory 'path' unless 'icon_path' is specified.
        'icon' => 'moha_spotlight.png',
        // Optional full path to the directory the icon is located in.
        'icon_path' => MOHA__PATH. '/ckeditor/moha_spotlight/icons/',
        'label' => 'Moha Spotlight Widget',
      ),
    )
  );

  /* Define spotlight widget to make spotlight area from template as widget and draggable. */
  $plugins['moha_save'] = array(
    // Name of the plugin used to write it.
    'name' => 'moha_save',
    // Description of the plugin - it would be displayed in the plugins management section of profile settings.
    'desc' => t('Moha customized saving function.'),
    // The full URL to the CKEditor plugins directory, with the trailing slash.
    'path' => MOHA__PATH. '/ckeditor/moha_save/',
    // All toolbar buttons need be declared here.
    'buttons' => array(
      'moha_saveiconsicons' => array(
        // Path to the button icon. Relative to the plugin directory 'path' unless 'icon_path' is specified.
        'icon' => 'moha_save.png',
        // Optional full path to the directory the icon is located in.
        'icon_path' => MOHA__PATH. '/ckeditor/moha_save/icons/',
        'label' => 'Moha Save',
      ),
    )
  );

  return $plugins;

}
