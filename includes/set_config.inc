<?php

/**
 *      Implements hook_form()
 * 
 *      Formulario para alta para configuraciones de la subscripción
 * 
 */

function mobbexsubscription_config_form($form, &$form_state){

    $config = getDataConfig();

    $form['access_config'] = [
        '#type' => 'fieldset',
        '#title' => t('Acceso a la API de Mobbex'),
        '#description' => t('Ingrese la información necesaria para comunicarse con Mobbex'),
    ];
    $form['access_config']['api_key'] = [
        '#type' => 'textfield',
        '#title' => t('Clave API'),
        '#description' => t('Ingrese la Clave API de su aplicación'),
        '#default_value' => $config['apikey'],
    ];
    $form['access_config']['access_token'] = [
        '#type' => 'textfield',
        '#title' => t('Token de Acceso '),
        '#description' => t('Ingrese el Token de Acceso a la entidad para la cual desea realizar las operaciónes.'),
        '#default_value' => $config['token'],
    ];

    $form['subscription_config'] = [
        '#type' => 'fieldset',
        '#title' => t('Configuraciones Subscripción'),
        '#description' => t('Ingrese la información necesaria para configurar la subs'),
    ];
    $form['subscription_config']['id_subscription'] = [
        '#type' => 'textfield',
        '#title' => t('Id de Subscripción'),
        '#description' => t('Ingrese el ID de la Subscripción '),
        '#default_value' => $config['idsub'],
    ];
    if(isset($form_state['subsList'])){
        $form['subscription_config']['List'] = array(
            '#markup' => $form_state['subsList']
          );
    }
    $form['subscription_config']['createSubscription'] = array(
        '#markup'   => l(
            t('Crear nueva suscripción'),
            'admin/config/mobbexsubscription/nueva-suscripcion', 
            ['attributes' => ['class' => 'createSubscription-link']]
        ),
      );
    $form['subscription_config']['getSubList'] = array(
        '#type'   => 'submit',
        '#name'   => 'getSubList',
        '#value'  => t('Listar suscripciones'),
        '#validate' => ['getSubList_validate'],
        '#submit' => ['getSubList_submit'],
      );
    $form['users_config'] = [
        '#type' => 'fieldset',
        '#title' => t('Configuraciones para Usuarios'),
        '#description' => t('Ingrese la información necesaria para configurar los usuarios'),
        
    ];  
    $allRoles = user_roles(TRUE);
    $form['users_config']['role_subscriber_active'] = [
        '#type' => 'select',
        '#options' => $allRoles,
        '#title' => t('Rol para usuario suscrito'),
        '#description' => t('Seleccione el rol que se le asignara a un nuevo subscriptor'),
        '#default_value' => $config['activerole'],
    ];

    $form['save'] = array(
        '#type' => 'submit',
        '#name'   => 'SaveConfig',
        '#value' => t('Guardar las configuraciones'),
        '#validate' => ['SaveConfig_validate'],
        '#submit' => ['SaveConfig_submit'],
      );
    return $form;
}
function SaveConfig_validate($form, &$form_state){
    if (empty($form_state['values']['api_key'])) {
        form_set_error(
            'api_key', 
            t('Ingrese la Clave API de su aplicación')
        );
    }
    if (empty($form_state['values']['access_token'])) {
        form_set_error(
            'access_token', 
            t('Ingrese el Token de Acceso a la entidad para la 
            cual desea realizar las operaciónes')
        );
    }
    if (empty($form_state['values']['id_subscription'])) {
        form_set_error(
            'id_subscription', 
            t('Ingrese el ID de subscripcion'));
    }

}
function getSubList_validate($form, &$form_state){
    if (empty($form_state['values']['api_key'])) {
        form_set_error(
            'api_key', 
            t('Ingrese la Clave API de su aplicación')
        );
    }
    if (empty($form_state['values']['access_token'])) {
        form_set_error(
            'access_token', 
            t('Ingrese el Token de Acceso a la entidad para la 
            cual desea realizar las operaciónes')
        );
    }
}
function getSubList_submit($form, &$form_state){
  
    $apikey = $form_state['values']['api_key'];
    $token  = $form_state['values']['access_token'];
    
    $urlBase = 'https://api.mobbex.com/p/subscriptions';
    //$data = ['id' => 'NigTBaOGV'];
   // $data = array('uid' => 'UOM0Xtubq');
    $options = array(
        'method' => 'GET',
        //'data' => drupal_http_build_query($data),
        'headers' => [
            'x-api-key'      => $apikey,
            'x-access-token' => $token,
            'Content-Type'   => 'application/json',
        ],
    );
    $response = drupal_http_request($urlBase, $options);

    if (($response->code == 200) && ($response->status_message == 'OK')){
        
        $result =  json_decode($response->data, TRUE);
        $form_state['subsList'] = drawSubsList($result);
    }
    $form_state['rebuild'] = TRUE;
}


function SaveConfig_submit($form, &$form_state){
    variable_set(
        'mobbexsubscription_apikey', 
        $form_state['values']['api_key']
    );
    variable_set(
        'mobbexsubscription_token' , 
        $form_state['values']['access_token']
    );
    variable_set(
        'mobbexsubscription_idsubs', 
        $form_state['values']['id_subscription']
    );
    variable_set(
        'mobbexsubscription_activerole' , 
        $form_state['values']['role_subscriber_active']
    );

    $idSubscription = $form_state['values']['id_subscription'];
    $urlBase = 'https://api.mobbex.com/p/subscriptions/'.$idSubscription;
    $options = array(
        'method' => 'GET',
        'data' => null,
        'headers' => [
            'x-api-key'      => $form_state['values']['api_key'],
            'x-access-token' => $form_state['values']['access_token'],
            'Content-Type'   => 'application/json',
        ],
    );
    $response = drupal_http_request($urlBase, $options);

    if (($response->code == 200) && ($response->status_message == 'OK')){
        
        $result =  json_decode($response->data, TRUE);

        if( $result['result'] && ($result['data']['uid'] == $idSubscription)){
            //"shorten_url": "http://mbbx.co/c3VHJsG",
            variable_set(
                'mobbexsubscription_shorten_url' , 
                $result['data']['shorten_url']
            );
            drupal_set_message('Ok, la url de suscripcion es '.$result['data']['shorten_url']);
        }else{
            variable_set(
                'mobbexsubscription_shorten_url' , 
                0
            );
            drupal_set_message('No exixte la suscripcion seleccionada');
        }
    }
}
function getDataConfig(){
    return [
        'apikey'           => variable_get('mobbexsubscription_apikey', 0),
        'token'            => variable_get('mobbexsubscription_token', 0),
        'idsub'            => variable_get('mobbexsubscription_idsubs', 0),
        'activerole'       => variable_get('mobbexsubscription_activerole', 0),
    ];
}

function drawSubsList($result) {
    if(!$result['result']) return null;
    $aux = 0;
    $tabla = [];
    foreach ($result['data']['docs'] as $valor){
        //drupal_set_message(print_r($valor, TRUE));
        $tabla[$aux]['UID']         = $valor['uid'];
        $tabla[$aux]['NAME']        = $valor['name'];
        $tabla[$aux]['DESCRIPTION'] = $valor['description'];
        $tabla[$aux]['INTERVAL']    = $valor['interval'];
        $tabla[$aux]['COUNT']       = $valor['count'];
        $tabla[$aux]['TOTAL']       = $valor['currency'].' $'.$valor['total'];
        $tabla[$aux]['URL']         = $valor['url'];
        $aux ++;        
    }
    $header = [t('UID'),t('NOMBRE'),t('DESCRIPCIÓN'),t('INTERVALO'),t('SUSCRIPTORES'),t('TOTAL'),t('URL SUBSCRIPCIÓN') ];

    if(count($tabla)==0)
        return 'NO SE ENCONTRARON DATOS';

    return  theme('table', array('header' => $header, 'rows' => $tabla));
}