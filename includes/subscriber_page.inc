<?php

/**
 *      Implements hook_form()
 * 
 *      Informacion de suscripción por usuarios
 * 
 */

function subscriber_page_form($form, &$form_state){
    $user = $GLOBALS['user'];
    $roles = $user->roles;
    $body = [];
    $active = activeSubscriptorCheck($body);

    if(array_search('inactive_subscriptor', $user->roles) && $active){
        changeSubcriberRoles(true);
    }
    drupal_set_message('<pre>'.print_r($body, TRUE).'</pre>');
    return $form;
}


function activeSubscriptorCheck(array &$body = []){
    $uid = $GLOBALS['user']->uid;
    $apikey = variable_get('mobbexsubscription_apikey', 0);
    $token  = variable_get('mobbexsubscription_token', 0);

    $user = user_load($uid);
    $subscription   = $user->field_mobbex_subscription_uid[LANGUAGE_NONE]['0']['value'];
    $subscriber     = $user->field_mobbex_subscriber_uid[LANGUAGE_NONE]['0']['value'];
    $subscriber_url = $user->field_mobbex_subscriber_url[LANGUAGE_NONE]['0']['value'];
    drupal_set_message($subscriber.$subscription);
    
    $request = new HTTP_Request2();
    $request->setUrl('https://api.mobbex.com/p/subscriptions/'. $subscription .'/subscriber/'. $subscriber);
    $request->setMethod(HTTP_Request2::METHOD_GET);
    $request->setConfig(array(
    'follow_redirects' => TRUE
    ));
    $request->setHeader(array(
    'x-api-key'      => $apikey,
    'x-access-token' => $token ,
    'Content-Type'   => 'application/json'
    ));
    try {
    $response = $request->send();
    if ($response->getStatus() == 200) {
        drupal_set_message('Respuesta correcta 200');
        drupal_set_message('<pre>'.print_r($response->getBody(), TRUE).'</pre>');
        $body = json_decode( $response->getBody(), TRUE);
        drupal_set_message('<pre>'.print_r($body, TRUE).'</pre>');
        if($body['data']['status'] == "active"){
            if(isset($body['data']['executions']) && $body['data']['executions']['status'] == 'paid'){
                return true;
            }
        }
    }
    else {
        $message = 'Unexpected HTTP status: ' . $response->getStatus() . ' ' .$response->getReasonPhrase();
        watchdog('mebbexsubscription', $message, array(), WATCHDOG_ERROR);
        drupal_set_message($message);     
        }
    }
    catch(HTTP_Request2_Exception $e) {
        $message = 'Error: ' . $e->getMessage();
        watchdog('mebbexsubscription', $message, array(), WATCHDOG_ERROR);
        drupal_set_message($message);
    }
    return false;
}