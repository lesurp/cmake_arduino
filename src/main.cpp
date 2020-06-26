#include "L3G.h"
#include "LSM303.h"
#include "matrix.hpp"
#include <Arduino.h>
#include <Wire.h>

// state
Vector<4> x; // TODO: should the biases at some point
Matrix<4, 4> F_;

// covariances etc
float Q; // Q = K * I, so we just keep that one scalar (gyro)
float R; // same thing (accel)
Matrix<4, 4> P;

// gyro / accel
float w[3];
float bw[3]; // TODO: constant for now
float a[3];
float ba[3];

// sensors
L3G gyro;
LSM303 compass;

// output buffer
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

  x(0) = 1;
  F_(0) = 1;
  F_(5) = 1;
  F_(10) = 1;
  F_(15) = 1;
}

void fill_F(Matrix<4, 4> &F_, float gx, float gy, float gz, float dt) {
  float ewx = dt * 0.5 * gx;
  float ewy = dt * 0.5 * gy;
  float ewz = dt * 0.5 * gz;

  F_(1) = -ewx;
  F_(2) = -ewy;
  F_(3) = -ewz;

  F_(6) = ewz;
  F_(7) = -ewy;

  F_(11) = ewx;

  F_(4) = -F_(1);
  F_(8) = -F_(2);
  F_(12) = -F_(3);

  F_(9) = -F_(6);
  F_(13) = -F_(7);

  F_(14) = -F_(8);
}

int main() {
  setup();

  auto t = micros();
  for (;;) {
    gyro.read();
    // compass.read();

    auto nt = micros();
    fill_F(F_, gyro.g.x, gyro.g.y, gyro.g.z, nt - t);
    x = F_ * x;
    x.normalize();
    P = F_ * P * F_.transpose();

    t = nt;
  }
  return 0;
}
