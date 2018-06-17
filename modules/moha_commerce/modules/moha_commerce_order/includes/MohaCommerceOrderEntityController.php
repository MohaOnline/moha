<?php
/**
 * @file
 */

/**
 * Class MohaCommerceOrderEntityController
 *
 * CRUD support for moha_commerce_order entity.
 */
class MohaCommerceOrderEntityController extends EntityAPIController{
  /**
   * Builds a structured array representing the entity's content.
   * Invoked by entity_view function.
   * 
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param $entity object
   *   An entity object.
   * @param $view_mode string
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode string
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @param $content array
   *   Optionally. Allows pre-populating the built content to ease overridding
   *   this method.
   *
   * @return array
   *   The renderable array, keyed by entity name or numeric id.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);



    return $build;
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * Update entity status instead of really delete them.
   *
   * @param $ids array
   *   Entity ID array.
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   *
   * @throws \Exception
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }
    $transaction = isset($transaction) ? $transaction : db_transaction();

    try {
      $ids = array_keys($entities);


      if (isset($this->revisionTable)) {
        // Save revision.
      }

      db_update($this->entityInfo['base table'])
        ->fields(array(
          'status' => 0,
          'updated' => REQUEST_TIME,
        ))
        ->condition($this->idKey, $ids, 'IN')
        ->execute();

      // Reset the cache as soon as the changes have been applied.
      $this->resetCache($ids);

      // Ignore slave server temporarily.
      db_ignore_slave();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * Set entity saved time.
   *
   * @param $entity
   *   The entity to save.
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   *
   * @return integer
   *   SAVED_NEW | SAVED_UPDATED
   *
   * @throws \Exception
   *   In case of failures.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    if (isset($entity->is_new)){
      $entity->created = REQUEST_TIME;
    }

    $entity->updated = REQUEST_TIME;

    return parent::save($entity, $transaction);
  }

}
