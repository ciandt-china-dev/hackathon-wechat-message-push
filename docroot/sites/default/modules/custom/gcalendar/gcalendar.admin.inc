<?php

/**
 * @file
 * Administration function for gcalendar.
 */

/**
 * Uses the form api for an admin settings form.
 */
function gcalendar_settings_form() {
  global $user;
  $gcalendar_clientid = variable_get('gcalendar_clientid', '');
  $gcalendar_secret = variable_get('gcalendar_secret', '');

  $form['sitewide']['gcalendar_clientid'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => $gcalendar_clientid,
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t('The site wide google client id.'),
  );

  $form['sitewide']['gcalendar_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#default_value' => $gcalendar_secret,
    '#description' => t('The site wide google client secret.'),
  );

  $form = system_settings_form($form);
  return $form;
}

