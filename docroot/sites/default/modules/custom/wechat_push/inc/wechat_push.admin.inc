<?php
/**
 * @file
 * Enbrel admin from.
 */

/**
 * Generates the profile type editing form.
 */
function wechat_app_form($form, &$form_state, $wechat_app, $op = 'edit', $entity_type = NULL) {
  $form['label'] = array(
    '#title' => t('App Name'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->label) ? $wechat_app->label : '',
  );
  $form['app_id'] = array(
    '#title' => t('Corp ID'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->app_id) ? $wechat_app->app_id : '',
  );
  $form['app_secret'] = array(
    '#title' => t('Secret'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->app_secret) ? $wechat_app->app_secret : '',
  );
  $form['agent_id'] = array(
    '#title' => t('Agentid'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->agent_id) ? $wechat_app->agent_id : '',
  );
  $form['token'] = array(
    '#title' => t('Token'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->token) ? $wechat_app->token : '',
  );
  $form['encoding_aes_key'] = array(
    '#title' => t('EncodingAesKey'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->encoding_aes_key) ? $wechat_app->encoding_aes_key : '',
  );
  $form['access_token'] = array(
    '#title' => t('Access Token'),
    '#type' => 'textfield',
    '#default_value' => isset($wechat_app->access_token) ? $wechat_app->access_token : '',
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Wechat App'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function wechat_app_form_submit(&$form, &$form_state) {
  $wechat_app = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $wechat_app->save();
  $form_state['redirect'] = 'admin/structure/wechat';
}