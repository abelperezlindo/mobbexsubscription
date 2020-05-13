<?php

function create_subscription_form($form, &$form_state){

    $form['name'] = [
        '#type' => 'textfield',
        '#title' => t('Nombre'),
        '#description' => t('El nombre de la suscripción, se mostrará como un titulo en el Checkout.'),
        '#required' => TRUE,
    ];
    $form['description'] = [
        '#type' => 'textfield',
        '#title' => t('Descripción'),
        '#description' => t('Descripción de la operación que será mostrada en el Checkout.'),
        '#required' => TRUE,
    ];
    $form['type'] = [
        '#type' => 'select',
        '#title' => t('Tipo'),
        '#options' => ['dynamic' => 'dynamic', 'manual' => 'manual'],
        '#description' => t('Valores posibles: dynamic o manual. Las suscripciones de tipo manual deberán procesarse externamente llamando al servicio, no se tendrá en cuenta intervalo en este tipo.'),
        '#default_value' => 'dynamic',
        '#required' => TRUE,
    ];
    $form['interval'] = [
        '#type' => 'textfield',
        '#title' => t('Intervalo'),
        '#description' => t('Intervalo de Cobro de la suscripción.'),
        '#default_value' => '1m',
        '#required' => TRUE,
    ];
    $form['trial'] = [
        '#type' => 'textfield',
        '#title' => t('Periodos de prueba'),
        '#description' => t('Cantidadd de Periodos de Prueba.'),
        '#default_value' => '0',
        '#required' => TRUE,
    ]; 
    $form['limit'] = [
        '#type' => 'textfield',
        '#title' => t('Duración limite'),
        '#description' => t('Duracion de la suscripción en meses'),
        '#default_value' => '0',
        '#required' => TRUE,
    ];
    $form['total'] = [
        '#type' => 'textfield',
        '#title' => t('Monto'),
        '#description' => t('Cantidadd de Periodos de Prueba.'),
        '#required' => TRUE,
    ];
    $form['currency'] = [
        '#type' => 'textfield',
        '#title' => t('Tipo de moneda'),
        '#description' => t('Moneda de la operación.'),
        '#default_value' => 'ARS',
        '#required' => TRUE,

    ];
    $form['webhook'] = [
        '#type' => 'textfield',
        '#title' => t('WebHook'),
        '#description' => t('URL en la que se recibirán las notificaciones via POST. Debe ser HTTPs.'),
        '#required' => TRUE,
    ];
    $form['return_url'] = [
        '#type' => 'textfield',
        '#title' => t('Url de returno'),
        '#description' => t('URL a la que se enviará al usuario una vez suscripto. Debe ser HTTPs.'),
        '#required' => TRUE,
    ];
    $form['confirm'] = [
        '#type' => 'submit',
        '#name' => 'confirmNewSubscription',
        '#value' => t('Confirmar'),
        '#validate' => ['confirmNewSubscription_validate'],
        '#submit' => ['confirmNewSubscription_submit'],
    ];
    return $form;
}
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
    /*
    if (empty($form_state['values']['trial'])) {
        form_set_error('trial', t('Campo obligatorio'));
    }
    if (empty($form_state['values']['limit'])) {
        form_set_error('limit', t('Campo obligatorio'));
    }
    */
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
function confirmNewSubscription_submit($form, &$form_state){

    $apikey = variable_get('mobbexsubscription_apikey', 0);
    $token  = variable_get('mobbexsubscription_apikey', 0);


    if(!$apikey || !$token) return;

    $urlBase = 'https://api.mobbex.com/p/subscriptions';
    $data = [
            'name'          => $form_state['values']['name'],
            'description'   => $form_state['values']['description'],
            'type'          => $form_state['values']['type'],
            'interval'      => $form_state['values']['interval'],
            'trial'         => $form_state['values']['trial'],
            'limit'         => $form_state['values']['limit'],
            'total'         => $form_state['values']['total'],
            'currency'      => 'ARS',
            'webhook'       => $form_state['values']['webhook'],
            'return_url'    => $form_state['values']['return_url'],
            
    ];
    $query = drupal_http_build_query($data);
    //$query = urlencode($query);
    //$query = json_encode($data);
    $options = array(
        'method' => 'POST',
        'data' => $query,
        'headers' => [
            'x-api-key'      => $apikey,
            'x-access-token' => $token,
            'Content-Type'   => 'application/json',
            'charset'        => 'utf-8'
        ],
    );
    $response = drupal_http_request($urlBase, $options);

    if (($response->code == 200) && ($response->status_message == 'OK')){
        
        $result =  json_decode($response->data, TRUE);
        //drupal_set_message('<pre>'.print_r($data, TRUE).'</pre>');
        
        drupal_set_message('<pre>'.print_r($result, TRUE).'</pre>');
    }
    drupal_set_message('<pre>'.print_r($data, TRUE).'</pre>');
    drupal_set_message('<pre>'.print_r($response, TRUE).'</pre>');
    
}