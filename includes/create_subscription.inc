<?php
/**
 * @file
 * Contiene un formulario.
 * 
 * Implements hook_form().
 * 
 *      Formulario para alta una nueva suscripción.
 * 
 */
include "moxy.php";
include "data_access.inc";
function create_subscription_form($form, &$form_state){
    global $base_url;
    $form['name'] = [
        '#type'          => 'textfield',
        '#title'         => t('Nombre'),
        '#description'   => t('El nombre de la suscripción, se mostrará como un titulo en el Checkout.'),
        '#required'      => TRUE,
    ];
    $form['description'] = [
        '#type'          => 'textfield',
        '#title'         => t('Descripción'),
        '#description'   => t('Descripción de la operación que será mostrada en el Checkout.'),
        '#required'      => TRUE,
    ];
    $form['type'] = [
        '#type'          => 'select',
        '#title'         => t('Tipo'),
        '#options'       => ['dynamic' => 'dynamic', 'manual' => 'manual'],
        '#description'   => t('Valores posibles: dynamic o manual. Las suscripciones de tipo manual deberán procesarse externamente llamando al servicio, no se tendrá en cuenta intervalo en este tipo.'),
        '#default_value' => 'dynamic',
        '#required'      => TRUE,
    ];
    $form['interval'] = [
        '#type'          => 'textfield',
        '#title'         => t('Intervalo'),
        '#description'   => t('Intervalo de Cobro de la suscripción.'),
        '#default_value' => '1m',
        '#required'      => TRUE,
    ];
    $form['trial'] = [
        '#type'          => 'textfield',
        '#title'         => t('Periodos de prueba'),
        '#description'   => t('Cantidadd de Periodos de Prueba.'),
        '#default_value' => '0',
        '#required'      => TRUE,
    ]; 
    $form['limit'] = [
        '#type'          => 'textfield',
        '#title'         => t('Duración limite'),
        '#description'   => t('Duracion de la suscripción en meses'),
        '#default_value' => '0',
        '#required'      => TRUE,
    ];
    $form['total'] = [
        '#type'          => 'textfield',
        '#title'         => t('Monto'),
        '#description'   => t('Cantidadd de Periodos de Prueba.'),
        '#required'      => TRUE,
    ];
    $form['currency'] = [
        '#type'          => 'textfield',
        '#title'         => t('Tipo de moneda'),
        '#description'   => t('Moneda de la operación.'),
        '#default_value' => 'ARS',
        '#required'      => TRUE,

    ];
    $form['webhook'] = [
        '#type'          => 'textfield',
        '#title'         => t('WebHook'),
        '#description'   => t('URL en la que se recibirán las notificaciones via POST. Debe ser HTTPs.'),
        '#required'      => TRUE,
        '#default_value' => $base_url.'/mobbex/webhook',
    ];
    $form['return_url'] = [
        '#type'          => 'textfield',
        '#title'         => t('Url de returno'),
        '#description'   => t('URL a la que se enviará al usuario una vez suscripto. Debe ser HTTPs.'),
        '#required'      => TRUE,
        '#default_value' => $base_url.'/mi-suscripcion',
    ];
    $form['confirm'] = [
        '#type'          => 'submit',
        '#name'          => 'confirmNewSubscription',
        '#value'         => t('Confirmar'),
        '#validate'      => ['confirmNewSubscription_validate'],
        '#submit'        => ['confirmNewSubscription_submit'],
    ];

    return $form;
}
/**
 * Validación del Formulario
 */
function confirmNewSubscription_validate($form, &$form_state){
    if (empty($form_state['values']['name'])) {
        form_set_error('name', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['description'])) {
        form_set_error('description', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['type'])) {
        form_set_error('type', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['interval'])) {
        form_set_error('interval', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['currency'])) {
        form_set_error('currency', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['total'])) {
        form_set_error('total', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['webhook'])) {
        form_set_error('', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['return_url'])) {
        form_set_error('', t('Campo obligatorio'));
    }
}
/**
 * Envío del formulario.
 */
function confirmNewSubscription_submit($form, &$form_state){

    
    $apikey = variable_get('mobbexsubscription_apikey', 0);
    $token  = variable_get('mobbexsubscription_token', 0);

    if(!$apikey || !$token) return;
 
    $data = [
        'name'          => $form_state['values']['name'],
        'description'   => $form_state['values']['description'],
        'type'          => $form_state['values']['type'],
        'interval'      => $form_state['values']['interval'],
        'trial'         => $form_state['values']['trial'],
        'limit'         => $form_state['values']['limit'],
        'total'         => $form_state['values']['total'],
        'currency'      => $form_state['values']['total'],
        'webhook'       => $form_state['values']['webhook'],
        'return_url'    => $form_state['values']['return_url'],
    ];
    $mbx  = new Moxy($apikey, $token);
    $body = $mbx->setNewSuscription($data);
    
    if($body['result'] == 'true')
        drupal_set_message( 'Se creó la suscripcion '.$body['data']['uid']);
    else 
        drupal_set_massage('Ocurrio un error', 'ERROR');
    
}