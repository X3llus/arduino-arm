// #include <Arduino.h>
// #include <I2Cdev.h>
// #include <MPU6050_6Axis_MotionApps20.h>
// #include <Wire.h>
//
// class Arm {
//   struct Vector {
//     double x;
//     double y;
//     double z;
//   };
//   private:
//     Vector wrist{65, 0, 0};
//     Vector elbow{30, 0, 0};
//     Vector wristA{0, 0, 0};
//     Vector elbowA{0, 0, 0};
//     const Vector shoulder{0, 0, 0};
//     const Vector core{-30, 0, 0};
//     Vector forearm{makeForearm()};
//     Vector upperarm{makeUpperarm()};
//     double elbowAngle = 0;
//     Vector shoulderAngles{0, 0, 0};
//
//     Vector makeForearm() {
//       double x = elbow.x - wrist.x;
//       double y = elbow.y - wrist.y;
//       double z = elbow.z - wrist.z;
//       return {x, y, z};
//     }
//     Vector makeUpperarm() {
//       double x = shoulder.x - elbow.x;
//       double y = shoulder.y - elbow.y;
//       double z = shoulder.z - elbow.z;
//       return {x, y, z};
//     }
//
//   public:
//     Vector getWrist() {
//       return wrist;
//     }
//
//     void setForearm() {
//       double x = elbow.x - wrist.x;
//       double y = elbow.y - wrist.y;
//       double z = elbow.z - wrist.z;
//       forearm = {x, y, z};
//     }
//
//     void setUpperarm() {
//       double x = shoulder.x - elbow.x;
//       double y = shoulder.y - elbow.y;
//       double z = shoulder.z - elbow.z;
//       upperarm = {x, y, z};
//     }
//
//     void setElbowAngle() {
//       double a = sqrt(sq(forearm.x) + sq(forearm.y) + sq(forearm.z));
//       double b = sqrt(sq(upperarm.x) + sq(upperarm.y) + sq(upperarm.z));
//       double c = sqrt(sq(shoulder.x - wrist.x) + sq(shoulder.y - wrist.y) + sq(shoulder.z - wrist.z));
//
//       elbowAngle = acos((sq(a) + sq(b) - sq(c)) / (2*a*b));
//     }
//
//     void setShoulderAngleX() {
//       double a = sqrt(sq(upperarm.y) + sq(upperarm.z));
//       double b = sqrt(sq(shoulder.y + core.y) + sq(shoulder.z + core.z));
//       double c = sqrt(sq(core.y - elbow.y) + sq(core.z - elbow.z));
//
//       shoulderAngles.x = acos((sq(a) + sq(b) - sq(c)) / (2*a*b));
//     }
//
//     void setShoulderAngleY() {
//       double a = sqrt(sq(upperarm.x) + sq(upperarm.z));
//       double b = sqrt(sq(shoulder.x + core.x) + sq(shoulder.z + core.z));
//       double c = sqrt(sq(core.x - elbow.x) + sq(core.z - elbow.z));
//
//       shoulderAngles.y = acos((sq(a) + sq(b) - sq(c)) / (2*a*b));
//     }
//
//     void setShoulderAngleZ() {
//       double a = sqrt(sq(upperarm.x) + sq(upperarm.y));
//       double b = sqrt(sq(shoulder.x + core.x) + sq(shoulder.y + core.y));
//       double c = sqrt(sq(core.x - elbow.x) + sq(core.y - elbow.y));
//
//       shoulderAngles.z = acos((sq(a) + sq(b) - sq(c)) / (2*a*b));
//     }
//
//     double getElbowAngle() {
//       setElbowAngle();
//       return elbowAngle;
//     }
//
//     double getShoulderAngleX() {
//       return shoulderAngles.x;
//     }
//
//     double getShoulderAngleY() {
//       return shoulderAngles.y;
//     }
//
//     double getShoulderAngleZ() {
//       return shoulderAngles.z;
//     }
//
//     void setWrist(double ax, double ay, double az) {
//
//     }
//
//     void setElbow(double ax, double ay, double az) {
//
//     }
// };
//
// MPU6050 mpuElbow(0x69);
// MPU6050 mpuWrist;
// #define out 7
//
// // MPU control/status vars
// bool dmpReady = false;  // set true if DMP init was successful
// uint8_t devStatusElbow;      // return status after each device operation (0 = success, !0 = error)
// uint8_t devStatusWrist;      // return status after each device operation (0 = success, !0 = error)
// uint16_t packetSizeElbow;    // expected DMP packet size (default is 42 bytes)
// uint16_t packetSizeWrist;    // expected DMP packet size (default is 42 bytes)
// uint16_t fifoCountElbow;     // count of all bytes currently in FIFO
// uint16_t fifoCountWrist;     // count of all bytes currently in FIFO
// uint8_t fifoBufferElbow[64]; // FIFO storage buffer
// uint8_t fifoBufferWrist[64]; // FIFO storage buffer
//
// // orientation/motion vars
// Quaternion q;           // [w, x, y, z]         quaternion container
// VectorInt16 aa;         // [x, y, z]            accel sensor measurements
// VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
// VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
// VectorFloat gravity;    // [x, y, z]            gravity vector
// float euler[3];         // [psi, theta, phi]    Euler angle container
// float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//
// Quaternion q2;           // [w, x, y, z]         quaternion container
// VectorInt16 aa2;         // [x, y, z]            accel sensor measurements
// VectorInt16 aaReal2;     // [x, y, z]            gravity-free accel sensor measurements
// VectorInt16 aaWorld2;    // [x, y, z]            world-frame accel sensor measurements
// VectorFloat gravity2;    // [x, y, z]            gravity vector
// float euler2[3];         // [psi, theta, phi]    Euler angle container
// float ypr2[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//
// int16_t ax1, ay1, az1;
// int16_t ax2, ay2, az2;
// long double lastTime;
//
// void setup() {
//   Arm arm;
//
//   pinMode(out, OUTPUT);
//   digitalWrite(out, HIGH);
//
//   Wire.begin();
//   // Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
//
//   // initialize serial communication
//   Serial.begin(38400);
//
//   // initialize device
//   Serial.println(F("Initializing I2C devices..."));
//   mpuElbow.initialize();Serial.println("Elbow Init");
//   mpuWrist.initialize();Serial.println("Wrist Init");
//
//   // verify connection
//   Serial.println(F("Testing device connections..."));
//   Serial.println(mpuElbow.testConnection() ? F("Elbow connection successful") : F("Elbow connection failed"));
//   Serial.println(mpuWrist.testConnection() ? F("Wrist connection successful") : F("Wrist connection failed"));
//
//   // load and configure the DMP
//   Serial.println(F("Initializing DMP..."));
//   devStatusElbow = mpuElbow.dmpInitialize();
//   devStatusWrist = mpuWrist.dmpInitialize();
//
//   // make sure it worked (returns 0 if so)
//   if (devStatusElbow == 0 && devStatusWrist == 0) {
//     // Calibration Time: generate offsets and calibrate our MPU6050
//     // mpuElbow.CalibrateAccel(6);
//     // mpuElbow.CalibrateGyro(6);
//     // mpuElbow.PrintActiveOffsets();
//
//     // mpuWrist.CalibrateAccel(6);
//     // mpuWrist.CalibrateGyro(6);
//     // mpuWrist.PrintActiveOffsets();
//
//     Serial.println(F(")..."));
//
//     // set our DMP Ready flag so the main loop() function knows it's okay to use it
//     dmpReady = true;
//
//     // get expected DMP packet size for later comparison
//     packetSizeElbow = mpuElbow.dmpGetFIFOPacketSize();
//     Serial.println(packetSizeElbow);
//     packetSizeWrist = mpuWrist.dmpGetFIFOPacketSize();
//     Serial.println(packetSizeWrist);
//   } else {
//     // ERROR!
//     // 1 = initial memory load failed
//     // 2 = DMP configuration updates failed
//     // (if it's going to break, usually the code will be 1)
//     Serial.print(F("DMP Initialization failed (code "));
//     Serial.print(devStatusElbow);
//     Serial.print(devStatusWrist);
//     Serial.println(F(")"));
//   }
// }
//
// void loop(){
//
//   // if programming failed, don't try to do anything
//   if (!dmpReady) {
//     Serial.println(dmpReady);
//     return;
//   }
//   Serial.println(fifoCountElbow);
//   Serial.println(packetSizeElbow);
//   // wait for MPU interrupt or extra packet(s) available
//   while (fifoCountElbow < packetSizeElbow || fifoCountWrist < packetSizeWrist) { //  || fifoCountWrist < packetSizeWrist
//     Serial.println(fifoCountElbow);
//     Serial.println(fifoCountWrist);
//     if (fifoCountElbow < packetSizeElbow) {
//       fifoCountElbow = mpuElbow.getFIFOCount();
//     }
//     if (fifoCountWrist < packetSizeWrist) {
//       fifoCountWrist = mpuWrist.getFIFOCount();
//     }
//     // other program behavior stuff here
//     // .
//     // .
//     // .
//     // if you are really paranoid you can frequently test in between other
//     // stuff to see if mpuInterrupt is true, and if so, "break;" from the
//     // while() loop to immediately process the MPU data
//     // .
//     // .
//     // .
//   }
//     Serial.println(fifoCountElbow);
//     // get current FIFO count
//     fifoCountElbow = mpuElbow.getFIFOCount();
//     fifoCountWrist = mpuWrist.getFIFOCount();
//     if(fifoCountElbow < packetSizeElbow && fifoCountWrist < packetSizeWrist){
//       //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
//     // check for overflow (this should never happen unless our code is too inefficient)
//   } else {
//     if (fifoCountElbow >= 1024) {
//       // reset so we can continue cleanly
//       mpuElbow.resetFIFO();
//       Serial.println(F("Elbow FIFO overflow!"));
//       delay(100);
//     }
//     if (fifoCountWrist >= 1024) {
//       // reset so we can continue cleanly
//       mpuWrist.resetFIFO();
//       Serial.println(F("Wrist FIFO overflow!"));
//       delay(100);
//     // otherwise, check for DMP data ready interrupt (this should happen frequently)
//     }
//
//       // read a packet from FIFO
//       while(fifoCountElbow >= packetSizeElbow){ // Lets catch up to NOW, someone is using the dreaded delay()!
//         mpuElbow.getFIFOBytes(fifoBufferElbow, packetSizeElbow);
//         // track FIFO count here in case there is > 1 packet available
//         // (this lets us immediately read more without waiting for an interrupt)
//         fifoCountElbow -= packetSizeElbow;
//       }
//       // display initial world-frame acceleration, adjusted to remove gravity
//       // and rotated based on known orientation from quaternion
//       mpuElbow.dmpGetQuaternion(&q, fifoBufferElbow);
//       mpuElbow.dmpGetAccel(&aa, fifoBufferElbow);
//       mpuElbow.dmpGetGravity(&gravity, &q);
//       mpuElbow.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//       mpuElbow.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
//
//       Serial.print("aworld elbow\t");
//       Serial.print(aaWorld.x);
//       Serial.print("\t");
//       Serial.print(aaWorld.y);
//       Serial.print("\t");
//       Serial.println(aaWorld.z);
//
//       // read a packet from FIFO
//       while(fifoCountWrist >= packetSizeWrist){ // Lets catch up to NOW, someone is using the dreaded delay()!
//         mpuWrist.getFIFOBytes(fifoBufferWrist, packetSizeWrist);
//         // track FIFO count here in case there is > 1 packet available
//         // (this lets us immediately read more without waiting for an interrupt)
//         fifoCountWrist -= packetSizeWrist;
//       }
//       // display initial world-frame acceleration, adjusted to remove gravity
//       // and rotated based on known orientation from quaternion
//       mpuWrist.dmpGetQuaternion(&q2, fifoBufferWrist);
//       mpuWrist.dmpGetAccel(&aa2, fifoBufferWrist);
//       mpuWrist.dmpGetGravity(&gravity2, &q2);
//       mpuWrist.dmpGetLinearAccel(&aaReal2, &aa2, &gravity2);
//       mpuWrist.dmpGetLinearAccelInWorld(&aaWorld2, &aaReal2, &q2);
//
//       Serial.print("aworld wrist\t");
//       Serial.print(aaWorld2.x);
//       Serial.print("\t");
//       Serial.print(aaWorld2.y);
//       Serial.print("\t");
//       Serial.println(aaWorld2.z);
//
//       delay(500);
//       mpuElbow.resetFIFO();
//       mpuWrist.resetFIFO();
//
//   }
// }
