<?php

/**
 * @file
 * Comunicación con la base de datos 
 *
 * Inserta un nuevo registro en la base de datos.
 *
 * @param mixed $entry
 *     Array con los valores a guardar
 * @return int  
 *     The number of rows updated.
 */
function mobbexsubscription_entry_insert($entry)
{
    $isSub =  mobbexsubscription_entry_get($entry['uid']);
    if (!empty($isSub)) return 0;

    $return_value = NULL;
    $transaction = db_transaction();
    try {
        $fieldkeys = array_keys($entry);
        $fieldvalues = array_values($entry);

        $return_value = db_insert('mobbexsubscription')
            ->fields($fieldkeys)
            ->values($fieldvalues)
            ->execute();
    } catch (Exception $e) {
        $transaction->rollBack();
        drupal_set_message(t(
            'db_insert failed. Message = %message, query= %query',
            array('%message' => $e->getMessage(), '%query' => $e->query_string)
        ), 'error');
    }
    return $return_value;
}
/**
 * Actualiza un registro en la base de datos.
 *
 * @param mixed $entry
 *     Array con los valores a guardar
 * @return int
 *     The number of rows updated.
 */
function mobbexsubscription_entry_update($entry)
{
    if (!isset($entry['updated'])) $entry['updated'] = date("Y-m-d H:m:s");
    $transaction = db_transaction();
    try {
        // db_update()...->execute() returns the number of rows updated.
        $count = db_update('mobbexsubscription')
            ->fields($entry)
            ->condition('uid',             $entry['uid'])
            ->condition('subscription_id', $entry['subscription_id'])
            ->condition('subscriber_id',   $entry['subscriber_id'])
            ->execute();
    } catch (Exception $e) {
        $transaction->rollBack();
        drupal_set_message(t(
            'db_update failed. Message = %message, query= %query',
            array('%message' => $e->getMessage(), '%query' => $e->query_string)
        ), 'error');
    }
    return $count;
}

/**
 * Obtiene un registro en la base de datos, o todos lo s que coinsidan con
 * el user id.
 *
 * @param int $uid
 *     Id de usuario drupal
 * @return mixed
 *     respuesta o cero.
 */
function mobbexsubscription_entry_get($uid)
{
    $result = db_select('mobbexsubscription', 'mbx')
        ->fields('mbx')
        ->condition('uid', $uid, '=')
        ->execute()
        ->fetchAll();

    if (empty($result)) return 0;
    if (count($result) == 1) return $result[0];
    return $result;
}
/**
 * Obtiene un registro en la base de datos, o todos lo s que coinsidan con
 * el user id.
 *
 * @param int $uid
 *     Id de usuario drupal
 * @return mixed
 *     respuesta o cero.
 */
function mobbexsubscription_entry_getall()
{
    $result = db_select('mobbexsubscription', 'mbx')
        ->fields('mbx')
        ->execute()
        ->fetchAll();

    if (empty($result)) return 0;
    if (count($result) == 1) return $result[0];
    return $result;
}
/*
* @param mixed $entry
*     Array con los valores a guardar
* @return int  
*     The number of rows updated.
*/
function mobbexexecution_entry_insert($entry)
{

    $return_value = NULL;
    $transaction = db_transaction();
    try {
        $fieldkeys = array_keys($entry);
        $fieldvalues = array_values($entry);

        $return_value = db_insert('mobbexexecution')
            ->fields($fieldkeys)
            ->values($fieldvalues)
            ->execute();
    } catch (Exception $e) {
        $transaction->rollBack();
        drupal_set_message(t(
            'db_insert failed. Message = %message, query= %query',
            array('%message' => $e->getMessage(), '%query' => $e->query_string)
        ), 'error');
    }
    return $return_value;
}

function mobbexexecution_entry_get()
{
    $result = db_select('mobbexexecution', 'mbx')
        ->fields('mbx')
        ->execute()
        ->fetchAll();
    if (empty($result)) return 0;
    return $result;
}
