<div id="moha-message-wall-say-thank-you">
  <?php if (empty($contents['messages'])): ?>
    <p>Still empty message wall.</p>
  <?php endif; ?>
</div>

<?php if (!empty($contents['messages'])): ?>
  <link rel="stylesheet" href="<?php echo MOHA__PATH;?>/css/alertify.css">
  <script src="<?php echo MOHA__PATH;?>/js/alertify.js"></script>

  <script>
    (function ($) {
      Drupal.behaviors.moha_message = {
        attach: function (context, settings) {
          // Add collapsible effect for image and video field.
          $('#moha-message-wall-say-thank-you', context).once('alertify-js', function () {

            let messages = <?php echo json_encode($contents['messages'])?>;
            const messages_count = messages.length;
            const messages_init_count = 20;

            let current_message_index = 0;

            alertify.maxLogItems(messages_init_count);

            /**
             * Push and fetch message from message pool.
             */
            function show_message() {
              alertify.delay(0).log(messages[current_message_index++].message, function(ev) {
                // The click event is in the event variable, so you can use it here.
                ev.preventDefault();
              });

              if (current_message_index >= messages_count) {
                current_message_index = 0;
              }
            }

            for (let i = 0; i < messages_init_count; i++){
              show_message();
            }

            setInterval(show_message, 3000);

          });
        }
      };

    })(jQuery);
  </script>
<?php endif; ?>