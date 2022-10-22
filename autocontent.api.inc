<?php
/**	
 * @file 
 * API definitions
 */

/**
 * Implements hook_autocontent_intervals_alter().
 * 
 * @param (array) $intervals
 *   Array of potential interval options.
 */
function hook_autocontent_intervals_alter($intervals) {
  $intervals['new'] = t('New Interval');
}

/**
 * Implements hook_autocontent_frequencies_alter().
 * 
 * @param (array) $frequencies
 *   Array of potential frequency options.
 */
function hook_autocontent_frequencies_alter($frequencies) {
  $frequencies['new'] = t('New Frequency');
}