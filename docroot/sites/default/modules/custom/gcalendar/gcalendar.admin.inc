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
  $gcalendar_refresh_token = variable_get('gcalendar_refresh_token', '');

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

  $refresh_token_label = t('Refresh Token');
  $form['sitewide']['gcalendar_refresh_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh Token'),
    '#description' => t('To generate a <a href="@google_url">refresh token</a> (which is necessary for secure communication), configure client ID and secret, then press "@refresh_token_label" below', array('@google_url' => 'https://developers.google.com/identity/protocols/OAuth2WebServer#offline', '@refresh_token_label' => $refresh_token_label)),
    '#default_value' => $gcalendar_refresh_token,
  );

  $form = system_settings_form($form);
  $form ['actions']['refresh'] = array(
    '#type' => 'submit',
    '#value' => $refresh_token_label,
    '#submit' => array(
      'system_settings_form_submit',
      'gcalendar_settings_form_refresh_token_submit'
    ),
  );
  return $form;
}

/**
 * Refresh the token.
 */
function gcalendar_settings_form_refresh_token_submit($form, &$form_state) {
  $results = gcalendar_get_google_token(array('set_token' => TRUE, 'destination' => 'admin/config/content/gcalendar'));
  if (!empty($results['redirect'])) {
    $form_state['redirect'] = array(
      $results['redirect'],
      array(
        'external' => TRUE,
      ),
    );
  }
}
