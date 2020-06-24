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

function subscriber_page_form($form, &$form_state)
{
    //status=200&sid=8N_JHkTvV
    if (isset($_GET["status"]) && isset($_GET["sid"])) {
        //suscripcion completa
        $welcome = $_GET["sid"];
    }
    $apikey            = variable_get('mobbexsubscription_apikey', 0);
    $token             = variable_get('mobbexsubscription_token', 0);
    if (empty($apikey) || empty($token)) return;
    $idSubscription    = variable_get('mobbexsubscription_idsubs', 0);
    $user              = $GLOBALS['user'];
    $sub               = mobbexsubscription_entry_get($user->uid);
    $roleActSub        = user_role_load_by_name('active_subscriptor');
    $userHasActRole    = user_has_role($roleActSub->rid, $user);


    if (empty($sub) || is_array($sub)) {
        //El usuario nunca envio el formulario de suscripción.
        $form['Message'] = [
            '#markup' => 'No suscipto'
        ];
        return $form;
    } elseif ($idSubscription !== $sub->subscription_id) {
        // Hay una discrepancia con la subscripción seteada en la 
        //configuracion del modulo y la suscripción a la cual 
        //pertenece este usuario. Se debe manejar este error.
        return;
    }
    $mbx = new Moxy($apikey, $token);
    $subData = $mbx->getSubscriber($sub->subscription_id, $sub->subscriber_id);

    if (!$userHasActRole) {
        //el usuario realizo la suscripcion pero no tiene un medio de pago activo.
        $link = l(
            'aquí',
            $subData['data']['url']
        );
        $form['Message'] = [
            '#markup' => 'Debe agregar un medio de pago en ' . $link . ', su suscripcion será activada cuando se debite el primer periodo'
        ];
        return $form;
    }


    $form['subscriptor'] = [
        '#markup' => '<pre>' . print_r($sub, TRUE) . '</pre>'
    ];
    $form['message'] = [
        '#markup' => '<pre>' . print_r($subData, TRUE) . '</pre>'
    ];
    return $form;
}
