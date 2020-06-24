<?php

function executions_form($form, &$formstate){
    $currentUser = $GLOBALS['user'];
    $form['contenedor'] = [
        '#type' => 'fieldset'
    ];
    $role    = user_role_load_by_name('administrator'); //OR role for admin this
    $isAdmin = user_has_role($role->rid, $currentUser);

    if(!$isAdmin){
        
        $form['contenedor']['mensage'] = [
            '#markup' => 'Usuario sin permiso suficiente'
        ];
        return $form;
    }
    $form['contenedor']['message'] = [
        '#markup' =>  'Las ejecuciones se ralizan automaticamente por agenda si la suscripcion es del tipo dinamica.',
    ];
    $executions = mobbexexecution_entry_get();
    if(!empty($executions)){

        $form['contenedor']['table'] = [
            '#markup' =>  markup_table_executions(),
        ];
    }
    
    return $form;
}

function markup_table_executions($filters = null){
    $executions = mobbexexecution_entry_get();
    if(empty($executions)) return 'NO SE ENCONTRARON DATOS QUE MOSTRAR';

    $content = [];
    for($i = 0 ; $i < count($executions) ; $i++){

        $content[$i]['uid']             = $executions[$i]->uid;
        $content[$i]['subscription_id'] = $executions[$i]->subscription_id;
        $content[$i]['subscriber_id']   = $executions[$i]->subscriber_id;
        $content[$i]['description']     = $executions[$i]->description;
        $content[$i]['status']          = $executions[$i]->status;
        $content[$i]['total']           = '$'.$executions[$i]->total;
        $content[$i]['datetime']        = $executions[$i]->datetime;
        $content[$i]['reference']       = $executions[$i]->reference;

    }
    $header = [
        t('ID de usuario'),
        t('Mobbex Suscripción'),
        t('Mobbex subscriptor'),
        t('Descripción de la ejecucion'),
        t('Estado'), 
        t('Monto'),
        t('Fecha'),
        t('Referencia de Ejecucion'),
        #['data'=>t('Operaciones'), 'colspan'=>1]
    ];

    return theme('table',['header' => $header, 'rows' => $content]);
}