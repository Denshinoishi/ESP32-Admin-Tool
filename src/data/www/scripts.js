/*Capturar el url del navegador*/
const pathname = window.location.pathname;
/*Websocket*/
//var connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);
var connection = new WebSocket('ws://192.168.0.150/ws', ['arduino']);
/* Conexión Abierta */
connection.onopen = function () {
    console.log('Websocket Conectado');
};
/* Conexión Cerrada */
connection.onclose = function () {
    console.log('Websocket Desconectado');
};
/* Recibir Mensaje */
connection.onmessage = function (event) {
    let obj = JSON.parse(event.data);
    //console.log("Mensaje Recibido:", obj);
    processData(event.data);

};
/* Error en la Conexión */
connection.onerror = function (error) {
    console.log('Error en la Conexión: ' + error);
};
/ * Procesar Datos Recibidos */
function processData(data) {
    let jsonData = JSON.parse(data);
    console.log('Mensaje Recibido: ' + jsonData);
    if(jsonData.type === "info"){
        Swal.fire({
            position: 'top-end',
            icon: jsonData.icon,
            title: jsonData.texto,
            showConfirmButton: false,
            timer: 2000
        });
    }
    if(pathname === "/"){
        if(jsonData.type === "data"){
            console.log("Datos Recibidos Procesando:", jsonData);
            document.getElementById("wifi_status").innerHTML = jsonData.wifi_status;
            document.getElementById("mqtt_status").innerHTML = jsonData.mqtt_status;
            document.getElementById("mqtt_on").innerHTML = jsonData.mqtt_on;
            document.getElementById("mqtt_server").innerHTML = jsonData.mqtt_server;
            document.getElementById("wifi_dmb").innerHTML = jsonData.wifi_dbm + " dBm";
            document.getElementById("dbm_label").innerHTML = jsonData.wifi_dbm + " dBm";
            document.getElementById("wifi_percent").innerHTML = jsonData.wifi_percent + " %";
            //Progress Bar
            document.getElementById("wifi_Signal").style.width = jsonData.wifi_percent + "%";
            document.getElementById("wifi_label").innerHTML = jsonData.wifi_percent + " %";
            document.getElementById("spiffs_Signal").style.width = jsonData.spiffs_used + "%";
            document.getElementById("spiffs_label").innerHTML = jsonData.spiffs_used + " %";
            document.getElementById("ram_Signal").style.width = jsonData.ram_available + "%";
            document.getElementById("ram_label").innerHTML = jsonData.ram_available + " %";

            document.getElementById("temp_cpu").innerHTML = jsonData.temp_cpu + " °C";
            document.getElementById("time_active").innerHTML = jsonData.time_active;

            //Actualizar el estado de los relés
            let RELAY1_Status = document.getElementById("RELAY1_Status");
            let RELAY2_Status = document.getElementById("RELAY2_Status");
            let divRELAY1 = document.getElementById("RELAY1").parentNode;
            let divRELAY2 = document.getElementById("RELAY2").parentNode;

            if(jsonData.relay_01_status){
                divRELAY1.classList.remove("switch-off");
                divRELAY1.classList.add("switch-on");           
                RELAY1_Status.innerHTML = "Activo";
                RELAY1_Status.classList.remove("label-info");
                RELAY1_Status.classList.add("label-primary");        
            } else {
                divRELAY1.classList.remove("switch-on");
                divRELAY1.classList.add("switch-off");
                RELAY1_Status.innerHTML = "Inactivo";
                RELAY1_Status.classList.remove("label-primary");
                RELAY1_Status.classList.add("label-info"); 
            }
            if(jsonData.relay_02_status){
                divRELAY2.classList.remove("switch-off");
                divRELAY2.classList.add("switch-on");
                RELAY2_Status.innerHTML = "Activo";
                RELAY2_Status.classList.remove("label-info");
                RELAY2_Status.classList.add("label-primary");        
            } else {
                divRELAY2.classList.remove("switch-on");
                divRELAY2.classList.add("switch-off");
                RELAY2_Status.innerHTML = "Inactivo";
                RELAY2_Status.classList.remove("label-primary");
                RELAY2_Status.classList.add("label-info");

             }
 
        }
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



let relayChange = (e) => {
    console.log(e.id);
    let miChecbox = document.getElementById(e.id);
    let msg = document.getElementById(e.id+"_Status");
    if (miChecbox.checked) {
        msg.innerHTML = "Activo";
        msg.classList.remove("label-info");
        msg.classList.add("label-primary");
        OnOffRelay(e.id, true);
    }else{
        msg.innerHTML = "Inactivo";
        msg.classList.remove("label-primary");
        msg.classList.add("label-info");
        OnOffRelay(e.id, false);
    }
}
// Función para enviar JSON ON/OFF al servidor WebSocket
function OnOffRelay(relay, command){
    connection.send(`{"protocol": "WS", "output": "${relay}", "command": ${command}}`);

}


//------------------------------------------------------
//----------Función para Restaurar el Dispositivo-------
//------------------------------------------------------
function restaurar() {
    Swal.fire({
        title: 'Restablecer!',
        text: ' Toda la configuración que haya guardado se perderá y el dispositivo se restaurará a su configuración de fábrica. ¿Desea continuar?',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Sí, restablecer',
        cancelButtonText: 'Cancelar'
    }).then((result) => {
        if (result.isConfirmed) {
            RestoreRestart("restore");
        } else if(result.dismiss === Swal.DismissReason.cancel){
           history.back();
        }

    });

}

//------------------------------------------------------
//----------Función para Reiniciar el Dispositivo-------
//------------------------------------------------------
function reiniciar() {
    Swal.fire({
        title: 'Reiniciar!',
        text: 'El dispositivo se reiniciará y se desconectará de la red. ¿Desea continuar?',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Sí, reiniciar',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            RestoreRestart("restart");
        } else if(result.dismiss === Swal.DismissReason.cancel){
           history.back();
        }
    });
}

// Conteo de reinicio
let t = 10;
// Ocultar Visu
function visu(){
    var x = document.getElementById("visu");
    x.style.display = "none";
}

//------------------------------------------------------
//----------Función para Reiniciar el Dispositivo-------
//------------------------------------------------------
function RestoreRestart(type) {
    console.log(type);
    t--;
    document.getElementById("visu").style.display = "block";
    document.getElementById("time").innerHTML = " " + t;
    document.getElementById("progress").style.width = t * 10 + "%";
    document.getElementById("progressvalue").innerHTML = " " + t * 10 + "%";
    if(type === "restart"){
        if (t === 0) {
            document.getElementById("visu").style.display = "none";
            document.getElementById("btn").disabled = false;
            window.location = "/";
        }else if(t === 9){
        connection.send("restart");
        document.getElementById("btn").disabled = true;
        window.setTimeout("RestoreRestart('restart')", 1000);           
        } else{
        document.getElementById("btn").disabled = true;
        window.setTimeout("RestoreRestart('restart')", 1000);
        }
    } else {
        if (t === 0) {
            document.getElementById("visu").style.display = "none";
            document.getElementById("btn").disabled = false;
            window.location = "/";
            t = 10;
        }else if(t === 9){
        connection.send("restore");
        getElementById("btn").disabled = true;
        window.setTimeout("RestoreRestart('restore')", 1000);           
        }else{
        document.getElementById("btn").disabled = true;
        window.setTimeout("RestoreRestart('restore')", 1000);
        }
    }
}
