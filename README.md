# arduino-arm

Sudo Code
---------
connect to devices
  wait for robot
  wait for person

calibrate the home values for the sensors
  make a class for each of the two sensors that contain x, y, x, ax, ay, az
  initialize the classes with the initial start values

update the position
  get new sensor data
  put data into the objects
  find the direction vector between the shoulder (0, 0, 0) and the elbow
  find the direction vector between the elbow and the wrist

make classes for all the servo motors
pass in base server positions
update the servo motors
  calculate the angles on the x, y, and z for the two direction vectors
  pass them into the servos
