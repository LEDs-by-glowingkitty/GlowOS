#include <Adafruit_LSM6DS3TRC.h>

Adafruit_LSM6DS3TRC lsm6ds3trc;


void init(){
    Serial.println("Adafruit LSM6DS3TR-C test!");

    if (!lsm6ds3trc.begin_I2C()) {
        Serial.println("Failed to find LSM6DS3TR-C chip");
        while (1) {
        delay(10);
        }
    }

    Serial.println("LSM6DS3TR-C Found!");

    lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
    lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2
}

void getdata() {
  // Get a new normalized sensor event
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();

  delay(100);
}