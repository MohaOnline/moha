
var hint_area_template = '';
hint_area_template += '<div class="moha-spotlight hint-area">';
hint_area_template += '<div class="moha-spotlight-icon hint-icon fa fa-info"><span>&nbsp;</span></div>';
hint_area_template += '<div class="moha-spotlight-details hint-details"><p>&nbsp;</p>';
hint_area_template += '<p>&nbsp;</p></div>';
hint_area_template += '</div>';

var reference_links_template = '';
reference_links_template += '<div class="moha-spotlight reference-area">';
reference_links_template += '<div class="moha-spotlight-icon reference-icon fa fa-external-link"><span>&nbsp;</span></div>';
reference_links_template += '<div class="moha-spotlight-details reference-details"><p>&nbsp;</p>';
reference_links_template += '<p>&nbsp;</p></div>';
reference_links_template += '</div>';

CKEDITOR.addTemplates('default',
  {
    imagesPath: Drupal.settings.moha_clip.ckeditor_template_image_path,
    templates: [
      {
        title: 'Hint block',
        image: 'reference-block.png',
        description: 'Gives hint messages.',
        html: hint_area_template
      },
      {
        title: 'Reference block',
        image: 'reference-block.png',
        description: 'Collects related reference links.',
        html: reference_links_template
      }
    ]
  });
