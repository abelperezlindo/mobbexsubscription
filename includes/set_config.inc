<?php
/**
 * @file
 * Contiene formulario de configuraciones
 *
 * Implements hook_form()
 *
 *    Formulario para alta para configuraciones de la subscripción
 *
 */
return 0;
 
function mobbexsubscription_config_form($form, &$form_state)
{

    if (!function_exists('curl_version')) {
        drupal_set_message(t('CURL NO INSTALADO'), 'error');
    }
    if(isset($form_state['config'])){
        $config = $form_state['config'];
        $config['idsub'] = variable_get('mobbexsubscription_idsubs', 0);
    }
    else {
        $config = getDataConfig();
    }

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
    if (isset($form_state['subsList'])) {
        $form['subscription_config']['List'] = array(
            '#markup' => $form_state['subsList'],
        );
    }
    $form['subscription_config']['createSubscription'] = array(
        '#markup' => l(
            t('Crear nueva suscripción'),
            'admin/config/mobbexsubscription/nueva-suscripcion',
            ['attributes' => ['class' => 'createSubscription-link']]
        ),
    );
    $form['subscription_config']['getSubList'] = array(
        '#type' => 'submit',
        '#name' => 'getSubList',
        '#value' => t('Listar suscripciones'),
        '#validate' => ['getSubList_validate'],
        '#submit' => ['getSubList_submit'],
    );
    $form['save'] = array(
        '#type' => 'submit',
        '#name' => 'SaveConfig',
        '#value' => t('Guardar las configuraciones'),
        '#validate' => ['SaveConfig_validate'],
        '#submit' => ['SaveConfig_submit'],
    );
    return $form;
}
/**
 * Validación principal del Formulario.
 */
function SaveConfig_validate($form, &$form_state)
{
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
/**
 * Validación para obtener listado de suscripciones
 */
function getSubList_validate($form, &$form_state)
{
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
/**
 * Envío de Obtener listado de suscripciones.
 */
function getSubList_submit($form, &$form_state)
{

    $apikey = $form_state['values']['api_key'];
    $token = $form_state['values']['access_token'];

    $mbx = new Moxy($apikey, $token);

    $result = $mbx->getSubscriptions();
    $form_state['subsList'] = drawSubsList($result);
    $form_state['rebuild'] = true;
    $form_state['config'] = [
        'apikey' => $apikey,
        'token'  => $token,
    ];
}

/**
 * Envío principal del Formulario.
 */
function SaveConfig_submit($form, &$form_state)
{
    $apikey = $form_state['values']['api_key'];
    $token = $form_state['values']['access_token'];
    $subscription = $form_state['values']['id_subscription'];

    variable_set('mobbexsubscription_apikey', $apikey);
    variable_set('mobbexsubscription_token' , $token);
    variable_set('mobbexsubscription_idsubs', $subscription);

    $mbx = new Moxy($apikey, $token);
    $result = $mbx->getSubscriptions($subscription);
    if (empty($result)) {
        variable_set(
            'mobbexsubscription_shorten_url',
            0
        );
        drupal_set_message('No exixte la suscripcion seleccionada');

    } elseif ($result['result'] && ($result['data']['uid'] == $subscription)) {
        variable_set(
            'mobbexsubscription_shorten_url',
            $result['data']['shorten_url']
        );
        drupal_set_message('Ok, la url de suscripcion es ' . $result['data']['shorten_url']);
    }
}
function getDataConfig()
{
    return [
        'apikey' => variable_get('mobbexsubscription_apikey', 0),
        'token' => variable_get('mobbexsubscription_token', 0),
        'idsub' => variable_get('mobbexsubscription_idsubs', 0),
    ];
}

function drawSubsList($result)
{
    if (!$result['result']) {
        return null;
    }

    $aux = 0;
    $tabla = [];
    foreach ($result['data']['docs'] as $valor) {

        $tabla[$aux]['UID'] = $valor['uid'];
        $tabla[$aux]['NAME'] = $valor['name'];
        $tabla[$aux]['DESCRIPTION'] = $valor['description'];
        $tabla[$aux]['INTERVAL'] = $valor['interval'];
        $tabla[$aux]['COUNT'] = $valor['count'];
        $tabla[$aux]['TOTAL'] = $valor['currency'] . ' $' . $valor['total'];
        $tabla[$aux]['URL'] = $valor['url'];
        $aux++;
    }
    $header = [
        t('UID'),
        t('NOMBRE'),
        t('DESCRIPCIÓN'),
        t('INTERVALO'),
        t('SUSCRIPTORES'),
        t('TOTAL'),
        t('URL SUBSCRIPCIÓN'),
    ];

    if (count($tabla) == 0) {
        return 'NO SE ENCONTRARON DATOS';
    }

    return theme('table', array('header' => $header, 'rows' => $tabla));
}
