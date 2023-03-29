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

// Definir los pines del botón
const int BUTTON_1_PIN = 5;
const int BUTTON_2_PIN = 7;

// Definir el tiempo de espera antes de la acción de fallback (en milisegundos)
const unsigned long TIMEOUT = 61000;

// Definir las variables para el estado anterior del botón y el tiempo de la última pulsación
bool lastButtonState_1 = LOW;
bool lastButtonState_2 = LOW;
unsigned long lastButtonPressTime = 0;
int lastButton = 0;

// ================== DECLARACION PARA EL TIMER. =================
// Definir el tiempo en milisegundos para el temporizador de un minuto
const unsigned long minute = 60000;
// Variable que almacenará el tiempo en el que se realizó la última acción
unsigned long lastActionTime;
// ================== DF MINE PLAYER. =================
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


void setup() {
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP); // Configuración del pin como entrada con resistencia pull-up interna
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  // Configuración de la interrupción en el cambio de estado del pin
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptFunction, CHANGE); 

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

  // Leer el estado actual de los botones
  bool buttonState_1 = digitalRead(BUTTON_1_PIN);
  bool buttonState_2 = digitalRead(BUTTON_2_PIN);

  // Verificar si alguno de los botones se ha presionado
  if (buttonState_1 != lastButtonState_1 || buttonState_2 != lastButtonState_2) {
    // Actualizar el tiempo de la última pulsación de botón
    lastButtonPressTime = millis();

    // Verificar qué botón se ha presionado
    if (buttonState_1 == LOW) {
      // Realizar la acción para el botón 1
      lastButton = 1;

    } else if (buttonState_2 == LOW){
      // Realizar la acción para el botón 2
      lastButton = 2;
    }
    // Actualizar el estado anterior del botón
    lastButtonState_1 = buttonState_1;
    lastButtonState_2 = buttonState_2;
  } else {
    // Verificar si ha pasado el tiempo de espera para la acción de fallback
    if (millis() - lastButtonPressTime >= TIMEOUT) {
      // Realizar la acción de fallback
      lastButton = 3;

    }
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
  myDFPlayer.pause();
  if(triggered == 0){
    currentState = nullState;
  }
}

void stateTwo(){
  Serial.println("estado dos");
  int randomNumber = random(1, 5);
  if(lastButton == 1){
    // ESP
    myDFPlayer.play(randomNumber);
    delay(40000);
    myDFPlayer.pause();
    currentState = nullState;
  }else if(lastButton == 2){
    // ING
    myDFPlayer.play(randomNumber+5);
    delay(42000);
    myDFPlayer.pause();
    currentState = nullState;
  }else if(lastButton == 3){
    // ITL
    myDFPlayer.play(randomNumber+10);
    delay(45000);
    myDFPlayer.pause();
    currentState = nullState;
  }  
}

void nullState_n(){
  Serial.println(lastButton);
}

