<?php
/**
 * @file
 * The API file of the module.
 */

/**
 * Returns association array to indicate system to use correct function to handle OAuth2 profile.
 *
 * @return array
 */
function hook_moha_user_oauth2_post_config() {
  $config['oauth2_client_name'] = 'hook_moha_user_oauth2_post_handler';

  return $config;
}

/**
 * Example function configured in implementation of hook_moha_user_oauth2_post_config().
 *
 * @param $profile_json
 *
 * @throws Exception
 */
function hook_moha_user_oauth2_post_handler($profile_json) {
  // Fetch / Create user to login.
  $oauth2_user_name = moha_an2e($profile_json, 'name');
  $oauth2_user_mail = moha_an2e($profile_json, 'mail');
  $oauth2_user = user_load_by_mail($oauth2_user_mail);

  if (empty($oauth2_user)) {
    // Create new user.
    $password = user_password(16);
    $user_fields = array(
      'name' => $oauth2_user_name,
      'mail' => $oauth2_user_mail,
      'pass' => $password,
      'timezone' => 'Asia/Shanghai',
      'status' => 1,
      'init' => __FUNCTION__,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      ),
    );

    $oauth2_user = user_save('', $user_fields);
  }
  else if (isset($oauth2_user->status) && $oauth2_user->status == 0) {
    throw new Exception("User $oauth2_user->name($oauth2_user->uid) is blocked.");
  }

  moha_login($oauth2_user);
}