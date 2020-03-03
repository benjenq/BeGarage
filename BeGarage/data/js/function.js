let BeGarageDoor_mDNS = 'begarage.local';
let title = '車庫遙控器';
let pressDurationMs = 300;
//按鈕
$("button").click(function () {
    var thisbtn =  $(this);
    var buttonname = thisbtn.attr('buttonname');
    pushButton('http://'+BeGarageDoor_mDNS + '/button',buttonname);
    thisbtn.prop('disabled',true);
    setTimeout(function(){
        thisbtn.prop('disabled',false);            
    },pressDurationMs);
});
/* API 與函數 */
function pushButton(apiutl,buttonname){
    var para = "action=".concat(buttonname);
    $.ajax({
        url: apiutl,
        method: 'GET',
        cache: false,
        data: para,
        dataType: 'json',
        timeout: 1000,
        success: ajaxSuccess
    }).done(function (result) {
        if (console && console.log) {
            //console.log("Sample of data:", result);
            //console.log("Done: Sample of data:", result.slice(0, 100));
            //ajaxSuccess(data);
        }
    }).always(function () {


    }).statusCode({
        404: function () {
            alert('404: Not Found');
        },
        403: function () {
            alert('403: Forbidden');
        }

    }).fail(function (jqXHR, statustext, message) {
        alert('fail: |' + jqXHR.status + '|' + statustext + '|' + message);
    });
}

function ajaxSuccess(data) {
    console.log(data);
    if(data.code <0){
        console.log('Error:' .concat(data.result));
        $(".resultstring").text(data.result);
        return;
    }
    $(".resultstring").text(data.result);
    $(".resultstring").fadeIn(100);
    setTimeout(function(){
        $(".resultstring").fadeOut(250);
    },2000);
}

function getmcuname(apiurl){
    $.ajax({
        url: apiurl,
        method: 'GET',
        cache: false,
        dataType: 'json',
        timeout: 1000,
        success: ajaxSuccess
    }).done(function (result) {
        if (console && console.log) {
            //console.log(result.result);
            var newtitle = title.concat(' (').concat(result.result).concat(')');
            $('title').text(newtitle);
            $('#title').text(newtitle);
        }
    }).always(function () {


    }).statusCode({
        404: function () {
            alert('404: Not Found');
        },
        403: function () {
            alert('403: Forbidden');
        }

    }).fail(function (jqXHR, statustext, message) {
        alert('fail: |' + jqXHR.status + '|' + statustext + '|' + message);
    });
    
}

$(document).ready(function () {
    getmcuname('http://'.concat(BeGarageDoor_mDNS).concat('/mcuname'));
    //getTempHumi();
    //getacStstus();
});
