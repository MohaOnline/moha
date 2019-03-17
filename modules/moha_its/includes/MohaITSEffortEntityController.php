<?php
/**
 * @file
 */

/**
 * Class MohaITSGroupEntityController
 *
 * CRUD support for moha_its_group entity.
 */
class MohaITSEffortEntityController extends EntityAPIController{
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

    global $user;

    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    foreach ($entities as $entity) {

      $entity->is_new_revision = TRUE;
      $entity->status = moha_array_key_by_value(MOHA__TERM__DELETED, MOHA__STATUS__ENTITY);
      $entity->updated = REQUEST_TIME;
      $entity->uid = $user->uid;

      parent::save($entity, $transaction);
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
    global $user;

    if (isset($entity->is_new)){
      $entity->created = REQUEST_TIME;
    }

    $entity->updated = REQUEST_TIME;
    $entity->is_new_revision = TRUE;
    $entity->uid = $user->uid;

    return parent::save($entity, $transaction);
  }

}
