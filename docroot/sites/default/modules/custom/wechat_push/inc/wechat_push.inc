<?php
/**
 * @file
 * Enbrel admin from.
 */

/**
 * wechat get access_token
 */
function wechat_get_access_token() {
  $access_token = FALSE;
  if (empty($appid)) {
    $appid = variable_get('wechat_appid', "");
  }

  if (empty($appsecret)) {
    $appsecret =variable_get('wechat_appsecret', "");
  }

  $access_token_object = db_query("SELECT * FROM {wechat_access_token} WHERE app_id=:appid", array(':appid' => $appid))->fetchObject();
  $current = time();
  if (isset($access_token_object->token)) {
    $expire = $access_token_object->expire;
    $create_time = $access_token_object->create_time;
    if ($create_time + $expire - 30 > $current) {
      $access_token = $access_token_object->token;
      return $access_token;
    }
  }
  $we_obj = _wechat_init_obj();
  $access_token = $we_obj->checkAuth($appid, $appsecret);

  if (!empty($access_token)) {
    db_delete('wechat_access_token')
      ->condition('app_id', $appid)
      ->execute();

    $query = db_insert('wechat_access_token')->fields(array('app_id', 'token', 'expire', 'create_time'));
    $query->values(array(
      'app_id' => $appid,
      'token' => $access_token,
      'expire' => 3600,
      'create_time' => $current,
    ));
    $query->execute();
  }
  return $access_token;
}

/**
 * init wechat object
 */
function _wechat_init_obj() {
  $we_obj = &drupal_static(__FUNCTION__);
  if (!isset($we_obj)) {
    module_load_include('php', 'wechat_push', 'sdk/qywechat.class');
    $options = array(
      'token' => variable_get('wechat_token', ""),
      'appid' => variable_get('wechat_appid', ""),
      'appsecret' => variable_get('wechat_appsecret', ""),
    );
    $we_obj = new Wechat($options);
  }
  return $we_obj;
}

function wechat_set_message() {
  $touser = 'linwuhen';
  $toparty = '';
  $totag = '';
  $agentid = variable_get('agentid', "");
  $content = 'this is test';

  $we_obj = _wechat_init_obj();
  $access_token = wechat_get_access_token();
  $we_obj->checkAuth('', '', $access_token);
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
  $result = $we_obj->sendMessage($data);
  var_dump($result);
}

function wechat_callback() {
  $msg_signature = $_GET['msg_signature'];
  $timestamp = $_GET['timestamp'];
  $nonce = $_GET['nonce'];
  $echostr = $_GET['echostr'];
  $token = variable_get('token', "");
  $encodingAesKey = variable_get('encodingAesKey', "");
  $corpId = variable_get('wechat_appid', "");
  watchdog('wechat',$echostr);
  if (strlen($encodingAesKey) != 43) {
    return;
  }

  module_load_include('php', 'wechat_push', 'sdk/qywechat.class');
  $pc = new Prpcrypt($encodingAesKey);

  $array = getSHA1($token, $timestamp, $nonce, $echostr);
  if ($array['0'] != 0) {
    return;
  }
  $signature = $array[1];
  if ($signature != $msg_signature) {
    return;
  }
  watchdog('wachat', '222');
  $result = $pc->decrypt($echostr, $corpId);
  if ($result[0] != 0) {
    return $result[0];
  }
  watchdog('wachat', '222');
  $sReplyEchoStr = $result[1];
  watchdog('wachat', '1111');
  echo $sReplyEchoStr;
  exit();
}


/**
 * @param string $token
 * @param string $timestamp
 * @param string $nonce
 * @param string $encrypt
 */
function getSHA1($token, $timestamp, $nonce, $encrypt_msg)
{
  try {
    $array = array($encrypt_msg, $token, $timestamp, $nonce);
    sort($array, SORT_STRING);
    $str = implode($array);
    return array(0, sha1($str));
  } catch (Exception $e) {
    print $e . "\n";
    return array(1, null);
  }
}
