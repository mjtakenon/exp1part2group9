
function xhrHandler(xhr) 
{
    switch ( xhr.readyState ) 
    {
        case 0:
            // 未初期化状態.
            console.log( 'uninitialized!' );
            break;
        case 1: // データ送信中.
            console.log( 'loading...' );
            break;
        case 2: // 応答待ち.
            console.log( 'loaded.' );
            break;
        case 3: // データ受信中.
            console.log( 'interactive... '+xhr.responseText.length+' bytes.' );
            break;
        case 4: // データ受信完了.
            if( xhr.status == 200 || xhr.status == 304 ) {
                var data = xhr.responseText; // responseXML もあり
                console.log( 'COMPLETE! :' + data );
                document.writeln('COMPLETE! :' + data);
            } else {
                console.log( 'Failed. HttpStatus: '+xhr.statusText );
                document.writeln( 'Failed. HttpStatus: '+xhr.statusText );
            }
            break;
    }
};

function getImage(){

    alert("push to ajax start.");
    console.log("push to ajax start.");
    
    var xhr = new XMLHttpRequest();
    // ハンドラの登録.
    xhr.onreadystatechange = xhrHandler(xhr);

    hostname = window.location.href.split("/");
    //var hostpage = "http://" + hostname[2] + "/ajaxtest.php\n";
    //var hostpage = window.location.href;
    //var host = "http://" + hostname[2] + "ajaxtest.php";
    var host = "http://localhost:15503/ajaxtest.php";
    document.writeln("hostname = " + host);
    
    //xhr.open( 'POST', hostpage, false );
    // POST 送信の場合は Content-Type は固定.
    //xhr.setRequestHeader( 'Content-Type', 'application/x-www-form-urlencoded' );
    //xhr.send( 'アイスティーしかなかったけど、いいかな?\n' );

    xhr.open( 'get', host, false );
    xhr.send(null);

    xhr.abort(); // 再利用する際にも abort() しないと再利用できないらしい.
    
}
