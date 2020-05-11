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
    $activerole = variable_get('mobbexsubscription_activerole', 0);
    if (isset($_GET['retorno']) && $_GET['retorno'] == 'mobbex') {
        //primera vez 
        if(is_current_user_suscriber()){
            $form['welcome'] = [
                '#markup' => '<div id="first-time-suscribe"> Gracias por suscribirte </div>'
            ];
        }else{
            $form['welcome'] = [
                '#markup' => '<div id="first-time-suscribe"> Al parecer la suscripción no se concretó, pero pueder volver a intentar en otro momento.  </div>'
            ];
            return $form;
        }
      }

    if(isset($roles[$activerole])){
        //el usuario tiene una suscripcion activa

        $form['subscription_info'] = [
            '#markup' => 'periodo, prox vec, darse de baja'
        ];
        return $form;
    }
}

function is_current_user_suscriber(){
    //retorna el id de suscriptor si es correcto
    //retorna false si no es un suscriptor
    return true;
}