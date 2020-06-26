#include "L3G.h"
#include "LSM303.h"
#include <Arduino.h>
#include <Wire.h>

// state
float x[4]; // TODO: should the biases at some point
float fx[4];

// gyro / accel
float w[3];
float bw[3]; // TODO: constant for now
float a[3];
float ba[3];

// covariances
float Q; // Q = K * I, so we just keep that one scalar (gyro)
float R; // same thing (accel)
float P[9];

// sensors
L3G gyro;
LSM303 compass;

// output buffer
char report[80];

void setup() {
  init();
  Serial.begin(9600);
  Wire.begin();

  if (!gyro.init()) {
    Serial.println("Failed to autodetect gyro type!");
    while (1)
      ;
  }

  gyro.enableDefault();
  compass.init();
  compass.enableDefault();
}

void f(float *x, float *w, float dt, float *fx) {
  auto wx2 = w[0] * w[0];
  auto wy2 = w[1] * w[1];
  auto wz2 = w[2] * w[2];

  // small angles assumption means [cos(a) sin(a)/|a| * dt*w/2]
  // becomes [1 w]
  float ew[4];
  ew[0] = 1;
  ew[1] = dt * 0.5 * w[0];
  ew[2] = dt * 0.5 * w[1];
  ew[3] = dt * 0.5 * w[2];

  fx[0] = x[0] * ew[0] - x[1] * ew[1] - x[2] * ew[2] - x[3] * ew[3];
  fx[1] = x[0] * ew[1] + x[1] * ew[0] + x[2] * ew[3] - x[3] * ew[2];
  fx[2] = x[0] * ew[2] - x[1] * ew[1] + x[2] * ew[0] + x[3] * ew[3];
  fx[3] = x[0] * ew[3] + x[1] * ew[2] - x[2] * ew[1] + x[3] * ew[0];
}

int main() {
  setup();

  for (;;) {
    gyro.read();

    Serial.print("G ");
    Serial.print("X: ");
    Serial.print((int)gyro.g.x);
    Serial.print(" Y: ");
    Serial.print((int)gyro.g.y);
    Serial.print(" Z: ");
    Serial.println((int)gyro.g.z);

    compass.read();

    snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
             compass.a.x, compass.a.y, compass.a.z, compass.m.x, compass.m.y,
             compass.m.z);

    Serial.println(report);

    delay(100);
  }
  return 0;
}
