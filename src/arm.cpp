#include <Arduino.h>
#include <Servo.h>

#define pinElbow 6
#define pinShoulderX 9
#define pinShoulderY 10
#define pinShoulderZ 11

class Arm{
  private:
    Servo elbowAngle;
    Servo shoulderXAngle;
    Servo shoulderYAngle;
    Servo shoulderZAngle;

  public:
    void init() {
      elbowAngle.attach(pinElbow);
      shoulderXAngle.attach(pinShoulderX);
      shoulderYAngle.attach(pinShoulderY);
      shoulderZAngle.attach(pinShoulderZ);
      elbowAngle.write(100);
      shoulderXAngle.write(90);
      shoulderYAngle.write(0);
      shoulderZAngle.write(125);
    }
    void setElbow(double angle) {
      elbowAngle.write(angle);
    }
    void setShoulderX(double angle) {
      shoulderXAngle.write(angle);
    }
    void setShoulderY(double angle) {
      shoulderYAngle.write(angle);
    }
    void setShoulderZ(double angle) {
      shoulderZAngle.write(angle);
    }
    int getElbow() {
      return elbowAngle.read();
    }
    int getShoulderXAngle() {
      return shoulderXAngle.read();
    }
    int getShoulderYAngle() {
      return shoulderYAngle.read();
    }
    int getShoulderZAngle() {
      return shoulderZAngle.read();
    }

};

struct Message {
  double elbowA;
  double shoulderX;
  double shoulderY;
  double shoulderZ;
};

Message split(char* data) {
  double angles[4];
  for (uint8_t i = 0, part = 0; i < strlen(data); i++) {
    if (data[i] == ',') {
      part++;
    } else {
      angles[part] +=data[i];
    }
  }
  return { angles[0], angles[1], angles[2], angles[3] };
}

Arm arm;

void setup() {
  arm.init();
  Serial.begin(9600);
  Serial.write("Serial connection started, waiting for instructions...");
}

void loop() {
  char * data;
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == ';') {
      // Do Stuff
      Message msg;
      msg = split(data);
      arm.setElbow(msg.elbowA);
      arm.setShoulderX(msg.shoulderX);
      arm.setShoulderX(msg.shoulderY);
      arm.setShoulderX(msg.shoulderZ);
      Serial.write("Received");
    } else {
      // Add inByte to msg
      data += inByte;
    }
  }

}
