#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// ============= VARIABLES DE ESTADOS ==================
#define state1 1
#define state2 2
#define state3 3
#define nullState 0
int currentState = 0;
bool a = true;
int valorAnterior = 0;
int valorActual;
// IDIOMA -> ESP = 0, ING = 1, ITL = 2
int idoma = 0;

const byte interruptPin = 3; // Pin del interruptor de límite
volatile bool triggered = false; // Variable booleana que indica si se ha activado la interrupción



// ================== DECLARACION PARA EL TIMER. =================
// Definir el tiempo en milisegundos para el temporizador de un minuto
const unsigned long minute = 40000;
// Variable que almacenará el tiempo en el que se realizó la última acción
unsigned long lastActionTime;
// ================== DF MINE PLAYER. =================
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


void setup() {
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP); // Configuración del pin como entrada con resistencia pull-up interna
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptFunction, CHANGE); // Configuración de la interrupción en el cambio de estado del pin

  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  // Iniciar la variable lastActionTime
  lastActionTime = millis();
  // ======= DF MINI PLAYER ==========
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) { 
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
    }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(15);
  
}


void loop() {
  controlState();
  States();
  // myDFPlayer.play(1);
  // delay(5000);
  // Serial.println(triggered);

}

void interruptFunction() {
  int valorActual = digitalRead(3);
  // haces todos tus cálculos y al final del loop pones
  if(valorActual != valorAnterior){
    if(valorActual == 0){
      triggered = false;
    }
    else if(valorActual == 1){
      triggered = true; 
      myDFPlayer.pause();
    }    
  }
  else{
  }
  valorAnterior = valorActual;
}

void controlState(){

  if (millis() - lastActionTime >= minute) {
    lastActionTime = millis(); // Actualizar la variable lastActionTime 
    currentState = state1;
    Serial.println("=========================");
  }

  if(triggered == 1 && currentState == state1 ){
    currentState = state2;
  }

}

void States(){
  switch(currentState)
  {
    case nullState:
      nullState_n();
      break;
    case state1:
      stateOne();
      break;
    case state2:
      stateTwo();
      break;
    default:
      break;      
  }
}

// ======== FUNCIONES ============
void stateOne(){
  Serial.println("estado uno");
  myDFPlayer.loop(1);
  delay(8000);
  if(triggered == 0){
    currentState = nullState;
  }
  
}

void stateTwo(){
  Serial.println("estado dos");
  if(idoma == 0){
    // ESP
    myDFPlayer.play(2);
    delay(9000);
    currentState = nullState;
  }else if(idoma == 1){
    // ING

  }else{
    // ITL

  }  
}

void nullState_n(){
  Serial.println("Limbo");
}

