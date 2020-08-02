/**
 * @file
 *   Customized Ajax effect behaviors.
 */
/* jshint esversion: 6 */
/* jshint unused:false, strict: false */
/* globals Drupal:object, jQuery:object, weui:object */

(function ($) {
  Drupal.behaviors.mohaUIAjax = {
    attach: function (context, settings) {

      /* When ajax.js is not loaded. */
      if (Drupal.ajax === undefined) { return; }

      /**
       * Prepare the Ajax request before it is sent.
       * Replace ajax progress with weui.loading.
       *
       * Replace Drupal.ajax.prototype.beforeSend in ajax.js.
       *
       * @see ajax.js
       */
      Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options) {
        Drupal.behaviors.mohaUIAjax.weUILoading = weui.loading(Drupal.t('Processing...'));

        // For forms without file inputs, the jQuery Form plugin serializes the
        // form values, and then calls jQuery's $.ajax() function, which
        // invokes this handler. In this circumstance, options.extraData is
        // never used. For forms with file inputs, the jQuery Form plugin uses
        // the browser's normal form submission mechanism, but captures the
        // response in a hidden IFRAME. In this circumstance, it calls this
        // handler first, and then appends hidden fields to the form to submit
        // the values in options.extraData. There is no simple way to know
        // which submission mechanism will be used, so we add to extraData
        // regardless, and allow it to be ignored in the former case.
        if (this.form) {
          options.extraData = options.extraData || {};

          // Let the server know when the IFRAME submission mechanism is used.
          // The server can use this information to wrap the JSON response in a
          // TEXTAREA, as per http://jquery.malsup.com/form/#file-upload.
          options.extraData.ajax_iframe_upload = '1';

          // The triggering element is about to be disabled (see below), but if
          // it contains a value (e.g., a checkbox, textfield, select, etc.),
          // ensure that value is included in the submission. As per above,
          // submissions that use $.ajax() are already serialized prior to the
          // element being disabled, so this is only needed for IFRAME
          // submissions.
          var v = $.fieldValue(this.element);
          if (v !== null) {
            options.extraData[this.element.name] = Drupal.checkPlain(v);
          }
        }

        // Disable the element that received the change to prevent user
        // interface interaction while the Ajax request is in progress.
        // ajax.ajaxing prevents the element from triggering a new request, but
        // does not prevent the user from changing its value.
        $(this.element).addClass('progress-disabled').attr('disabled', true);
      };

      /**
       * Handler for the form redirection completion.
       */
      Drupal.ajax.prototype.success = function (response, status) {

        $(this.element).removeClass('progress-disabled').removeAttr('disabled');

        Drupal.freezeHeight();

        for (var i in response) {
          if (response.hasOwnProperty(i) && response[i]['command'] && this.commands[response[i]['command']]) {
            this.commands[response[i]['command']](this, response[i], status);
          }
        }

        // Reattach behaviors, if they were detached in beforeSerialize(). The
        // attachBehaviors() called on the new content from processing the
        // response commands is not sufficient, because behaviors from the
        // entire form need to be reattached.
        if (this.form) {
          var settings = this.settings || Drupal.settings;
          Drupal.attachBehaviors(this.form, settings);
        }

        Drupal.unfreezeHeight();

        // Remove any response-specific settings so they don't get used on the
        // next call by mistake.
        this.settings = null;

        Drupal.behaviors.mohaUIAjax.weUILoading.hide();
      };

      /**
       * Handler for the form redirection error.
       */
      Drupal.ajax.prototype.error = function (xmlhttprequest, uri, customMessage) {
        Drupal.displayAjaxError(Drupal.ajaxError(xmlhttprequest, uri, customMessage));

        // Undo hide.
        $(this.wrapper).show();
        // Re-enable the element.
        $(this.element).removeClass('progress-disabled').removeAttr('disabled');
        // Reattach behaviors, if they were detached in beforeSerialize().
        if (this.form) {
          var settings = this.settings || Drupal.settings;
          Drupal.attachBehaviors(this.form, settings);
        }

        Drupal.behaviors.mohaUIAjax.weUILoading.hide();
      };

    // mohaUIAjax attached.
    }
  };

})(jQuery);
