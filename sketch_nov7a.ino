#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"  //libreria para usar el sensor de temperatura

const int pinFotoresistencia = A0;  //pin analogo al que esta conectada la fotoresistencia
#define DHTPIN 2                    //pin digital al que esta conectado el sensor de temperatura
#define DHTTYPE DHT11               // se define el tipo de sensor de temperatura que estamos usando
DHT dht(DHTPIN, DHTTYPE);           //se crea un objeto con el tipo de sensor usado y el pin al que esta conectado
LiquidCrystal_I2C lcd(0x3F,16,2);

//variables para el controlador de los motores de las ruedas
const int AIA = 3;
const int AIB = 4;
const int BIA = 5;
const int BIB = 6;
byte speed = 255;


//metodo para hacer avanzar los motores
void forward() {
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
}

//metodo para hacer retroceder los motores
void backward() {
  analogWrite(AIA, 0);
  analogWrite(AIB, speed);
  analogWrite(BIA, 0);
  analogWrite(BIB, speed);
}

//metodo para detener los motores
void stopMotors() {
  analogWrite(AIA, 0);
  analogWrite(AIB, 0);
  analogWrite(BIA, 0);
  analogWrite(BIB, 0);
}


//setup() Se utiliza para inicializar el hardware y las variables necesarias antes de comenzar el bucle principal.
void setup() {
  dht.begin();  //se inicia el sensor de temperatura

  // se definen como salidas los pines de control del motor
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  Serial.begin(9600);  // esto es para imprimir por consola cositas para debugear
   
  lcd.init(); // Inicializar el LCD
  lcd.backlight(); //Encender la luz de fondo del lcd
  lcd.print("Luz|Temp|Humedad");// Escribimos el Mensaje en el LCD.

}

void loop() {
  int valorLuz = analogRead(pinFotoresistencia);       //se lee el valor de la fotoresistencia
  int porcentajeLuz = map(valorLuz, 0, 1023, 0, 100);  //se mapea el valor a un porcentaje entre 1 y 100
  float t = dht.readTemperature();                     //lectura de temperatura del sensor de temperatura
  float h = dht.readHumidity();                        //lectura de humedad (el sensor de temperatura también lee humedad)

  //agregar logica para mover o detener los motores con la temperatura y humedad
  if (porcentajeLuz < 50) {
    Serial.println("Luz baja, ejecutando motores...");
    forward();
    delay(2000);
  } else {
    Serial.println("Luz suficiente, motores detenidos.");
    stopMotors();
  }
  
  // Ubicamos el cursor del lcd en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);

   // Escribimos en el lcd los datos de los sensores
  lcd.print(porcentajeLuz);
  lcd.print("%|");
  lcd.print((int)t);
  lcd.print("C |");
  lcd.print(h);
  lcd.print("%");
  delay(1000);
}
