
var info_area_template = '';
info_area_template += '<div class="moha-spotlight info-area">';
info_area_template += '<div class="moha-spotlight-icon hint-icon fa fa-info"><span>&nbsp;</span></div>';
info_area_template += '<div class="moha-spotlight-details hint-details"><p>&nbsp;</p></div>';
info_area_template += '</div>';

var reference_links_template = '';
reference_links_template += '<div class="moha-spotlight reference-area">';
reference_links_template += '<div class="moha-spotlight-icon reference-icon fa fa-external-link"><span>&nbsp;</span></div>';
reference_links_template += '<div class="moha-spotlight-details reference-details"><p>&nbsp;</p></div>';
reference_links_template += '</div>';

CKEDITOR.addTemplates('default',
  {
    imagesPath: Drupal.settings.moha_clip.ckeditor_template_image_path,
    templates: [
      {
        title: 'Block Info',
        image: 'block-info.png',
        description: 'Gives info messages.',
        html: info_area_template
      },
      {
        title: 'Block Reference',
        image: 'block-reference.png',
        description: 'Collects related reference links.',
        html: reference_links_template
      }
    ]
  });
