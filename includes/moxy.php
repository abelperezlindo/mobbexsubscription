<?php

class Moxy
{

    private $postdata;
    private $apikey;
    private $accesstoken;
    private $typesWithPayment = [
        "subscription:registration",
        "subscription:source_change",
        "subscription:execution",
    ];
    public function __construct()
    {
        $params = func_get_args();
        //saco el número de parámetros que estoy recibiendo
        $num_params = func_num_args();
        if ($num_params == 2 && method_exists($this, "__moxy")) {
            //si existía esa función, la invoco, reenviando los parámetros que recibí en el constructor original
            call_user_func_array(array($this, "__moxy"), $params);
        }
    }
    public function __moxy($apikey, $accesstoken)
    {
        if (!empty($apikey)) {
            $this->apikey = $apikey;
        }

        if (!empty($accesstoken)) {
            $this->accesstoken = $accesstoken;
        }

    }
    public function setPost($post = null)
    {
        if ($post) {
            $this->postdata = $post;
        } else {
            $this->postdata = file_get_contents("php://input");
        }

    }
    public function setApiKey($apikey)
    {
        if (!empty($apikey)) {
            $this->apikey = $apikey;
        }

    }
    public function setAccessToken($accessToken)
    {
        if (!empty($accessToken)) {
            $this->accessToken = $accessToken;
        }

    }
    public function getPostString()
    {
        if (empty($this->postdata)) {
            return 0;
        }

        return $this->postdata;
    }
    public function getPostArray()
    {
        if (empty($this->postdata)) {
            return 0;
        }

        $obj = [];
        parse_str($this->postdata, $obj);
        return $obj;
    }
    public function proccessPost()
    {
        $obj = $this->getPostArray();

        if (in_array($obj['type'], $this->typesWithPayment)) {

            if (!isset($obj['data']) || $obj['data']['status'] != 200) {
                return false;
            }

            if ($obj['data']['payment']['operation']['type'] == 'payment') {
                $this->getPaymentData($obj['data']['payment']['id']);
            }
            return;
        }
        if ($obj['type'] == "") {
            return;
        }
        if ($obj['type'] == "") {
            return;
        }
    }
    public function getPaymentData($idPayment)
    {
        if (!function_exists('curl_version') || empty($idPayment)) {
            return 0;
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

        $curl = curl_init();

        curl_setopt_array($curl, array(
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
        ));

        $response = curl_exec($curl);
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
        $urlBase -= '/subscriber';
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
