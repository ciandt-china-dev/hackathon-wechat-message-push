<?php
/**
 * @file
 * Enbrel admin from.
 */

/**
 * wechat get access_token
 */
function wechat_get_access_token($app_id) {
  $wechat_app = entity_load('wechat_app', array($app_id));
  $wechat_app = reset($wechat_app);

  if (isset($wechat_app->access_token)) {
    if ($wechat_app->create_time + $wechat_app->expire - 30 > time()) {
      return $wechat_app->access_token;
    }
  }
  
  $we_obj = _wechat_init_obj($app_id);
  
  $access_token = $we_obj->checkAuth($wechat_app->app_id, $wechat_app->app_secret);

  if (!empty($access_token)) {
    $wechat_app->access_token = $access_token;
    $wechat_app->expire = 3600;
    $wechat_app->create_time = time();
    
    entity_save('wechat_app', $wechat_app);
  }
  else {
    // todo error
  }
  
  return $access_token;
}

/**
 * init wechat object
 */
function _wechat_init_obj($app_id) {
  $we_obj = &drupal_static(__FUNCTION__);
  $wechat_app = entity_load('wechat_app', array($app_id));
  $wechat_app = reset($wechat_app);
  // todo check for existance
  if (!isset($we_obj)) {
    module_load_include('php', 'wechat_push', 'libs/sdk/qywechat.class');
    $options = array(
      'token' => $wechat_app->token,
      'appid' => $wechat_app->app_id,
      'appsecret' => $wechat_app->app_secret,
    );
    $we_obj = new Wechat($options);
  }
  return $we_obj;
}

function wechat_set_message($app_id, $touser, $content = NULL) {
  $we_obj = _wechat_init_obj($app_id);
  $access_token = wechat_get_access_token($app_id);
  $we_obj->checkAuth('', '', $access_token);
  
  $toparty = '';
  $totag = '';
  $agentid = 8;
  $content = isset($content) ? $content : 'this is test';

  $data = array(
    'touser' => $touser,
    'toparty' => $toparty,
    'totag' => $totag,
    'msgtype' => 'text',
    'agentid' => $agentid,
    'text' => array(
      'content' => $content,
    ),
    'safe' => 0,
  );
  watchdog('wechat msg data', '<pre>' . var_export($data, TRUE) . '</pre>');
  $result = $we_obj->sendMessage($data);
  watchdog('wechat msg result', '<pre>' . var_export($result, TRUE) . '</pre>');
}

function wechat_callback($app_id){
  module_load_include('php', 'wechat_push', 'libs/php/WXBizMsgCrypt');

  header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
  header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past
  
  $wechat_app = entity_load('wechat_app', array($app_id));
  $wechat_app = reset($wechat_app);

  watchdog('calendar', '<pre>' . var_export($wechat_app, TRUE) . '</pre>');
  // todo check for sanity

  $sVerifyMsgSig = urldecode($_GET['msg_signature']);
  $sVerifyTimeStamp = urldecode($_GET['timestamp']);
  $sVerifyNonce = urldecode($_GET['nonce']);

  $wxcpt = new WXBizMsgCrypt($wechat_app->token, $wechat_app->encoding_aes_key, $wechat_app->app_id);

  if (isset($_GET['echostr'])) {
    $sVerifyEchoStr = urldecode($_GET['echostr']);
    watchdog('calendar', '<pre>' . var_export($sVerifyEchoStr, TRUE) . '</pre>');

    // 需要返回的明文
    $sEchoStr = "";

    $errCode = $wxcpt->VerifyURL($sVerifyMsgSig, $sVerifyTimeStamp, $sVerifyNonce, $sVerifyEchoStr, $sEchoStr);
    if ($errCode == 0) {
      // 验证URL成功，将sEchoStr返回
      watchdog('calendar', '<pre>' . var_export($sEchoStr, TRUE) . '</pre>');
      print $sEchoStr;
      die();
    } else {
      watchdog('calendar', '<pre>' . var_export($sEchoStr, TRUE) . '</pre>');
      watchdog('calendar', '<pre>' . var_export($errCode, TRUE) . '</pre>');
      watchdog('calendar', '<pre>' . var_export($_GET, TRUE) . '</pre>');
      print("ERR: " . $errCode . "\n\n");
    }
  }
}
