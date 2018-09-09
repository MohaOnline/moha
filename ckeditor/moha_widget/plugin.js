let moha_widget_template = '';
moha_widget_template += '<div class="moha-clip-widget">&nbsp</div>';

let allowedMohaWidgetDetails = 'table(*); thead; tbody; th; tr; td; a(*); div(*); span; img[*]; img(*); p(*)';

CKEDITOR.plugins.add('moha_widget', {
  requires: 'widget',
  icons: 'moha_widget',

  init: function (editor) {
    // Register dialog, prepare for moha_spotlight widget.
    CKEDITOR.dialog.add('moha_widget', this.path + 'dialogs/moha_widget.js');

    editor.widgets.add('moha_widget', {

      button: 'Moha Widget',
      template: moha_widget_template,
      allowedContent: allowedMohaWidgetDetails,
      dialog: 'moha_widget',

      editables: {
        details: {
          selector: '.moha-clip-widget-editable',
          allowedContent: allowedMohaWidgetDetails
        }
      },

      upcast: function (element) {
        return element.hasClass('moha-clip-widget');
      },

      // Read widget DOM status, then set to Widget data structure.
      init: function () {
      },

      // executed every time the widget data is changed, @See commit function
      // in dialog.
      // https://docs.ckeditor.com/ckeditor4/latest/guide/widget_sdk_tutorial_2.html.
      data: function () {
      }

    });
    /// editor.widgets.add.

    editor.ui.addButton('moha_widget', {
      label: 'Moha Widget',
      command: 'moha_widget',
    });
    /***/

  }
/// init.
});