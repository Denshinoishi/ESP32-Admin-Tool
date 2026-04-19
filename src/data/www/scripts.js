//---------------------------------------
//----------Persistencia MQTT----------------
//---------------------------------------
function mqtt() {
    var mqtt_en = document.getElementById("mqtten");
    if (mqtt_en.value === "1") {
        formDisableFalse("mqtt");
    } else {
        formDisableTrue("mqtt");
    }
}
//---------------------------------------
//----------Hablitar input WIFI/AP-------
//---------------------------------------
function wifi(){
    var ip = document.getElementById("wifi_staticIPen");
    var ap = document.getElementById("ap_AP_en");
    if (ip.value === "1") {
        formDisableFalse("ip");
    } else {
        formDisableTrue("ip");
    }

    if (ap.value === "1") {
        formDisableFalse("ap");
    } else {
        formDisableTrue("ap");
    }
}


//--------------------------------------- 
//---------Habilitar MQTT----------------
//---------------------------------------
function mqttenable() {
    if (document.getElementById("mqtt_enable").checked) {
        document.getElementById("mqtten").value = "1";
        formDisableFalse("mqtt");
} else {
        document.getElementById("mqtten").value = "0";
        formDisableTrue("mqtt");
}
}

//---------------------------------------
//---------Habilitar IP Estática---------
//---------------------------------------
function staticIPen() {
    if (document.getElementById("wifi_staticIP").checked) {
        document.getElementById("wifi_staticIPen").value = 1;
        formDisableFalse("ip");
    } else {
        document.getElementById("wifi_staticIPen").value = 0;
        formDisableTrue("ip");
    }
}
//---------------------------------------
//---------Habilitar Modo AP-------------
//---------------------------------------
function apenable() {
    if (document.getElementById("accessPoint").checked) {
        document.getElementById("ap_AP_en").value = 1;
        formDisableFalse("ap");
    } else {
        document.getElementById("ap_AP_en").value = 0;
        formDisableTrue("ap");
    }
}
//---------------------------------------
//---------Habilitar Visibilidad AP-------
//---------------------------------------
function visibilityen() {
    if (document.getElementById("ap_hiddenap").checked) {
        document.getElementById("ap_visibility").value = 0;
        
    } else {
        document.getElementById("ap_visibility").value = 1;
    }
}

//---------------------------------------
//---------Deshabilitar MQTT----------------
//---------------------------------------
function formDisableFalse(clase) {
    var elements = document.getElementsByClassName(clase);
    for (var i = 0; i < elements.length; i++){ 
        elements[i].disabled = false;
    }
}


//---------------------------------------
//---------Habilitar MQTT----------------
//---------------------------------------
function formDisableTrue(clase) {
    var elements = document.getElementsByClassName(clase);
    for (var i = 0; i < elements.length; i++) {
        elements[i].disabled = true;
    }
}

//---------------------------------------
//---------Limpiar Formulario------------
//---------------------------------------
function clearAdmin() {
    document.getElementById("formulario").reset();
}   