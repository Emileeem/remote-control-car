#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define switchCar 13//switch to start the car 

Adafruit_MPU6050 mpu;

SoftwareSerial bluetooth(10, 11); //PINOS QUE EMULAM A SERIAL, ONDE
float anguloY = 0;
float last_timeY = 0;

void angles()
{
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print("Rotation Y: ");
    Serial.print(g.gyro.y);
    Serial.println(" rad/s");

    Serial.println("");
    delay(500);

    anguloY = anguloY + g.gyro.y * (millis() - last_timeY) / 1000;
    last_timeY = millis();
    Serial.print("anguloY:");
    Serial.println(anguloY);

    delay(1000);
}

void setup()
{
    Serial.begin(9600);
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
        delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
        case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
        case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
        case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
        case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH
    bluetooth.print("$"); //IMPRIME O CARACTERE
    bluetooth.print("$"); //IMPRIME O CARACTERE
    bluetooth.print("$"); //IMPRIME O CARACTERE
    Serial.println("");
    delay(100);
    pinMode(switchCar, INPUT);
}

void loop()
{
  angles();

  bluetooth.write((digitalRead(switchCar)));
  bluetooth.write("\n");
  bluetooth.write(anguloY);




  delay(500);
}
