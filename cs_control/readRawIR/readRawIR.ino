/*
Read raw from MLX90614
type: uint16_t

*/
#include <SoftwareSerial.h>
#include <Stepper.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
 

 Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
void setup(){
  Serial.begin(38400);
  Serial.println('connected to MLX90614');
  mlx.begin();
}

void loop(){
  
    Serial.print(mlx.readRawIR1());Serial.print(';');
    Serial.print(mlx.readRawIR2());Serial.print(';');
    Serial.println(mlx.readObjectTempC());
    delay(1000);

}
