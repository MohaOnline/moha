<?php
/**
 * @file
 */

class MohaITSOperationEntity extends Entity {

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => 'admin/moha/its/operations/manage/' . $this->identifier());
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
