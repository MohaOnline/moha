<?php
/**
 * @file
 */

class MohaCommerceOrderEntityAdminController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   *
   * Add additional column headers to entity admin list page.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = []) {

    $additional_header[] = t('SSO');
    $additional_header[] = t('Name');
    $additional_header[] = t('BU');
    $additional_header[] = t('Product');
    $additional_header[] = t('Status');
    $additional_header[] = t('Updated');
    $additional_header[] = t('Created');

    $header = $additional_header;

    // Replace label with ID.
    array_unshift($header, t('ID'));

    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }

    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount());

    return $header;
  }

  /**
   * {@inheritdoc}
   *
   * Add additional columns to entity admin list page.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = []) {

    // SSO
    $owner = user_load($entity->oid);
    $additional_cols[] = $owner->name;  // SSO.

    // SAML name, and BU
    $profile = profile2_load_by_user($entity->oid, __MOHA_SAML);
    if ($profile) {
      $wrapper = entity_metadata_wrapper('profile2', $profile);
      $additional_cols[] = $wrapper->moha_saml_formal_name->value();
      $additional_cols[] = $wrapper->moha_saml_business_unit->value();
    }
    else {
      $additional_cols[] = '';
      $additional_cols[] = '';
    }

    // Product name.
    $product = node_load($entity->nid);
    $additional_cols[] = $product->title;

    $additional_cols[] = moha_commerce_order_status_array()[$entity->status];

    // Order updated and created time.
    $additional_cols[] = format_date($entity->updated, 'short');
    $additional_cols[] = format_date($entity->created, 'short');

    return parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
  }
}
