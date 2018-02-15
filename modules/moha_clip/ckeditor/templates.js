
var reference_links_template = '';
reference_links_template += '<div class="reference-area">';
reference_links_template += '<div class="reference-icon fa fa-external-link fa-2x"><span>&nbsp;</span></div>';
reference_links_template += '<div class="reference-details"><p>&nbsp;</p>';
reference_links_template += '<p>&nbsp;</p></div>';
reference_links_template += '</div>';


CKEDITOR.addTemplates('default',
  {
    imagesPath: Drupal.settings.moha_clip.ckeditor_template_image_path,
    templates: [
      {
        title: 'Reference block',
        image: 'reference-block.png',
        description: 'Collects related reference links.',
        html: reference_links_template
      }
    ]
  });
