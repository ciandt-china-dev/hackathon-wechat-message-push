<?php

/**
*
*/
class WechatPush {

  protected $account;
  protected $message;

  /**
   * @param object / string $account
   * @param string $message
   */
  function __construct($account, $message) {

    $this->account = is_object($account) ? $account : user_load((int) $account);

    $this->message = $message;
  }

  function prepare() {
    // Prepare message via templates file.

    return $this;
  }

  function presend() {
    // Do something before sending message.

    return $this;
  }

  function send() {
    module_load_include('inc', 'wechat_push', 'inc/wechat_push');
    if (function_exists('wechat_set_message')) {
      wechat_set_message($this->account, $this->message);
    }
  }

  function push() {

    $this->prepare();

    $this->presend();

    $this->send();

    $this->aftersend();
  }

  function aftersend() {
    // Do something after sending message.

    return $this;
  }




}
