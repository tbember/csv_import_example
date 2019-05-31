# CSV Import Example
CSV import example for Drupal 7 using Drush + Batch API.

## Installation
```
drush en csv_import_example
```

## Usage example
Copy `example.csv` to /tmp and then run:
```
drush csv-import-example temporary://example.csv
```

Additionally, you can use `size` option to set the batch size, e.g. `--size=5`.
