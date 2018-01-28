<?php
/**
 * @file
 * @Created: 07/07/2017 2:07 AM
 */

function _moha_ca_monitor_pluralsight() {
  $url_prefix = MOHA_CA__PLURALSIGHT_DATA_FEED_URL_PREFIX;

  $moha_ca_params = variable_get(MOHA_CA__VARIABLES, []);

  $moha_ca_params['pluralsight_courses_last_page'] = isset($moha_ca_params['pluralsight_courses_last_page'])?$moha_ca_params['pluralsight_courses_last_page']:1;

  try {
    $i = $moha_ca_params['pluralsight_courses_last_page'];

    do {
      $url = $url_prefix . $i;
      $data = moha_get_feed($url);

      $id = db_insert('pluralsight')
        ->fields(array(
          'url' => $url,
          'feed' => $data,
          'page' => $i,
        ))
        ->execute();

      watchdog(
        MOHA_CA__HUMAN_NAME,
        __FUNCTION__ . " R %i, id %id",
        [
          '%i' => $i,
          '%id' => $id,
        ],
        WATCHDOG_INFO
      );

      $moha_ca_params['pluralsight_courses_last_page'] = $i = $i + 1;
      variable_set(MOHA_CA__VARIABLES, $moha_ca_params);

      sleep(MOHA_CA___FEED_FETCHING_INTERVAL);

      if (!isset($json)) {
        $json = json_decode($data);

        if (!isset($json)) throw new Exception(__FUNCTION__ . "Invalid JSON");
      }
    }
    while ($i <= $json->general->pageTotal);

    // If finish to fetch all feeds, let next fetch from first page.
    $moha_ca_params['pluralsight_courses_last_page'] = 1;
    variable_set(MOHA_CA__VARIABLES, $moha_ca_params);
  }
  catch (Exception $e) {
    watchdog(
      MOHA_CA__HUMAN_NAME,
      __FUNCTION__ . " Exception: %exception",
      [
        '%exception' => $e->getMessage(),
      ],
      WATCHDOG_ERROR
    );
  }

}

function _moha_ca_process_pluralsight() {
  watchdog(
    MOHA_CA__HUMAN_NAME,
    __FUNCTION__ . " Start"
  );

  db_truncate('pluralsight_courses')->execute();
  db_truncate('pluralsight_keywords')->execute();

  /* Unprocessed records count
  $query = db_select('pluralsight')->condition('proceeded', FALSE);
  $query->addExpression('COUNT(pluralsight.id)', 'count');
  $count = $query->execute()->fetchField();
  */

  $query = db_select('pluralsight');
  $query->condition('processed', FALSE);
  $query->fields('pluralsight', array('feed'));
  $query->orderBy('updated', 'DESC');
  $results = $query->execute();

  $i = 1;
  while ($record = $results->fetchAssoc()){
    $json = json_decode($record['feed']);

    watchdog(
      MOHA_CA__HUMAN_NAME,
      __FUNCTION__ . " processing R %i",
      [
        '%i' => $i++,
      ],
      WATCHDOG_INFO
    );

    // If json feed is invalid, re-fetch it.
    if (!$json){
      watchdog(
        MOHA_CA__HUMAN_NAME,
        __FUNCTION__ . " invalid json feed",
        [
        ],
        WATCHDOG_ERROR
      );

      $moha_ca_params = variable_get(MOHA_CA__VARIABLES, []);
      $moha_ca_params['pluralsight_courses_refetch'][] = $i - 1;
      variable_set(MOHA_CA__VARIABLES, $moha_ca_params);

      continue;
    }

    foreach ($json->resultsets[0]->results as $course){
      $rate = isset($course->ratingCount)?$course->ratingCount:0;

      $id = db_insert('pluralsight_courses')
        ->fields(array(
          'title' => $course->title,
          'description' => $course->description,
          'rate' => $rate,
        ))
        ->execute();

      if ($rate == 0) continue;

      $keywords = explode(' ', $course->title);

      foreach ($keywords as $keyword){

        if (_moha_ca_meanless($keyword)) continue;

        $query = db_select('pluralsight_keywords')->fields('pluralsight_keywords', array('rate'));
        $query->condition('keyword', $keyword);
        $keyword_rate = $count = $query->execute()->fetchField();
        if ($keyword_rate) {
          $query = db_update('pluralsight_keywords');
          $query->fields(array('rate' => $keyword_rate+$rate));
          $query->condition('keyword', $keyword);
          $query->execute();
        }
        else {
          $kid = db_insert('pluralsight_keywords')
            ->fields(array(
              'keyword' => $keyword,
              'rate' => $rate,
            ))
            ->execute();
        }
      }
    }
  }

  watchdog(
    MOHA_CA__HUMAN_NAME,
    __FUNCTION__ . " End"
  );
}

function _moha_ca_meanless($keyword){
  $meanless_keywords = [
    "in",
    "the",
    "and",
    "width",
    "to",
    "for",
    "-",
    "a",
    "an",
  ];

  $keyword = drupal_strtolower($keyword);

  if(in_array($keyword, $meanless_keywords) || is_numeric($keyword)) return TRUE;
  else return FALSE;
}