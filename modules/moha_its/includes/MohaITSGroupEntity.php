<?php
/**
 * @file
 */

class MohaITSGroupEntity extends Entity {

  /**
   * Human name
   */
  public $human_name;

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => 'admin/moha/its/groups/manage/' . $this->identifier());
  }

  /**
   * @return string
   */
  protected function defaultLabel() {
    if (isset($this->is_new) && $this->is_new) {
      return '';
    }
    else {
      return $this->human_name;
    }
  }
}
