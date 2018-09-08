<?php

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup templates
 */

// Additional configuration for tocify.
drupal_add_js(array(
  'tocify' => array(
    // Usage: Drupal.settings.tocify.<CONFIG NAME>.
    //   '<CONFIG NAME>' => <CONFIG VALUE>,.
    'showAndHide' => false,
    'scrollTo' => 30,
    'highlightOffset' => 30,
    'extendPage' => false,

  )
), 'setting');

drupal_add_js(MOHA_CLIP__RELATIVE_PATH . '/js/moha_clip.js');

drupal_add_css(MOHA_CLIP__RELATIVE_PATH . '/css/moha_clip.css', array('group' => CSS_THEME, 'media' => 'all', 'weight' => 990));
drupal_add_css(MOHA_CLIP__RELATIVE_PATH . '/ckeditor/styles.css', array('group' => CSS_THEME, 'media' => 'all', 'weight' => 998));
?>

<link  href="/sites/all/libraries/ckeditor/ckeditor-4.7.3-full/plugins/codesnippet/lib/highlight/styles/monokai_sublime.css" rel="stylesheet" />
<script src="/sites/all/libraries/ckeditor/ckeditor-4.7.3-full/plugins/codesnippet/lib/highlight/highlight.pack.js"></script>

<link  href="/sites/all/libraries/swiper/dist/css/swiper.min.css" rel="stylesheet" />
<script src="/sites/all/libraries/swiper/dist/js/swiper.min.js"></script>

<script>hljs.initHighlightingOnLoad();</script>

<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if ((!$page && !empty($title)) || !empty($title_prefix) || !empty($title_suffix) || $display_submitted): ?>
  <header>
    <?php print render($title_prefix); ?>
    <?php if (!$page && !empty($title)): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
  </header>
  <?php endif; ?>



  <div class="row">
    <!---- Main Content -->
    <div class="white-back-2018 col-md-9 col-xs-12">
    <?php
      // Hide comments, tags, and links fields from default render.
      hide($content['language']);
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      hide($content['technology_stacks']);
      print render($content);
    ?>
    </div>
    <!-- Main Content ---->

    <!---- Sidebar -->
    <div class="white-back-2018 col-md-3 col-xs-12">
      <div class="white-back-2018 author-block">
        <?php if ($display_submitted): ?>
          <div class="author-name"><?php print t("Author: ") . $name; ?></div>
          <?php print $user_picture; ?>
          <?php $changed_date = new \DateTime(); $changed_date->setTimestamp($node->changed); ?>
          <div class="article-created"><?php print t('Updated: ') . $changed_date->format('Y/m/d'); ?></div>
        <?php endif; ?>
        <?php
        // Include Reward block if enabled.
        if (defined('__MOHA_REWARD')) {
          $block = module_invoke(__MOHA_REWARD, 'block_view', MOHA_REWARD_BLOCK);
          if (!empty($block['content'])) {
            print render($block['content']);
          }
        }
        ?>
      </div>

      <div class="toc-block">
        <!--
        <div class="toc" data-toc="h2, h3">
        </div>
        -->
        <?php
        $block = module_invoke('tocify', 'block_view', 'tocify');
        if (!empty($block['content'])) {
          print render($block['content']);
        }
        ?>

      </div>
      <div class="moha-clip-scroll-2-top">
        <i class="fa fa-chevron-up" aria-hidden="true"></i>
        <!--jQuery("html, body").animate({ scrollTop: 0 });-->
      </div>
    </div>
    <!-- Sidebar ---->

  </div>
  <?php
    // Only display the wrapper div if there are tags or links.
    $field_tags = render($content['field_tags']);
    $links = render($content['links']);
    if ($field_tags || $links):
  ?>
   <footer>
     <?php print $field_tags; ?>
     <?php print $links; ?>
  </footer>
    <?php endif; ?>
  <?php print render($content['comments']); ?>
</article>
