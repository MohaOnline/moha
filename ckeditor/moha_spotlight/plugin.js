
var info_area_template = '';
info_area_template += '<div class="moha-spotlight info-area">';
info_area_template += '<div class="moha-spotlight-icon fa fa-info"><span>&nbsp;</span></div>';
info_area_template += '<div class="moha-spotlight-details"><p>&nbsp;</p></div>';
info_area_template += '</div>';

var allowedContents = 'p br ul ol li strong em a div span i img';

CKEDITOR.plugins.add( 'moha_spotlight', {
  requires: 'widget',
  icons: 'moha_spotlight',

  init: function( editor ) {
    // Register dialog, prepare for moha_spotlight widget.
    CKEDITOR.dialog.add( 'moha_spotlight', this.path + 'dialogs/moha_spotlight.js' );

    editor.widgets.add( 'moha_spotlight', {

      button: 'Moha Spotlight Widget',
      template: info_area_template,
      allowedContent: allowedContents,
      dialog: 'moha_spotlight',

      editables: {
        details: {
          selector: '.moha-spotlight-details',
          allowedContent: allowedContents
        }
      },

      upcast: function( element ) {
        return element.name == 'div' && element.hasClass( 'moha-spotlight' );
      },

      // Read widget DOM status, then set to Widget data structure.
      init: function () {
        // set widget.data.type values per class of DOM.
        if ( this.element.hasClass( 'primary-area' ) ){
          this.setData( 'type', 'primary-area' );
        }
        else if ( this.element.hasClass( 'info-area' ) ){
          this.setData( 'type', 'info-area' );
        }
        else if ( this.element.hasClass( 'reference-area' ) ){
          this.setData( 'type', 'reference-area' );
        }
        else if ( this.element.hasClass( 'warning-area' ) ){
          this.setData( 'type', 'warning-area' );
        }
        else if ( this.element.hasClass( 'danger-area' ) ){
          this.setData( 'type', 'danger-area' );
        }
      },

      // executed every time the widget data is changed, @See commit function in dialog.
      // https://docs.ckeditor.com/ckeditor4/latest/guide/widget_sdk_tutorial_2.html.
      data: function () {

        if ( this.data.type ) {
          // @See: https://docs.ckeditor.com/ckeditor4/latest/api/CKEDITOR_dom_element.html#method-removeClass
          this.element.removeClass('primary-area');
          this.element.removeClass('info-area');
          this.element.removeClass('reference-area');
          this.element.removeClass('warning-area');
          this.element.removeClass('danger-area');
          this.element.addClass(this.data.type);
        }
      }

    } );
    /// editor.widgets.add.

    editor.ui.addButton( 'moha_spotlight', {
      label : 'Moha Spotlight',
      command : 'moha_spotlight',
    });
    /***/

  }
  /// init.
} );
