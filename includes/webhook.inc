<?php
include "data_access.inc";
/**
 *  Obtiene la llamada desde Mobbex
 */

 // Query params from POST.
function webhook_form($form, &$form_state){
    $postdata = file_get_contents("php://input");
    //$obj = rawurldecode($postdata);
    $obj = array();    
    parse_str($postdata, $obj);
    if(empty($obj)){
        watchdog("debug", 'Empty request to mobbex webhook');
        return [];
    }else{
        watchdog("debug", '<pre> parse_str() : <br />'. print_r( $obj, true) . '</pre>');
    }

    
   // $obj = $postdata;

    return $form;
}

function manageWebhookRequest($obj){
    switch($obj['type']){
        case "subscription:registration" : 
            //nueva suscripción
            $fields = [
                'uid'            => 4,
                'subscription_id'=> 2,
                'subscriber_id'  => 3,
                'status'         => 1,
                'source'         => 'card',
                'uptodatepay'    => 1
            ];
            mobbexsubscription_entry_insert($fields);
            
        break;
        case "subscription:source_change" :
            //Cambio de Método de Pago
            $fields = [
                'uid'            => 4,
                'subscription_id'=> 2,
                'subscriber_id'  => 3,
                'status'         => 1,
                'source'         => 'card',
                'uptodatepay'    => 1
            ];
            mobbexsubscription_entry_update($fields);
        break;
        case "subscription:execution" : 
            //Suscripción Ejecutada
            #comprobamos si la transaccion afecta al estado actual de un suscriptor

        break;
        case "subscription:subscriber:suspended" : 
            //Suscriptor Suspendido
            $fields = [
                'uid'            => 4,
                'subscription_id'=> 2,
                'subscriber_id'  => 3,
                'status'         => 1,
                'source'         => 'card',
                'uptodatepay'    => 1
            ];
            mobbexsubscription_entry_update($fields);
        break;
        case "subscription:subscriber:active" : 
            //Suscriptor Activado
            $fields = [
                'uid'            => 4,
                'subscription_id'=> 2,
                'subscriber_id'  => 3,
                'status'         => 1,
                'source'         => 'card',
                'uptodatepay'    => 1
            ];
            mobbexsubscription_entry_update($fields);
        break;
    }

}
