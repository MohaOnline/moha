/**
 * @license Copyright (c) 2003-2018, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see LICENSE.md or https://ckeditor.com/legal/ckeditor-oss-license
 */

/**
 * @fileOverview The Save plugin.
 */

( function() {
	const mohaSaveCmd = {
		readOnly: 1,
		modes: { wysiwyg: 1,source: 1 },

		exec: function( editor ) {
			if ( editor.fire( 'moha_save' ) ) {
				const $form = editor.element.$.form;

				if ( $form ) {
					try {
            // jQuery($form).find('#edit-silent-save').trigger('mousedown');

            const element = $form.getElementsByClassName('edit-silent-save-processed')[0];
            let event; // The custom event that will be created

            if (document.createEvent) {
              event = document.createEvent("HTMLEvents");
              event.initEvent("mousedown", true, true);
            } else {
              event = document.createEventObject();
              event.eventType = "mousedown";
            }

            event.eventName = "mousedown";

            if (document.createEvent) {
              element.dispatchEvent(event);
            } else {
              element.fireEvent("on" + event.eventType, event);
            }

					} catch ( e ) {
						// If there's a button named "submit" then the form.submit
						// function is masked and can't be called in IE/FF, so we
						// call the click() method of that button.
						if ( $form.submit.click )
							$form.submit.click();
					}
				}
			}
		}
	};

	var pluginName = 'moha_save';

	// Register a plugin named "moha_save".
	CKEDITOR.plugins.add( pluginName, {
		icons: 'moha_save',
		hidpi: true,
		init: function( editor ) {
			// Save plugin is for replace mode only.
			if ( editor.elementMode != CKEDITOR.ELEMENT_MODE_REPLACE )
				return;

			var command = editor.addCommand( pluginName, mohaSaveCmd );
			command.startDisabled = !( editor.element.$.form );

			editor.ui.addButton && editor.ui.addButton( 'Moha Save', {
				label: 'Moha Save',
				command: pluginName,
				toolbar: 'document,9'
			} );
		}
	} );
} )();

/**
 * Fired when the user clicks the Save button on the editor toolbar.
 * This event allows to overwrite the default Save button behavior.
 *
 * @since 4.2
 * @event save
 * @member CKEDITOR.editor
 * @param {CKEDITOR.editor} editor This editor instance.
 */
