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
      'token' => 'kB2E1lbm8uMPRgQo9CreV',
      'appid' => variable_get('wechat_appid', ""),
      'appsecret' => variable_get('wechat_appsecret', ""),
    );
    $we_obj = new Wechat($options);
  }
  return $we_obj;
}

function wechat_set_message($account = NULL, $content = NULL) {
  $touser = 'weynhamz';
  $toparty = '';
  $totag = '';
  $agentid = 8;
  $content = isset($content) ? $content : 'this is test';

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
  watchdog('wechat msg data', '<pre>' . var_export($data, TRUE) . '</pre>');
  $result = $we_obj->sendMessage($data);
  watchdog('wechat msg result', '<pre>' . var_export($result, TRUE) . '</pre>');
}

/*function wechat_callback() {
  $msg_signature = $_GET['msg_signature'];
  $timestamp = $_GET['timestamp'];
  $nonce = $_GET['nonce'];
  $echostr = $_GET['echostr'];
  $token = variable_get('token', "");
  $encodingAesKey = variable_get('encodingAesKey', "");
  $corpId = variable_get('wechat_appid', "");
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
  $result = $pc->decrypt($echostr, $corpId);
  if ($result[0] != 0) {
    return $result[0];
  }
  $sReplyEchoStr = $result[1];
  echo $sReplyEchoStr;
  exit();
}*/


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

/*function wechat_callback(){
  watchdog('wechat', '111');
  module_load_include('php', 'wechat_push', 'php/WXBizMsgCrypt');
  $encodingAesKey = variable_get('encodingAesKey', "");
  $token = variable_get('token', "");
  $corpId = variable_get('wechat_appid', "");
  $sVerifyMsgSig = $_GET['msg_signature'];
// $sVerifyTimeStamp = HttpUtils.ParseUrl("timestamp");
  $sVerifyTimeStamp = $_GET['timestamp'];
// $sVerifyNonce = HttpUtils.ParseUrl("nonce");
  $sVerifyNonce = $_GET['nonce'];
// $sVerifyEchoStr = HttpUtils.ParseUrl("echostr");
  $sVerifyEchoStr = $_GET['echostr'];

// 需要返回的明文
  $sEchoStr = "";

  $wxcpt = new WXBizMsgCrypt($token, $encodingAesKey, $corpId);
  $errCode = $wxcpt->VerifyURL($sVerifyMsgSig, $sVerifyTimeStamp, $sVerifyNonce, $sVerifyEchoStr, $sEchoStr);
  if ($errCode == 0) {
    echo $sEchoStr;
    // 验证URL成功，将sEchoStr返回
    // HttpUtils.SetResponce($sEchoStr);
  } else {
    print("ERR: " . $errCode . "\n\n");
  }
}*/


function wechat_callback(){
  module_load_include('php', 'wechat_push', 'php/WXBizMsgCrypt');

// 假设企业号在公众平台上设置的参数如下
  $encodingAesKey = "qTnwQRGmD4gocAkfUtQ26VrTw9oljlI5oX8msKdIoYv";
  $token = "kB2E1lbm8uMPRgQo9CreV";
  $corpId = "wx2ef8799c2a79ecae";


  /*
  ------------使用示例一：验证回调URL---------------
  *企业开启回调模式时，企业号会向验证url发送一个get请求 
  假设点击验证时，企业收到类似请求：
  * GET /cgi-bin/wxpush?msg_signature=5c45ff5e21c57e6ad56bac8758b79b1d9ac89fd3&timestamp=1409659589&nonce=263014780&echostr=P9nAzCzyDtyTWESHep1vC5X9xho%2FqYX3Zpb4yKa9SKld1DsH3Iyt3tP3zNdtp%2B4RPcs8TgAE7OaBO%2BFZXvnaqQ%3D%3D 
  * HTTP/1.1 Host: qy.weixin.qq.com
  
  接收到该请求时，企业应
  1.解析出Get请求的参数，包括消息体签名(msg_signature)，时间戳(timestamp)，随机数字串(nonce)以及公众平台推送过来的随机加密字符串(echostr),
  这一步注意作URL解码。
  2.验证消息体签名的正确性 
  3. 解密出echostr原文，将原文当作Get请求的response，返回给公众平台
  第2，3步可以用公众平台提供的库函数VerifyURL来实现。
  
  */

  $sVerifyMsgSig = urldecode($_GET['msg_signature']);
  $sVerifyTimeStamp = urldecode($_GET['timestamp']);
  $sVerifyNonce = urldecode($_GET['nonce']);
  $sVerifyEchoStr = urldecode($_GET['echostr']);

  watchdog('calendar', '<pre>' . var_export($_GET, TRUE) . '</pre>');
  
// 需要返回的明文
  $sEchoStr = "";

  $wxcpt = new WXBizMsgCrypt($token, $encodingAesKey, $corpId);
  $errCode = $wxcpt->VerifyURL($sVerifyMsgSig, $sVerifyTimeStamp, $sVerifyNonce, $sVerifyEchoStr, $sEchoStr);
  if ($errCode == 0) {
    //
    // 验证URL成功，将sEchoStr返回
    watchdog('calendar', '<pre>' . var_export($sEchoStr, TRUE) . '</pre>');
    print $sEchoStr;
    die();
  } else {
    print("ERR: " . $errCode . "\n\n");
  }
  
}
