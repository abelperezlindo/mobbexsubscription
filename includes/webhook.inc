<?php
/**
 * @file 
 * Obtiene la llamada desde Mobbex
 */

 // Query params from POST.

/**
 * Implements hook_form().
 *      Formulario caprura json (prox retirar form)
 */
function webhook_form($form, &$form_state){
    $wh = new MoxyWebhook();
    #MoxyWebhook
    //Vemos si se recibio contenido
    if(!$wh->getPostString()) return;

    $type   = $wh->getPostArray('type');
    $data   = $wh->getPostArray('data');

    if( $type == 'subscription:registration' ||  $type == 'subscription:source_change'){
        // Ocurre un nuevo registro en nuestra suscripcion
        // O
        // Un suscriptor cambia su metodo de pago
        if($data['result'] != "true") return;
        //"reference" y "id_drupal" guardan el id de usuario en nuestro aplicativo
        if(isset($data['subscriber']['customer']['id_drupal'])){
            $uidFromMobbex = $data['subscriber']['customer']['id_drupal'];
        }
        elseif(isset($data['suscriber']['reference'])){
            $uidFromMobbex = intval(isset($data['suscriber']['reference']));
        }
        $user = user_load($uidFromMobbex);
        //manejamos el error de usuario no encontrado
        if(!isset($user)) return;
        $estado = $data['payment']['status']['code'];
        if($estado == "200" || $estado == "602"){
            //Estado 200 = Paga, mensage "aprobada"
            //Estado 602 = Devuelto, mensaje "Transacción Aprobada" (cambio de m p)
            $entry = mobbexsubscription_entry_get($user->uid);
            if(!$entry->status){
                $fields = [
                    'uid'            => $entry->uid,
                    'subscription_id'=> $entry->subscription_id,
                    'subscriber_id'  => $entry->subscriber_id,
                    'status'         => 1,
                  ];
                  mobbexsubscription_entry_update($fields);
            }
            if(!array_search('active_subscriptor', $user->roles)){
                //ponemos el rol de suscriptor activo
                activeSubscriptionRole($user->uid, TRUE);
            }
        }
        return;
    }

    if($type == 'subscription:subscriber:suspended'){
        //Se suspendio a un suscriptor
        if(isset($data['subscriber']['customer']['id_drupal'])){
            $uidFromMobbex = $data['subscriber']['customer']['id_drupal'];
        }
        elseif(isset($data['suscriber']['reference'])){
            $uidFromMobbex = intval(isset($data['suscriber']['reference']));
        }
        $user = user_load($uidFromMobbex);
        $entry = mobbexsubscription_entry_get($user->uid);
        if($entry->status){
            //Si esta activo en nuestra tabla lo pasamos a inactivo
            $fields = [
                'uid'            => $entry->uid,
                'subscription_id'=> $entry->subscription_id,
                'subscriber_id'  => $entry->subscriber_id,
                'status'         => 0,
                ];
                mobbexsubscription_entry_update($fields);
        }
        if(array_search('active_subscriptor', $user->roles)){
            //sacamos el rol de suscriptor activo
            activeSubscriptionRole($user->uid, FALSE);
        }
        return;
    }
    if($type == 'subscription:subscriber:active'){
        //Se activo a un suscriptor
        if(isset($data['subscriber']['customer']['id_drupal'])){
            $uidFromMobbex = $data['subscriber']['customer']['id_drupal'];
        }
        elseif(isset($data['suscriber']['reference'])){
            $uidFromMobbex = intval(isset($data['suscriber']['reference']));
        }
        $user = user_load($uidFromMobbex);
        $entry = mobbexsubscription_entry_get($user->uid);
        if(empty($entry->status)){
            //Si esta activo en nuestra tabla lo pasamos a inactivo
            $fields = [
                'uid'            => $entry->uid,
                'subscription_id'=> $entry->subscription_id,
                'subscriber_id'  => $entry->subscriber_id,
                'status'         => 1,
                ];
                mobbexsubscription_entry_update($fields);
        }
        if(!array_search('active_subscriptor', $user->roles)){
            //ponemos el rol de suscriptor activo
            activeSubscriptionRole($user->uid, TRUE);
        }
        return;
    }

    if( $type == 'subscription:execution'){
        // Si la suscripcion es Dinamica se realizaran las ejecuciones automaticamente 
        // según la agenda.
        if($data['result'] != "true") return;
        //"reference" y "id_drupal" guardan el id de usuario en nuestro aplicativo
        if(isset($data['subscriber']['customer']['id_drupal'])){
            $uidFromMobbex = $data['subscriber']['customer']['id_drupal'];
        }
        elseif(isset($data['suscriber']['reference'])){
            $uidFromMobbex = intval(isset($data['suscriber']['reference']));
        }
        $user = user_load($uidFromMobbex);
        //manejamos el error de usuario no encontrado
        if(!isset($user)) return;
        $estado = $data['payment']['status']['code'];
        if($estado == "200"){
            //Estado 200 = Paga, mensage "aprobada"
            $entry = mobbexsubscription_entry_get($user->uid);
            if(empty($entry)) return; 
            // si no lo tenemos en la tabla de las suscripciones
            // el usuario no se registro desde nuesta applic
            if(!$entry->status){
                $fields = [
                    'uid'            => $entry->uid,
                    'subscription_id'=> $entry->subscription_id,
                    'subscriber_id'  => $entry->subscriber_id,
                    'status'         => 1,
                  ];
                  mobbexsubscription_entry_update($fields);
            }
            if(!array_search('active_subscriptor', $user->roles)){
                //ponemos el rol de suscriptor activo
                activeSubscriptionRole($user->uid, TRUE);
            }
            //guardamos la ejecucion y el monto en db
            $fields = [
                    'uid'            => $uidFromMobbex,
                    'subscription_id'=> $data['subscription']['uid'],
                    'subscriber_id'  => $data['subscriber']['uid'],
                    'description'    => $data['payment']['description'],
                    'status'         => $data['payment']['status']['text'],
                    'total'          => $data['payment']['total'],
                    'datetime'       => date( "Y-m-d H:i:s", strtotime($data['payment']['created']) ),
                    'reference'      => $data['payment']['reference'],
            ];
            mobbexexecution_entry_insert($fields);
        }
        return;
    }
}