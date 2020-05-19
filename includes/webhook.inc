<?php

/**
 *  Obtiene la llamada desde Mobbex
 */

 // Query params from POST.
function webhook_form($form, &$form_state){
    $postdata = file_get_contents("php://input");
    $obj = json_decode($postdata, true);  
    $ip = ip_address();
    
    drupal_set_message( '<pre>' . print_r( $ip, true) . '</pre>');
    watchdog("debug", '<pre> ip'. $ip .  print_r( $postdata, true) . '</pre>'); 
    drupal_set_message( '<pre>' . print_r( $_POST, true) . '</pre>');
    return $form;
}
