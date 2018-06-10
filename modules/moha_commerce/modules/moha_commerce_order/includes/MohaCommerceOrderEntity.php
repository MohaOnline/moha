<?php
/**
 * @file
 */

class MohaCommerceOrderEntity extends Entity {

  /**
   * @return array
   */
  protected function defaultUri() {
    return array('path' => 'admin/moha/commerce/orders/manage/' . $this->identifier());
  }

  /**
   * @return string
   */
  protected function defaultLabel() {
    return $this->identifier();
  }
}
