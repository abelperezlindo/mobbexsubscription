<?php
/**
 * @file 
 * Obtiene la llamada desde Mobbex
 */

include "data_access.inc";
 // Query params from POST.

/**
 * Implements hook_form().
 *      Formulario caprura json (prox retirar form)
 */
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
/**
 *  Recibe post data de mobbex y realiza el procesamiento requerido para el tipo
 * @param mixed $obj
 *      post data
 */
function manageWebhookRequest($obj){
    switch($obj['type']){
        case "subscription:registration" : 
            
            /*
             {
                    "type": "subscription:registration",
                    "data": {
                        "payment": {
                            "id": "rkWpX3YU6x",
                            "operation": {
                                "type": "payment"
                            },
                            "status": {
                                "code": "200",
                                "text": "Paid",
                                "message": "APROBADA"
                            },
                            "total": 1907.5,
                            "currency": {
                                "code": "ARS",
                                "text": "Peso Argentino"
                            },
                            "currency_data": {
                                "value": "ARS",
                                "label": "Peso Argentino",
                                "symbol": "$",
                                "hidden": false
                            },
                            "source": {
                                "name": "Visa",
                                "type": "card",
                                "number": "454642xxxxxx7787",
                                "installment": {
                                    "description": "1 Cuotas",
                                    "amount": 500,
                                    "count": 1
                                }
                            },
                            "user": {
                                "name": "Juan Batman",
                                "email": "soybatman@gmail.com"
                            },
                            "reference": "HyjdiK8Tx"
                        },
                        "subscription": {
                            "uid": "JmfI3Ml3D",
                            "interval": "1m",
                            "limit": 0,
                            "name": "Nombre de la Suscripción",
                            "description": "Descripción de la Suscripción",
                            "total": 500
                        },
                        "subscriber": {
                            "uid": "qy82noStM",
                            "reference": "qy82noStM",
                            "customer": {
                                "name": "Juan Batman",
                                "identification": "999999",
                                "email": "soybatman@gmail.com"
                            }
                        }
                    }
                }
             */
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

        // SEPARADOR AUX //

        case "subscription:source_change" :
            /*
            {
                "type": "subscription:source_change",
                "data": {
                    "payment": {
                        "id": "rkWpX3YU6x",
                        "operation": {
                            "type": "payment"
                        },
                        "status": {
                            "code": "200",
                            "text": "Paid",
                            "message": "APROBADA"
                        },
                        "total": 1907.5,
                        "currency": {
                            "code": "ARS",
                            "text": "Peso Argentino"
                        },
                        "currency_data": {
                            "value": "ARS",
                            "label": "Peso Argentino",
                            "symbol": "$",
                            "hidden": false
                        },
                        "source": {
                            "name": "Visa",
                            "type": "card",
                            "number": "454642xxxxxx7787",
                            "installment": {
                                "description": "1 Cuotas",
                                "amount": 500,
                                "count": 1
                            }
                        },
                        "user": {
                            "name": "Juan Batman",
                            "email": "soybatman@gmail.com"
                        },
                        "reference": "HyjdiK8Tx"
                    },
                    "subscription": {
                        "uid": "JmfI3Ml3D",
                        "interval": "1m",
                        "limit": 0,
                        "name": "Nombre de la Suscripción",
                        "description": "Descripción de la Suscripción",
                        "total": 500
                    },
                    "subscriber": {
                        "uid": "qy82noStM",
                        "reference": "qy82noStM",
                        "customer": {
                            "name": "Juan Batman",
                            "identification": "999999",
                            "email": "soybatman@gmail.com"
                        }
                    }
                }
            }
            */
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

        // SEPARADOR AUX //


        case "subscription:execution" : 
            //Suscripción Ejecutada

            /*
            {
                "type": "subscription:execution",
                "data": {
                    "result": true,
                    "payment": {
                        "id": "rkWpX3YU6x",
                        "operation": {
                            "type": "payment"
                        },
                        "status": {
                            "code": "200",
                            "text": "Paid",
                            "message": "APROBADA"
                        },
                        "total": 1907.5,
                        "currency": {
                            "code": "ARS",
                            "text": "Peso Argentino"
                        },
                        "currency_data": {
                            "value": "ARS",
                            "label": "Peso Argentino",
                            "symbol": "$",
                            "hidden": false
                        },
                        "source": {
                            "name": "Visa",
                            "type": "card",
                            "number": "454642xxxxxx7787",
                            "installment": {
                                "description": "1 Cuotas",
                                "amount": 500,
                                "count": 1
                            }
                        },
                        "user": {
                            "name": "Juan Batman",
                            "email": "soybatman@gmail.com"
                        },
                        "reference": "HyjdiK8Tx"
                    },
                    "subscription": {
                        "uid": "JmfI3Ml3D",
                        "interval": "1m",
                        "limit": 0,
                        "name": "Nombre de la Suscripción",
                        "description": "Descripción de la Suscripción",
                        "total": 500
                    },
                    "subscriber": {
                        "uid": "qy82noStM",
                        "reference": "qy82noStM",
                        "period": 3,
                        "customer": {
                            "name": "Juan Batman",
                            "identification": "999999",
                            "email": "soybatman@gmail.com"
                        }
                    }
                }
            }
            */

            #comprobamos si la transaccion afecta al estado actual de un suscriptor

        break;

        // SEPARADOR AUX //


        case "subscription:subscriber:suspended" : 
            //Suscriptor Suspendido

            /*
            {
                "type": "subscription:subscriber:suspended",
                "data": {
                    "subscription": {
                        "uid": "JmfI3Ml3D",
                        "interval": "1m",
                        "limit": 0,
                        "name": "Nombre de la Suscripción",
                        "description": "Descripción de la Suscripción",
                        "total": 500
                    },
                    "subscriber": {
                        "uid": "qy82noStM",
                        "reference": "qy82noStM",
                        "customer": {
                            "name": "Juan Batman",
                            "identification": "999999",
                            "email": "soybatman@gmail.com"
                        }
                    }
                }
            }
            */
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


        // SEPARADOR AUX //



        case "subscription:subscriber:active" : 
            //Suscriptor Activado

            /*
            {
                "type": "subscription:subscriber:active",
                "data": {
                    "subscription": {
                        "uid": "JmfI3Ml3D",
                        "interval": "1m",
                        "limit": 0,
                        "name": "Nombre de la Suscripción",
                        "description": "Descripción de la Suscripción",
                        "total": 500
                    },
                    "subscriber": {
                        "uid": "qy82noStM",
                        "reference": "qy82noStM",
                        "customer": {
                            "name": "Juan Batman",
                            "identification": "999999",
                            "email": "soybatman@gmail.com"
                        }
                    }
                }
            }
            */
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
