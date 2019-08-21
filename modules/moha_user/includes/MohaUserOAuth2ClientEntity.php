<?php
/**
 * @file
 * The entity definition file.
 */

class MohaUserOAuth2ClientEntity extends Entity {

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => MOHA_USER__URL__OAUTH2_CLIENT_ADMIN_UI . '/manage/' . $this->identifier());
  }

  /**
   * @return string
   */
  protected function defaultLabel() {
    if (isset($this->is_new) && $this->is_new) {
      return '';
    }
    else {
      return parent::defaultLabel();
    }
  }
}
