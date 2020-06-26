#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <math.h>

template <typename T> struct MatrixTranspose;

template <typename Concrete> struct MatrixBase {
  static int constexpr row() { return Concrete::row(); }
  static int constexpr col() { return Concrete::col(); }
  float operator()(int idx) const {
    return static_cast<Concrete const *>(this)[idx];
  }
  float &operator()(int idx) { return static_cast<Concrete *>(this)[idx]; }

  float operator()(int i, int j) const {
    return static_cast<Concrete const *>(this)[i];
  }
  float &operator()(int i, int j) { return static_cast<Concrete *>(this)[i]; }

  MatrixTranspose<Concrete> transpose(); // { return MatrixTranspose(*this); }
};

template <int R, int C> struct Matrix : MatrixBase<Matrix<R, C>> {
  static int constexpr row() { return R; }
  static int constexpr col() { return C; }
  float data[R * C] = {0};
  float operator()(int idx) const { return data[idx]; }
  float &operator()(int idx) { return data[idx]; }
  float &operator()(int i, int j) { return data[i * C + j]; }
  float operator()(int i, int j) const { return data[i * C + j]; }

  float squared_norm() const {
    float sqn = 0;
    for (int i = 0; i < R * C; ++i) {
      sqn += data[i] * data[i];
    }
    return sqn;
  }

  float norm() const { return sqrt(squared_norm()); }

  void normalize() {
    auto n = 1.0 / norm();
    for (int i = 0; i < R * C; ++i) {
      data[i] *= n;
    }
  }
};

template <typename T>
struct MatrixTranspose : public MatrixBase<MatrixTranspose<T>> {
  static int constexpr row() { return T::col(); }
  static int constexpr col() { return T::row(); }
  MatrixTranspose(T &t) : t(t) {}
  T &t;
  float operator()(int i) const {
    int c = i % this->col();
    int r = (i - c) / this->col();
    return t(c * this->row() + r);
  }
  float &operator()(int i) {
    int c = i % this->col();
    int r = (i - c) / this->col();
    return t(c * this->row() + r);
  }

  float operator()(int i, int j) const { return t(j, i); }
  float &operator()(int i, int j) { return t(j, i); }
};

template <int R> using Vector = Matrix<R, 1>;

template <typename Lhs, typename Rhs>
auto operator*(Lhs const &lhs, Rhs const &rhs) {
  static_assert(Lhs::col() == Rhs::row(), "Wrong matrix sizes!");
  Matrix<Lhs::row(), Rhs::col()> out;
  for (int i = 0; i < lhs.row(); ++i) {
    for (int j = 0; j < rhs.col(); ++j) {
      float sum = 0;
      for (int k = 0; k < lhs.col(); ++k) {
        sum += lhs(i, k) * rhs(k, j);
      }
      out(i, j) = sum;
    }
  }
  return out;
}
template <typename Concrete>
MatrixTranspose<Concrete> MatrixBase<Concrete>::transpose() {
  return MatrixTranspose<Concrete>(*static_cast<Concrete *>(this));
}

#endif
