
var hint_area_template = '';
hint_area_template += '<div class="moha-spotlight info-area">';
hint_area_template += '<div class="moha-spotlight-icon hint-icon fa fa-info"><span>&nbsp;</span></div>';
hint_area_template += '<div class="moha-spotlight-details hint-details"><p>&nbsp;</p></div>';
hint_area_template += '</div>';

var allowedContents = 'p br ul ol li strong em a div span i img';

CKEDITOR.plugins.add( 'moha_spotlight', {
  requires: 'widget',
  icons: 'moha_spotlight',

  init: function( editor ) {
    editor.widgets.add( 'moha_spotlight', {

      button: 'Moha Spotlight Widget',

      template: hint_area_template,

      editables: {
        details: {
          selector: '.moha-spotlight-details',
          allowedContent: allowedContents
        }
      },

      allowedContent: allowedContents,

      upcast: function( element ) {
        return element.name == 'div' && element.hasClass( 'moha-spotlight' );
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
