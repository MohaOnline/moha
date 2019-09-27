<?php
/**
 * @file
 * The entity definition file.
 */

class MohaUserOAuth2ClientEntity extends Entity {

  /**
   * @var integer
   *   Identifier of the entity.
   */
  public $id;
  
  /**
   * @var string
   *   Internal machine name.
   */
  public $name;

  /**
   * @var integer
   *   Entity status @see \MOHA__STATUS__ENTITY.
   */
  public $status;

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
