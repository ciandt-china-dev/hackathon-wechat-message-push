<?php
/**
 * @file
 * Hook menu callback.
 */

/**
 * Request wechat info callback.
 */
function _wechat_request_info_url() {
  //$url = "https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=id&corpsecret=secrect";
  //$json = file_get_contents($url);
  $json = '{"user_id":"eleven"}';
  $array = json_decode($json, TRUE);
  $name = $array['user_id'];
  $data = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('name', $name, '=')
    ->execute()
    ->fetchAssoc();

  if (empty($data)) {
    $user = array(
      'name' => $name,
      'status' => 1,
    );
    user_save($account, $user);
    //drupal_goto();
  }
  else{
    return $name;
    //drupal_goto();
  }
}
