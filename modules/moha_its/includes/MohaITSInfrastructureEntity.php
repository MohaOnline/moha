<?php
/**
 * @file
 */

class MohaITSInfrastructureEntity extends Entity {

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => MOHA_ITS__URL__INFRASTRUCTURE_ADMIN_UI . '/manage/' . $this->identifier());
  }

  /**
   * @return string
   */
  protected function defaultLabel() {
    if (isset($this->is_new) && $this->is_new) {
      return '';
    }
    else {
      $label = parent::defaultLabel();

      if (isset($this->tid)){
        $term_name = moha_term_name_by_tid($this->tid);

        $label = $label . ' (' . $term_name . ')';
      }

      return $label;
    }
  }
}
