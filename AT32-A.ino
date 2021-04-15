// AT32-A
//#include<Wire.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#define ONBOARD_LED  4
int Armada;
int T_1;
int T_2;
char auth[] = "c706e092dca442b6af6f32ab05bcb765";
char ssid[] = "WifiVH";
char pass[] = "12265165";
int hum = 22; //calienta la máquina
int cal= 23; // inyecta humo
int luz= 21; // luz
int chapa=19;// abre puerta chapa electrónica
int PIR=15;
int Laser=13; //desde Wemos
int t_0; // timer inyección de humo
int t_1; // timer iluminación
int t_2; // tiempo calefacción máquina humo
int t_3; // conteo desde el ultimo movimiento

int TiempoLuz=3 ;//minutos la luz prendida luego de activación por PIR
int TiempoCaldera=30; //minutos con la caldera prendida luego de que se activa el láser 
int TiempoHumo=20; //SEGUNDOS que dura la máquina inyectando humo una vez que se activa el PIR. 

BlynkTimer timer;
WidgetLED led1(V5); // SEñal Láser
WidgetLED led2(V4); //Señal Alarma
WidgetLED led3(V6);//PIR
WidgetLED led4(V7);//Disponible
WidgetTerminal terminal(V3);

BLYNK_CONNECTED() {
digitalWrite(ONBOARD_LED,LOW);
 terminal.println( "CONECTADO");
        terminal.flush();
}


void RutinaPrincipal(){

   
  if (digitalRead(PIR)==HIGH){ // CASO EN QUE HAY MOVIMIENTO INTERIOR
      t_3=millis();
      digitalWrite(chapa, LOW); // en caso de movimiento al interior, abre puerta de manera definitiva
      if (Armada==1){  // ESTA ES CONDICION DE ALARMA
        //terminal.print( "ALARMA ");
        //terminal.flush();
        digitalWrite(cal, HIGH); //calienta o prende la máquina
        digitalWrite(hum, HIGH); //inyecta humo
        digitalWrite(luz, HIGH); //prende luz
        led2.on(); //LED Alarma
        t_0=millis(); // comienza conteo de tiempo de humo
        t_1=millis(); // comienza conteo de tiempo de luz
      }
      if (Armada==0){ // es decir, si se activa PIR y la alarma no está activada
        digitalWrite(hum, LOW); //apaga humo
        digitalWrite(luz, HIGH); //prende luz
        t_1=millis(); // comienza conteo de tiempo de luz
      }
  }


  if (digitalRead(Laser)==HIGH){
      led1.on();
      if (Armada==HIGH){
        digitalWrite(cal, HIGH); //calienta o prende la máquina
        t_2=millis(); //empieza conteo tiempo de calefacción de máquina
      }
      if (Armada==LOW){
        digitalWrite(luz, HIGH); //prende luz
        t_1=millis(); // comienza conteo de tiempo de luz
        
      }
  }
  if (digitalRead(Laser)==LOW) led1.off();
  
  
 



 if (Armada==1){ // CIERRA LA PUERTA DESPUES DE UN TIEMPO DE ARMADA LA ALARMA
  if (millis()-t_3 > 10*1000) digitalWrite(chapa, HIGH); //Cierra chapa puerta
 }

 
 
 
 if (millis()-t_0 > (TiempoHumo*1000)) digitalWrite(hum, LOW); //apaga humo cuando se cumple el tiempo de inyección
 if (millis()-t_0 > (20*1000)) led2.off();//apaga señal de alarma (inyectó humo un rato)
 if (millis()-t_1 > (TiempoLuz*60*1000)) digitalWrite(luz, LOW); //apaga luz cuando se cumple el tiempo de iluminación      
 if (millis()-t_2 > (TiempoCaldera*60*1000)) digitalWrite(cal, LOW); //apaga la caldera luego de 30 minutos que se prendió el láser
}

void setup(){
   Blynk.begin(auth, ssid, pass); //Blynk
   timer.setInterval(1000L, RutinaPrincipal);
   pinMode(hum, OUTPUT);
   pinMode(cal, OUTPUT);
   pinMode(luz, OUTPUT);
   pinMode(chapa, OUTPUT);
   pinMode(Laser, INPUT);
   pinMode(PIR, INPUT);
   terminal.clear();
   
   
}

BLYNK_WRITE(V1){
int AbrePuerta = param.asInt();
}

BLYNK_WRITE(V2){
  Armada = param.asInt(); 
}

BLYNK_WRITE(V15){
       int PIR = param.asInt(); // pinData variable will store value that came via Bridge
       if (PIR > 0 ) led3.on();
        if (PIR == 0 ) led3.off();
       }

void loop(){
  Blynk.run(); 
  timer.run(); 
}
