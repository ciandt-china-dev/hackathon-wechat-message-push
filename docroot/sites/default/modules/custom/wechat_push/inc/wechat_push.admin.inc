<?php
/**
 * @file
 * Enbrel admin from.
 */

/**
 * Enbrel settings form.
 *
 * @return mixed
 *   Array.
 */
function wechat_push_setting_form() {
  $forms = array();
  $forms['wechat_appid'] = array(
    '#title' => t('CorpID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('wechat_appid', ''),
  );
  $forms['wechat_appsecret'] = array(
    '#title' => t('Secret'),
    '#type' => 'textfield',
    '#default_value' => variable_get('wechat_appsecret', ''),
  );
  $forms['token'] = array(
    '#title' => t('Token'),
    '#type' => 'textfield',
    '#default_value' => variable_get('token', ''),
  );
  $forms['encodingAesKey'] = array(
    '#title' => t('EncodingAesKey'),
    '#type' => 'textfield',
    '#default_value' => variable_get('encodingAesKey', ''),
  );
  $forms['agentid'] = array(
    '#title' => t('Agentid'),
    '#type' => 'textfield',
    '#default_value' => variable_get('agentid', ''),
  );
  $forms['auth_code'] = array(
    '#title' => t('auth_code'),
    '#type' => 'textfield',
    '#default_value' => variable_get('auth_code', ''),
  );
  return system_settings_form($forms);
}