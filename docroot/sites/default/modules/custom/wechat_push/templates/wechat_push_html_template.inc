<?php
/**
 * @
 * Default template implementation for HTML Mesage.
 *
 * Available variables:
 * - $content
 */
?>

<?php if ($content) : ?>
  <div id="wechat-push-message">
  <?php print $content; ?>
  </div>
<?php endif; ?>
