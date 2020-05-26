<?php

function mobbexsubscription_entry_insert($entry) {
    $return_value = NULL;
    try {
        $fieldkeys = array_keys($entry);
        $fieldvalues = array_values($entry);

        $return_value = db_insert('mobbexsubscription')
                        ->fields($fieldkeys)
                        ->values($fieldvalues)
                        ->execute();
    }
    catch (Exception $e) {
        drupal_set_message(t('db_insert failed. Message = %message, query= %query',
        array('%message' => $e->getMessage(), '%query' => $e->query_string)), 'error');
    }
    return $return_value;
}

function mobbexsubscription_entry_update($entry) {
    if(!isset($entry['updated'])) $entry['updated'] = date("Y-m-d H:m:s");
    
    try {
        // db_update()...->execute() returns the number of rows updated.
        $count = db_update('mobbexsubscription')
                ->fields($entry)
                ->condition('uid',             $entry['uid'])
                ->condition('subscription_id', $entry['subscription_id'])
                ->condition('subscriber_id',   $entry['subscriber_id'])
                ->execute();
    }
    catch (Exception $e) {
        drupal_set_message(t('db_update failed. Message = %message, query= %query',
        array('%message' => $e->getMessage(), '%query' => $e->query_string)), 'error');
    }
    return $count;
}
    