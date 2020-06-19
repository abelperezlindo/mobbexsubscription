<?php

class Moxy
{

    private $apikey;
    private $accesstoken;

    public function __construct($apikey, $accesstoken)
    {
        if (!empty($apikey)) {
            $this->apikey = $apikey;
        }

        if (!empty($accesstoken)) {
            $this->accesstoken = $accesstoken;
        }

    }

    public function getSubscriptions($idSuscription = '')
    {

        if (!function_exists('curl_version')) {
            return 0;
        }

        $urlBase = 'https://api.mobbex.com/p/subscriptions';
        if (!empty($idSuscription)) {
            $urlBase = $urlBase . '/' . $idSuscription;
        }

        $curl    = curl_init();
        $options = [
            CURLOPT_URL => $urlBase,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "GET",
            CURLOPT_HTTPHEADER => [
                "x-api-key: " . $this->apikey,
                "x-access-token: " . $this->accesstoken,
            ],
        ];
        curl_setopt_array($curl, $options);

        $response       = curl_exec($curl);
        $responseDecode = json_decode($response, true);
        curl_close($curl);

        if ($responseDecode['result'] != 'true') {
           return 0;
        }

        return $responseDecode;

    }

    public function setNewSuscription($data)
    {
        if (!function_exists('curl_version')) {
            return 0;
        }

        if (empty($data) || !is_array($data)) {
            return 0;
        }

        $curl = curl_init();
        $options = [
            CURLOPT_URL => "https://api.mobbex.com/p/subscriptions",
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "POST",
            CURLOPT_POSTFIELDS => json_encode($data),
            CURLOPT_HTTPHEADER => [
                "x-api-key: " . $this->apikey,
                "x-access-token: " . $this->accesstoken,
                "Content-Type: application/json",
                "Content-Type: text/plain",
            ],
        ];
        curl_setopt_array($curl, $options);
        $response = curl_exec($curl);
        if (curl_errno($curl)) {
            echo 'Curl error: ' . curl_error($curl);
            curl_close($curl);
            return 0;
        }

        curl_close($curl);
        return json_decode($response, true);

    }
    public function setSubscriber($idSuscription, $data){

        if (!function_exists('curl_version')) {
            return 0;
        }
        if(empty($idSuscription) || empty($data)){
            return 0;
        }
        $urlBase = 'https://api.mobbex.com/p/subscriptions/';
        $urlBase .= $idSuscription;
        $urlBase .= '/subscriber';
        $curl    = curl_init();
        $options = [
            CURLOPT_URL => $urlBase,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "POST",
            CURLOPT_POSTFIELDS => json_encode($data, TRUE),
            CURLOPT_HTTPHEADER => [
                "x-api-key: "      . $this->apikey,
                "x-access-token: " . $this->accesstoken,
                "Content-Type: application/json"
            ],
        ];

        curl_setopt_array($curl, $options);
        $response = curl_exec($curl);

        if (curl_errno($curl)) {
            echo 'Curl error: ' . curl_error($curl);
            curl_close($curl);
            return 0;
        }

        curl_close($curl);
        return json_decode($response, true);

    }
    public function getSubscriber($idSubscription,  $idSubscriber){
        
        if (!function_exists('curl_version')) {
            return 0;
        }
        if(empty($idSubscription) || empty($idSubscriber)) return 0;
        $baseUrl = 'https://api.mobbex.com/p/subscriptions/';
        $baseUrl .= $idSubscription;
        $baseUrl .= '/subscriber/';
        $baseUrl .= $idSubscriber;



        $curl = curl_init();
        $options = [
            CURLOPT_URL => $baseUrl,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "GET",
            CURLOPT_HTTPHEADER => [
                "x-api-key: "      . $this->apikey,
                "x-access-token: " . $this->accesstoken,
            ],
            
        ];

        curl_setopt_array($curl, $options);

        $response = curl_exec($curl);
        if (curl_errno($curl)) {
            echo 'Curl error: ' . curl_error($curl);
            curl_close($curl);
            return 0;
        }
        $responseDecode = json_decode($response, true);
        curl_close($curl);

        if ($responseDecode['result'] != 'true') {
           return 0;
        }

        return $responseDecode;

    }

}


class MoxyWebhook
{
    private $postdata;
    public function __construct()
    {
        $this->postdata = file_get_contents("php://input");
    
    }

    public function getPostString()
    {
        if (empty($this->postdata)) {
            return 0;
        }

        return $this->postdata;
    }
    public function getPostArray($option = null)
    {
        if (empty($this->postdata)) {
            return 0;
        }
        $obj = [];
        parse_str($this->postdata, $obj);
        
        if(is_null($option)){
            return $obj;
        }
        elseif($option == 'data'){
            return $obj['data'];
        }
        elseif($option == 'type'){
            return $obj['type'];
        }
    }



}