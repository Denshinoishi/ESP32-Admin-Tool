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
    //console.log('Mensaje Recibido: ' + jsonData);
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

// Validación de Formulario
const formulario = document.getElementById("form");
const inputs = document.querySelectorAll("#form input");
// Objeto de Expresiones Regulares
const expresiones = {
    TextNumber: /^[a-zA-Z0-9]{4,30}$/,      // Letras y números, mínimo 4 caracteres y máximo 30
    TextUnderscore: /^[a-zA-Z\_]{4,30}$/,   // Letras minúsculas, mayúsculas y guion bajo, mínimo 4 caracteres y máximo 30
    TextNumberPassw: /^[a-zA-Z0-9\_\-\*]{4,30}$/, // Letras, números y los siguientes caracteres: (-)(_) (*), mínimo 4 caracteres y máximo 30
    Dominio: /^([a-zA-Z]{2,6}|[a-zA-Z0-9]{2,30}\.[a-zA-Z]{2,3})$/,  // Dominio
    ServerPort: /^(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[0-5]?([0-9]){0,3}[0-9])$/, // Puerto de Servidor
    TextNumberWIFI: /^[a-zA-Z0-9\_\-\*\.\#\']{4,30}$/, // Letras, números y los siguientes caracteres: (-)(_) (*), mínimo 4 caracteres y máximo 30
    TextNumberAP: /^[a-zA-Z0-9 \_\-\*\.\#\']{4,31}$/, // Letras, números y los siguientes caracteres: (-)(_) (*), mínimo 4 caracteres y máximo 31
    IPv4: /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/, // Dirección IP
    TextNumberPasswAP: /^[a-zA-Z0-9\_\-\*]{4,63}$/, // Letras, números y los siguientes caracteres: (-)(_) (*), mínimo 4 caracteres y máximo 63
    www_username: /^[a-z]{4,15}$/, // Letras minúsculas de 4 a 15 carácteres
    www_password: /^[a-zA-Z0-9]{4,15}$/, // Letras minúscuals y mayúsculas más números de 4 a 15 carácteres
    id: /^[a-z0-9]{4,30}$/ //Letras minúsculas y digítos 4 a 30 carácteres
     

}

const campos = {
        mqtt_id: true,
        mqtt_user: true,
        mqtt_passw: true,
        mqtt_server: true,
        mqtt_port: true,
        mqtt_time: true,
        // Wifi 
        wifi_ssid: true,
        wifi_passw: true,
        wifi_ip_static: true,
        wifi_subnet: true,
        wifi_gateway: true,
        wifi_primaryDNS: true,
        wifi_secondaryDNS: true,
        ap_nameap: true,
        ap_passwordap: true,
        ap_canalap: true,
        ap_connetap: true,
        // admin
        www_username: true,
        www_password: true,
        new_www_username: true,
        new_www_password: true,
        // device
        id: true
    }

// Dejar de Teclear y la pérdia de Foco del Input

inputs.forEach((input) => {
    input.addEventListener("keyup", validarFormulario);
    input.addEventListener("blur", validarFormulario);
});

// Función para Validar el Formulario
function validarFormulario(e) {
    //console.log(e.target.dataset.expresion);
    switch (e.target.name) {
        case "mqtt_id":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_user":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_passw":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_server":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_port":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_time":
            validarRango(e.target, e.target.name, 1, 60);
            break;
            // Sección WIFI
        case "wifi_ssid":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_passw":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_ip_static":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_subnet":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_gateway":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_primaryDNS":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_secondaryDNS":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
            //AP
        case "ap_nameap":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "ap_passwordap":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "ap_canalap":
            validarRango(e.target, e.target.name, 1, 13);
            break;
        case "ap_connetap":
            validarRango(e.target, e.target.name, 0, 8);
            break;
            /* ADMIN FORM */
        case "www_username":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "www_password":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "new_www_username":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "new_www_password":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            validarPassword();
            break;
        case "c_new_www_password":
            validarPassword();
            break;
        case "id":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
    }
}


// Función para Validar los Campos del Formulario
const validarCampo = (expresion, input, campo) => {
    //console.log("Validando Campo: " + campo);
    //console.log("Validar input: " + input.name);
    if (expresion.test(input.value) && input.value !== "") {
        document.querySelector(`#form_${campo}`).classList.remove('has-error');
        document.querySelector(`#form_${campo}`).classList.add('has-success');
        document.querySelector(`#form_${campo} .formulario_input-error`).classList.remove('formulario_input-error-activo');
        campos[campo] = true;
    } else {
        document.querySelector(`#form_${campo}`).classList.remove('has-success');
        document.querySelector(`#form_${campo}`).classList.add('has-error');
        document.querySelector(`#form_${campo} .formulario_input-error`).classList.add('formulario_input-error-activo');
        campos[campo] = false;
    }
}
const validarRango = (input, campo, min, max) => {
        if (input.value >= min && input.value <= max) {
            document.querySelector(`#form_${campo}`).classList.add('has-success');
            document.querySelector(`#form_${campo}`).classList.remove('has-error');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.remove('formulario_input-error-activo');
            campos[campo] = true;
        } else {
            document.querySelector(`#form_${campo}`).classList.add('has-error');
            document.querySelector(`#form_${campo}`).classList.remove('has-success');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.add('formulario_input-error-activo');
            campos[campo] = false;
        }
    }

// Validar passw y new passw
const validarPassword = () =>{
    const inputNewPassword = document.getElementById('www_password');
    const inputNwePasswordConfirm = document.getElementById('c_new_www_password');
    if(inputNewPassword.value === inputNwePasswordConfirm.value){
        document.querySelector('#form_c_new_www_password').classList.add('has-succes');
        document.querySelector('#form_c_new_www_password').classList.remove('has-error');
        document.querySelector('#form_c_new_www_password .formulario_input-error').classList.remove('formulario_input-error-activo');
        campos['new_www_password'] = true;
    }else{
        document.querySelector('#form_c_new_www_password').classList.add('has-error');
        document.querySelector('#form_c_new_www_password').classList.remove('has-succes');
        document.querySelector('#form_c_new_www_password .formulario_input-error').classList.add('formulario_input-error-activo');
        campos['new_www_password'] = false;
    }
}

    // Interceptar el Evento Submit Solo en los Formularios (wifi-mqtt-device-admin)
if (pathname == "/esp-wifi" || pathname == "/esp-mqtt" || pathname == "/esp-device" || pathname == "/esp-admin") {
    document.addEventListener("DOMContentLoaded", function(event) {
        document.getElementById('form').addEventListener('submit', manejadorValidacion)
    });
}

function manejadorValidacion(e) {
    e.preventDefault();
    let page = document.getElementById('page').innerHTML;
    console.log(page);
    if (page == "Configuración del Broker MQTT") {
        
        if (campos.mqtt_id && campos.mqtt_user && campos.mqtt_passw && campos.mqtt_server && campos.mqtt_port && campos.mqtt_time) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);

        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Configuración de la Red Inalámbrica") {
        if (campos.wifi_ssid && campos.wifi_passw && campos.wifi_ip_static && campos.wifi_subnet && campos.wifi_gateway && campos.wifi_primaryDNS && campos.wifi_secondaryDNS && campos.ap_nameap && campos.ap_passwordap && campos.ap_canalap && campos.ap_connetap) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Usuario y Contraseña") {
        if (campos.www_username && campos.www_password && campos.new_www_username && campos.new_www_password) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Información del dispositivo") {
        if (campos.id) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    }
}


// Mansaje para confirmar el Guardado con el Evento Submit
function SweetAlert(title, text, icon, e) {
    Swal.fire({
        title: title,
        text: text,
        icon: icon,
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Si, guardar',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            e.submit();
        } else if (
            result.dismiss === Swal.DismissReason.cancel
        ) {
            history.back();
        }
    })
}

// Mensaje de Error en la validación del Formulario
let mensajeFormError = () => {
    Swal.fire({
        position: 'top-end',
        icon: 'error',
        title: '¡No se puede Guardar!',
        showConfirmButton: false,
        timer: 2000
    });
};

// Escaneo de redes

let escanear = () =>{
    const url = 'http://'+ location.hostname +'/scan';
    fetch(url)
        .then(respuesta => respuesta.json())
        .then(data  =>{
            if(pathname == "/esp-wifi"){
                // capturamos el id de las redes
                let redes = document.getElementById('redes');
                // capturamos el id de serial
                let serial = document.getElementById('serial');
                // limpiamos redes
                redes.innerHTML = "";
                // limpiamos  serial
                serial.inputHTML = "";
                // Mensaje de respueta
                if (data.code == 1) {
                    // Count > 0
                    // Capturamos el id del body para dibujar la tabla
                    let mitabla = document.getElementById('mi_tabla');
                    // limpiamos la tabla
                    mitabla.innerHTML = "";                    
                    // Agregamos cantidad de redes
                    redes.innerHTML = data.meta.count;
                    // Agregamos el serial del dispositivo
                    serial.innerHTML = "<i class='icon-barcode'></i> " + data.meta.serial;
                    // Recorremos toda la longitud del data key DATA
                        for (let valor of data.data) {
                            if (parseInt(valor.rssi) >= -67) {
                                mitabla.innerHTML += `  <tr class="success">
                                                                <td align="center">${valor.n}</td>
                                                                <td>${valor.ssid}</td>
                                                                <td align="center">${valor.rssi}</td>
                                                                <td>${valor.bssid}/<br>${valor.secure}</td>
                                                                <td align="center">${valor.channel}</td>                  
                                                                <td><button class="btn btn-primary" onclick="addWiFiInput();"><i class="icon-magnet"></i> Add</button></td>
                                                            </tr>
                                                        `;
                            } else if (parseInt(valor.rssi) <= -67 && parseInt(valor.rssi) > -80) {
                                mitabla.innerHTML += `<tr class="info">
                                                                <td align="center">${valor.n}</td>
                                                                <td>${valor.ssid}</td>
                                                                <td align="center">${valor.rssi}</td>
                                                                <td>${valor.bssid}/<br>${valor.secure}</td>
                                                                <td align="center">${valor.channel}</td>
                                                                <td><button class="btn btn-primary" onclick="addWiFiInput();"><i class="icon-magnet"></i> Add</button></td>
                                                            </tr>
                                                        `;
                            } else if (parseInt(valor.rssi) <= -80 && parseInt(valor.rssi) > -90) {
                                mitabla.innerHTML += `<tr class="warning">
                                                                <td align="center">${valor.n}</td>
                                                                <td>${valor.ssid}</td>
                                                                <td align="center">${valor.rssi}</td>
                                                                <td>${valor.bssid}/<br>${valor.secure}</td>
                                                                <td align="center">${valor.channel}</td>
                                                                <td><button class="btn btn-primary" onclick="addWiFiInput();"><i class="icon-magnet"></i> Add</button></td>
                                                            </tr>
                                                            `;
                            } else {
                                mitabla.innerHTML += `<tr class="danger">
                                                                <td align="center">${valor.n}</td>
                                                                <td>${valor.ssid}</td>
                                                                <td align="center">${valor.rssi}</td>
                                                                <td>${valor.bssid}/<br>${valor.secure}</td>
                                                                <td align="center">${valor.channel}</td>
                                                                <td><button class="btn btn-primary" onclick="addWiFiInput();"><i class="icon-magnet"></i> Add</button></td>
                                                            </tr>
                                                        `;
                            }
                        };
                    
                }else{
                    // Agregamos cantidad de redes
                    redes.innerHTML = data.meta.count;
                    // Agregamos el serial del dispositivo
                    serial.innerHTML = "<i class='icon-barcode'></i> " + data.meta.serial;
                }
            }
        });
}

    // Agregar el Nombre de la Red al Input segun Click en el Botón en la fila correspondiente
let addWiFiInput = () => {
    const table = document.getElementById("tableId");
    const rows = table.getElementsByTagName("tr");
    for (let i = 0; i < rows.length; i++) {
        let currentRow = table.rows[i];
        let createClickHandler =
            function(row) {
                return function() {
                    let cell = row.getElementsByTagName("td")[1];
                    let ssid = cell.innerHTML;
                    //alert(ssid);
                    let network = document.getElementById("wifi_ssid");
                    network.value = ssid;
                    $('#myModal').modal('hide');
                };
            };
        currentRow.onclick = createClickHandler(currentRow);
    }
}