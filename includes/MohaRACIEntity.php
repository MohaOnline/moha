<?php
/**
 * @file
 */

class MohaRACIEntity extends Entity {

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => MOHA__URL__RACI_ADMIN_UI . '/manage/' . $this->identifier());
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
