<?php
function chatserval($userid,$text,$initflag){

        //$api_key = '6979596e473472664c4d323264436f38564b52346d5449636a2f6a67544c53465478455167725957575139';
        $api_url = sprintf('https://api.repl-ai.jp/v1/dialogue');

        $reqbody = array('botId' => 'tukutter',
                        'appUserId' => $userid,
                        'voiceText' => $text,
                        'initTalkingFlag' => $initflag,
                        'initTopicId' => 'tujou',


        );

        $header = array(
            'Content-Type: application/json; charset=UTF-8',
            'x-api-key: LcKqv83bEd9ULbxajuAxd3IeaWjbSk7vaL9vH36Z',
        );
        $option = array(
            'http' =>array(
                'method' => 'POST',
                'header' => implode("\r\n",$header),
                'content'=> json_encode($reqbody),
            )
        );

        $stream = stream_context_create($option);


        $json = json_decode(file_get_contents($api_url,false,$stream),true);

        //print_r($json);

        echo $json['systemText']['expression'];

}
if (isset($argv[3])) {
    chatserval($argv[1],$argv[2],$argv[3]);
}else {
    echo "ba-ka";
}

?>
