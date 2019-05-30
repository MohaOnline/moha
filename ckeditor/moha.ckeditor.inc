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
    'path' => __MOHA__PATH. '/ckeditor/moha_spotlight/',
    // All toolbar buttons need be declared here.
    'buttons' => array(
      'moha_spotlight' => array(
        // Path to the button icon. Relative to the plugin directory 'path' unless 'icon_path' is specified.
        'icon' => 'moha_spotlight.png',
        // Optional full path to the directory the icon is located in.
        'icon_path' => __MOHA__PATH. '/ckeditor/moha_spotlight/icons/',
        'label' => 'Moha Spotlight Widget',
      ),
    )
  );

  /* Define widget to make widget block from template as widget and draggable. */
  $plugins['moha_widget'] = array(
    // Name of the plugin used to write it.
    'name' => 'moha_widget',
    // Description of the plugin - it would be displayed in the plugins management section of profile settings.
    'desc' => t('Moha common widgets.'),
    // The full URL to the CKEditor plugins directory, with the trailing slash.
    'path' => __MOHA__PATH. '/ckeditor/moha_widget/',
    // All toolbar buttons need be declared here.
    'buttons' => array(
      'moha_widget' => array(
        // Path to the button icon. Relative to the plugin directory 'path' unless 'icon_path' is specified.
        'icon' => 'moha_widget.png',
        // Optional full path to the directory the icon is located in.
        'icon_path' => __MOHA__PATH. '/ckeditor/moha_widget/icons/',
        'label' => 'Moha Widget',
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
    'path' => __MOHA__PATH. '/ckeditor/moha_save/',
    // All toolbar buttons need be declared here.
    'buttons' => array(
      'moha_saveiconsicons' => array(
        // Path to the button icon. Relative to the plugin directory 'path' unless 'icon_path' is specified.
        'icon' => 'moha_save.png',
        // Optional full path to the directory the icon is located in.
        'icon_path' => __MOHA__PATH. '/ckeditor/moha_save/icons/',
        'label' => 'Moha Save',
      ),
    )
  );

  return $plugins;

}

/**
 * Handles files uploaded by CKEditor.
 *
 * @return int
 */
function moha_ajax_upload_by_ckeditor() {
  $validators = array(
    'file_validate_extensions' => array("jpg png gif jpeg")
  );

  $result = array();

  $ckeditor_destination = moha_array_variable_get(MOHA__VARIABLES, 'ckeditor_destination', 'public');
  $ckeditor_directory = moha_array_variable_get(MOHA__VARIABLES, 'ckeditor_directory', 'upload');
  $location = $ckeditor_destination . '://moha/ckeditor/' . $ckeditor_directory;

  try {
    $dir = file_prepare_directory($location, FILE_CREATE_DIRECTORY);

    if (!$dir) {
      $error_msg = format_string(t('%location can not be created or written.'), array('%location' => $location));
      throw new Exception($error_msg);
    }

    if (!empty($_FILES['upload']['size'])) {
      $_FILES['files']['name']['upload'] = ctools_uuid_generate() . '-' . transliteration_clean_filename($_FILES['upload']['name']);
      $_FILES['files']['tmp_name']['upload'] = $_FILES['upload']['tmp_name'];
      $_FILES['files']['size']['upload'] = $_FILES['upload']['size'];
      $_FILES['files']['error']['upload'] = $_FILES['upload']['error'];
    }
    else {
      $error_msg = t('File is not valid.');
      throw new Exception($error_msg);
    }

    $file = file_save_upload('upload', $validators, $location);

    if ($file) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, __MOHA, 'file', $file->fid);

      $url = file_create_url($file->uri);
      $result['url'] = $url;
      $result['fileName'] = $file->filename;
      $result['uploaded'] = 1;
    }
    else {
      $errors = error_get_last();
      $error_msg = json_encode($errors, MOHA__JSON_ENCODING_OPTIONS_BIT_MASK);
      throw new Exception($error_msg);
    }
  }
  // Exposed to public need handles all errors.
  catch (Exception $e) {
    $error_msg = $e->getMessage();
    watchdog(__FUNCTION__, $error_msg, NULL, WATCHDOG_ERROR);

    $result['uploaded'] = 0;
    $result['error']['message'] = $error_msg;
  }

  echo json_encode($result);

  return MENU_SITE_ONLINE;
}
