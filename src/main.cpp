#include "L3G.h"
#include "LSM303.h"
#include "matrix.hpp"
#include <Arduino.h>
#include <Wire.h>

struct ToSerialize {
  unsigned char header1 = 'y';
  unsigned char header2 = 'o';
  Vector<4> x; // TODO: should the biases at some point
  unsigned char tail = '\n';
} buf;

// state
auto &x = buf.x;
Matrix<4, 4> F_;

// covariances etc
float Q; // Q = K * I, so we just keep that one scalar (gyro)
float R; // same thing (accel)
Matrix<4, 4> P;

// gyro / accel
float w[] = {585.61, 358.12, 1018.55};
float bw[] = {19.72, -156.26, 2.02}; // TODO: constant for now
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

  gyro.init();
  gyro.enableDefault();

  compass.init();
  compass.enableDefault();

  x(0) = 1;
  F_(0) = 1;
  F_(5) = 1;
  F_(10) = 1;
  F_(15) = 1;
}

Matrix<4, 4> F_from_gyro(float gx, float gy, float gz, float dt) {
  auto ewx = dt * 0.5 * (gx - bw[0]) * PI * 250.0 / (180.0 * 32768.0);
  auto ewy = dt * 0.5 * (gy - bw[1]) * PI * 250.0 / (180.0 * 32768.0);
  auto ewz = dt * 0.5 * (gz - bw[2]) * PI * 250.0 / (180.0 * 32768.0);

  F_(1) = -ewx;
  F_(2) = -ewy;
  F_(3) = -ewz;

  F_(4) = ewx;
  F_(6) = ewz;
  F_(7) = -ewy;

  F_(8) = ewy;
  F_(9) = -ewz;
  F_(11) = ewx;

  F_(12) = ewz;
  F_(13) = ewy;
  F_(14) = -ewx;

  return F_;
}

int main() {
  setup();

  unsigned int count = 0;
  auto t = micros();
  for (;;) {
    gyro.read();
    // compass.read();

    auto nt = micros();
    auto F_ = F_from_gyro(gyro.g.x, gyro.g.y, gyro.g.z, (nt - t) * 1e-6);
    x = F_ * x;
    x.normalize();
    P = F_ * P * F_.transpose(); // add Q also :)

    if (++count == 100) {
      count = 0;
      Serial.write(reinterpret_cast<unsigned char *>(&buf), sizeof(buf));
    }

    t = nt;
  }
  return 0;
}
