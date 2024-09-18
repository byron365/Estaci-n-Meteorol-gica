#include <DHTesp.h>//Libreria para el uso del DHT22 con el ESP32
#include "Datos.h" //Archivo con información sensible

volatile int contador = 0;   // Variable entera que se almacena en la RAM del Micro
double velocidad = 0;
const float pi = 3.141592;
int dhtPin = 13;
int factor = 2.2; //Factor de error

//Instancias
DHTesp dhtSensor;

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);//Conectando a la red WIFI y a Blynk
  dhtSensor.setup(dhtPin, DHTesp::DHT22);//Configurando el módulo DHT22
  pinMode(2,OUTPUT);
  Serial.begin(57600);

  //Interrupción para el sensor de barrera
  attachInterrupt(digitalPinToInterrupt(14),interrupcion0,RISING);  //(pin 14) 

  //Verificando conexión con Blynk
  digitalWrite(2,HIGH);
  while(!Blynk.connect()){
    Serial.println("Conectando!");
  }
  digitalWrite(2,LOW);
}

void loop() {
 Blynk.run();//Iniciando Blynk
  if(Blynk.connected()){
    digitalWrite(2,LOW);

    //Leyendo temperatura
    TempAndHumidity datos = dhtSensor.getTempAndHumidity(); //Leyendo datos del DHT22
    delay(1000);   

    //Calculando la velocidad del viento
    velocidad = ((2*pi*0.05*contador)/60)*factor;
    Serial.println("Temp: " + String(datos.temperature) + "°c");
    Serial.println("Humedad: " + String(datos.humidity) + "%");
    Serial.println("Velocidad V. " + String(velocidad) + " m/s");

    //Enviando datos
    Blynk.virtualWrite(V0, datos.humidity);
    Blynk.virtualWrite(V1, datos.temperature);
    Blynk.virtualWrite(V2, velocidad);
    contador = 0;

  }else{
    digitalWrite(2,HIGH);
  }
}

void interrupcion0()    // Funcion que se ejecuta durante cada interrupion
{
  contador++;           // Se incrementa en uno el contador
}


