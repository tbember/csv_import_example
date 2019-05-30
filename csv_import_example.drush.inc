<?php

function csv_import_example_drush_command() {
  return array(
    'csv-import-example' => array(
      'description' => 'CSV import example',
      'options' => array(
        'size' => array(
          'description' => 'The number of rows per batch.',
          'example-value' => '50',
        ),
      ),
    ),
  );
}

function drush_csv_import_example($csv_path) {
  if (!file_exists($csv_path)) {
    return drush_set_error(dt('CSV file does not exist.'));
  }

  $rows = array_map('str_getcsv', file($csv_path));
  if (empty($rows)) {
    return drush_set_error(dt('Invalid or empty CSV file.'));
  }

  // Use that if your CSV has a header.
  array_shift($rows);

  $batch_size = drush_get_option('size', 10);
  $total = count($rows);
  $progress = 0;
  $operations = array();

  foreach (array_chunk($rows, $batch_size) as $chunk) {
    $progress += count($chunk);
    $operations[] = array('_drush_csv_import_example_rows_batch', array(
      $chunk,
      dt('@percent% (Processing @progress of @total)', array(
        '@percent' => round(100 * $progress / $total),
        '@progress' => $progress,
        '@total' => $total,
      ))),
    );
  }

  $batch = array(
    'operations' => $operations,
    'title' => dt('Process CSV rows'),
    'finished' => '_drush_csv_import_example_finished',
    'progress_message' => dt('@current rows of @total were processed.'),
  );

  batch_set($batch);
  drush_backend_batch_process();
}

function _drush_csv_import_example_rows_batch($rows, $details, &$context) {
  $context['message'] = $details;

  if (!isset($context['results']['success'])) {
    $context['results']['success'] = $context['results']['error'] = 0;
  }

  foreach ($rows as $row) {
    $key = _drush_import_csv_row($row) ? 'success' : 'error';
    $context['results'][$key]++;
  }
}

function _drush_import_csv_row($row) {
  // Do stuff. Returns TRUE when success, FALSE otherwise.
  print_r($row);
  return TRUE;
}

function _drush_csv_import_example_finished($success, $results, $operations) {
  if ($success) {
    drush_log(dt('@succeeded rows imported. @errored rows failed.', array(
      '@succeeded' => empty($results['success']) ? 0 : $results['success'],
      '@errored' => empty($results['error']) ? 0 : $results['error'],
    )), 'ok');
  }
}
