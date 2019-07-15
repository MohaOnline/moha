<?php
/**
 * @file
 */

class MohaMailerMailEntity extends Entity {

  /**
   * @var integer
   */
  public $nid;

  /**
   * @var integer
   */
  public $eid;

  /**
   * Entity type.
   *
   * @var string
   */
  public $eid_type;

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => MOHA_MAILER__URL__MAIL_ADMIN_UI . '/manage/' . $this->identifier());
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
