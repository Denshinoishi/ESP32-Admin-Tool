/*
    Guardar Estados IO
*/


boolean settingsSaveRelays(){
    //  Guarda la configuración de IO
    StaticJsonDocument<500> jsonConfig;
    File file = SPIFFS.open(F("/settingrelay.json"), "w+");
    if (file)
    {
        jsonConfig["relay_01_status"] = relay_01_status;
        jsonConfig["relay_02_status"] = relay_02_status;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Estado de IO Guardado"));
        serializeJsonPretty(jsonConfig, Serial);
        return true;

    }else{
        log(F("Error: Falló el guardado de Estado IO"));
        return false;
    }
    

}