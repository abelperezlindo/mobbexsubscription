<?php
/**
 * @file 
 * Info suscripción por usuario
 * 
 * Implements hook_form()
 * 
 *      Informacion de suscripción por usuarios
 * 
 */
include "data_access.inc";
include "moxy.php";
function subscriber_page_form($form, &$form_state){

    $apikey            = variable_get('mobbexsubscription_apikey', 0);
    $token             = variable_get('mobbexsubscription_token', 0);
    $idSubscription    = variable_get('mobbexsubscription_idsubs', 0);
    $user              = $GLOBALS['user'];
    $sub               = mobbexsubscription_entry_get($user->uid);
    $userHasActRole    = isCurrentUserSuscriber($user);

    if(!$userHasActRole || empty($sub)) {
        $form['Message'] = [
            '#markup' => 'No suscipto'
        ];
        return $form;
    }
    if($idSubscription !== $sub->subscription_id){
        $form['message'] = [
            '#markup' => '<pre> No hay coincidencia con la suscripcion</pre>'
        ];
        return $form;
    }
    $mbx = new Moxy($apikey, $token);
    $subData = $mbx->getSubscriber($sub->subscription_id, $sub->subscriber_id);

    $form['subscriptor'] = [
        '#markup' => '<pre>'.print_r($sub, TRUE).'</pre>'
    ];
    $form['message'] = [
        '#markup' => '<pre>'.print_r($subData, TRUE).'</pre>'
    ];
    return $form;
  
}
