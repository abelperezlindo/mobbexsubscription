<?php

function test_form($form, &$form_state) {
    $uid = $GLOBALS['user']->uid;
    // Load full user account object
    $account = user_load($uid);
    

    $auxArr = [LANGUAGE_NONE => ['0' => ['value' => 'valore', 'format' => '', 'safe_value'=> '']]];
    $account->field_mobbex_subscription_uid = $auxArr;
    $account->field_mobbex_subscriber_uid = [];

    // Get field;
    $items = field_get_items('user', $account, 'field');
    // Debug info
    drupal_set_message('<pre>'.print_r($items, TRUE).'</pre>');
    // This gets the sanitized output, from the first field delta value (0)
    $output = field_view_value('user', $account, 'field_node', $items[0]);
    drupal_set_message('<pre>'.print_r($output, TRUE).'</pre>');
    user_save($account);
    drupal_set_message('<pre>'.print_r($account, TRUE).'</pre>');

    return $form; 
}