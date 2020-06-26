#include "matrix.hpp"
#include <Arduino.h>

int failed = 0;
int total = 0;

void assert(bool b, char const *c) {
  ++total;
  Serial.print(c);
  if (b) {
    Serial.println(": OK");
  } else {
    ++failed;
    Serial.println(": FAILURE");
  }
}

template <typename L, typename R> void assert_eq(L l, R r, char const *c) {
  ++total;
  Serial.print(c);
  if (l == r) {
    Serial.print(": OK\n");
  } else {
    ++failed;
    Serial.print(": FAILURE\n");
    Serial.print("\tlhs: ");
    Serial.println(l);
    Serial.print("\trhs: ");
    Serial.println(r);
  }
}

template <typename L, typename R> void assert_eq(L l, R r) {
  ++total;
  Serial.print("ASSERT EQUAL");
  if (l == r) {
    Serial.print(": OK\n");
  } else {
    ++failed;
    Serial.print(": FAILURE\n");
    Serial.print("\tlhs: ");
    Serial.println(l);
    Serial.print("\trhs: ");
    Serial.println(r);
  }
}

void setup() {
  init();
  Serial.begin(9600);
}

int main() {
  setup();

  for (;;) {
    failed = 0;
    total = 0;
    Serial.println("###########################");
    Serial.println("BEGIN TESTS");

    Matrix<2, 3> A;
    A(0) = 1;
    A(1) = 2;
    A(2) = 3;
    A(3) = 4;
    A(4) = 5;
    A(5) = 6;

    auto At = A.transpose();
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 3; ++j) {
        assert_eq(A(i * 3 + j), At(j * 2 + i), "A / At: idx op");
        assert_eq(A(i, j), At(j, i), "A / At: ij op");
      }
    }

    Vector<2> x;
    x(0) = 7;
    x(1) = 8;

    Vector<3> b = A.transpose() * x;
    Vector<3> b2 = At * x;
    assert_eq(b(0), 39, "b0");
    assert_eq(b(1), 54, "b1");
    assert_eq(b(2), 69, "b2");

    assert_eq(b(0), b2(0));
    assert_eq(b(1), b2(1));
    assert_eq(b(2), b2(2));

    Serial.print("END TESTS: ");
    Serial.print(failed);
    Serial.print(" assertions failed (");
    Serial.print(total);
    Serial.println(" assertions ran)");
  }
  return 0;
}
