#include "L3G.h"
#include <Arduino.h>
#include <Wire.h>

L3G gyro;

void setup() {
  init();
  Serial.begin(9600);
  Wire.begin();
  gyro.init();
  gyro.enableDefault();
}

constexpr int N = 300;
float values[N];

void calib(float *data, float &mean, float &var) {
  float sum = 0.0;
  for (int i = 0; i < N; ++i) {
    sum += data[i];
  }
  mean = sum / N;

  float sum_squared = 0.0;
  for (int i = 0; i < N; ++i) {
    float delta = mean - data[i];
    sum_squared += delta * delta;
  }
  var = sum_squared / (N - 1); // unbiased
}

auto calib_with_fn(float (*f)(), char const *c) {
  Serial.print("Starting to calibrate: ");
  Serial.print(c);
  Serial.print("\n");

  for (int i = 0; i < N; ++i) {
    values[i] = f();
    delay(100);
  }

  Serial.print("Done!");
  Serial.print("\n");

  float mean;
  float var;

  calib(values, mean, var);

  return [=]() {
    Serial.print("\n");
    Serial.print(c);
    Serial.print("\n");
    Serial.print("\tbias: ");
    Serial.print(mean);
    Serial.print("\tvariance: ");
    Serial.print(var);
  };
}

float gx_fn() {
  gyro.read();
  return gyro.g.x;
}

float gy_fn() {
  gyro.read();
  return gyro.g.y;
}

float gz_fn() {
  gyro.read();
  return gyro.g.z;
}

int main() {
  setup();

  auto gx = calib_with_fn(gx_fn, "gyro x");
  auto gy = calib_with_fn(gy_fn, "gyro y");
  auto gz = calib_with_fn(gz_fn, "gyro z");

  for (;;) {
    gx();
    gy();
    gz();
  }
  return 0;
}
