#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Wire.begin();

  Serial.begin(38400);

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("a/g:\t");
  Serial.print(map(ax, -32768, +32767, -250, 250)); Serial.print("\t");
  Serial.print(map(ay, -32768, +32767, -250, 250)); Serial.print("\t");
  Serial.print(map(az, -32768, +32767, -250, 250)); Serial.print("\t");
  Serial.print(map(gx, -32768, +32767, -250, 250)); Serial.print("\t");
  Serial.print(map(gy, -32768, +32767, -250, 250)); Serial.print("\t");
  Serial.println(map(gz, -32768, +32767, -250, 250));

  delay(1000);
}
