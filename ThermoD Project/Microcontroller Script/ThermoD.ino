/*importar bibliotecas*/
#include <LiquidCrystal.h>
#include <OneWire.h>  
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define dados 32

String resposta, httpResponseCode;

int interval = 5000;
int lastMillis = 0;

//definir os pinos para o LCD e o pino "dadps"para temperature
const int RS=13,E=12,D0=14,D1=27,D2=26,D3=25,D4=33,D5=0,D6=4,D7=15;
LiquidCrystal lcd(RS, E, D0, D1, D2, D3, D4, D5, D6, D7);

OneWire oneWire(dados);  //Protocolo OneWire
DallasTemperature sensors(&oneWire); //encaminha referências OneWire para o sensor
DeviceAddress tempDeviceAddress; //usamos esta variavel para guardar o endereço do dispositivo encontrado

const char* ssid = "/*nome da net local /SSID*/"; 
const char* password = "/*pass da net local*/"; 

//Network Configurations
  IPAddress staticIP(192, 168, 30, 4);
  IPAddress gateway(192, 168, 30, 1);
  IPAddress subnet(255, 255, 255, 0);

void setup() {
  //iniciar o serial monitor em 9600
  Serial.begin(9600);
  pinMode(dados, OUTPUT);
  //definir a quantia de linhas e culunas 
  lcd.begin(16, 2);//16x2
  sensors.begin(); /*inicia biblioteca*/
/*Divisão */
  WiFi.config(staticIP,gateway,subnet);
  WiFi.begin(ssid, password);/*criar um corpo chamado wifi*/
  Serial.println("Connecting");/*printa no serial monitor que está conectado*/
  lcd.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  Serial.println("");
  lcd.clear();
  Serial.print("Connected to WiFi network with IP Address: ");
  lcd.print("IP:");
  Serial.println(WiFi.localIP());/*printa no serial monitor o IP local*/
  lcd.print(WiFi.localIP());
  delay(500);
/*Divisão */
  lcd.clear();
  pinMode(dados, OUTPUT);
  String initialText = String("Temperatura:");
  lcd.print(initialText);/*printa a sting no lcd*/}

String STemp(){
  //o sensors.requestTemperatures() serve para o requerimento de temperatura de todos os dispositivos ligados
  sensors.requestTemperatures(); // Envia o comando para leitura da temperatura
  String txt = String(sensors.getTempCByIndex(0))/*endereço do sensor*/;
  Serial.println(txt);
  lcd.setCursor(0,1);
  lcd.print(txt);
  return txt;}

void Request(String temp) {
  String URL = "Your_Website_goes_here";
  //if((millis()-lastMillis)==interval){
    if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        http.begin(URL.c_str());
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");//define que vou mandar ficheiros json
        String data = "temp="+temp+"sensor="+"1";
        Serial.println(data);
        //resposta = data;
        int httpResponseCode = http.POST(data);
        if (httpResponseCode>0) {
          String payload = http.getString();
          Serial.println(payload);
          resposta = payload;}
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);}
        http.end();
    }
  //}
  Serial.println("resposta "+resposta);
  Serial.println("httpResponseCode "+httpResponseCode);
}

void loop() {
  String temp = STemp();
  Request(temp);
}