<?php
  include ('/opt/bitnami/apps/wordpress/htdocs/wp-load.php');
  switch_to_blog(3);
  global $allowedposttags;
  $newtag = array('script' => array(
       'src' => array(),
  ));
  $allowedposttags = array_merge( $allowedposttags,$newtag);

  $p1 = get_page_by_title( $argv[1] );
  if ( empty($p1) ) 
  { 
    echo "post not found ";
  } 
  else 
  { 
    echo "deleting old post ";
    wp_delete_post($p1->ID);
  }

  $p1 = get_page_by_title( '1-Ceres' );
  $templ = $p1->post_content;


  $postType     = 'page';
  $leadTitle    = $argv[1];
  $leadContent  = str_replace('1-Ceres',$argv[1], $templ);
  $status       = 'publish';

  $newPost = array(
    'post_title' => $leadTitle,
    'post_content' => $leadContent,
    'post_type' => $postType,
    'post_status' => $status 
    );

  $id = wp_insert_post($newPost, true);

  if (is_wp_error ($id) ) {
     echo $result->get_error_message();
  } 
  else
  {
    echo "inserted new post \n";
  }
?>
