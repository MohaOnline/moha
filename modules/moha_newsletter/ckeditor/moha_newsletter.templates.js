/**
 * @file
 */


CKEDITOR.addTemplates('moha_newsletter',
  {
    imagesPath: Drupal.settings.moha_newsletter.ckeditor_template_image_path,

    templates: [
      {
        title: 'eMail - 850px',
        html: '<table border="0" cellpadding="0" cellspacing="0" width="850" style="border:none;" class="email-wrapper responsive-table"><tr><td>&nbsp;</td></tr></table>'
      },
      {
        title: 'eMail - Outside',
        html: '<table border="0" cellpadding="0" cellspacing="0" width="100%" class="email-wrapper"><tr><td><div align="center">&nbsp;</div></td></tr></table>'
      },
      {
        title: 'Paragraph - Normal',
        html: '<p>&nbsp;</p>'
      },
      {
        title: 'Paragraph - Center',
        html: '<p align="center">&nbsp;</p>'
      }
    ]
  }
  );
