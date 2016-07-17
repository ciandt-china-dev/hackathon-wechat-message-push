<?php
/**
 * @
 * Default template implementation for TEXT message.
 *
 * Available variables:
 * - $event
 */
watchdog('calendar changed events', '444<pre>' . var_export($event, TRUE) . '</pre>');
$start_datetime = new DateTime($event->getStart()->getDateTime());
$end_datetime = new DateTime($event->getEnd()->getDateTime());
$start_day = $start_datetime->format('Y-m-d');
$end_day = $end_datetime->format('Y-m-d');
if ($start_day == $end_day) {
  $time_str = $start_day . ' ' . $start_datetime->format('H:i') . ' - ' . $end_datetime->format('H:i');
} 
else {
  $time_str = $start_datetime->format('Y-m-d H:i') . ' -- ' . $end_datetime->format('Y-m-d H:i');
}
?>
<?php print $event->getCreator()->getDisplayName() ?> has invited you to meeting "<?php print $event->getSummary() ?>" @ <?php print $time_str ?>, see <a href="<?php print $event->getHtmlLink() ?>">more detail</a>, or join the <a href="<?php print $event->getHangoutLink() ?>">Hangout</a>.
