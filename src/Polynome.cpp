#include <math.h>
#include <stdlib.h>

#include "Polynome.hpp"
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

template <typename T> Polynome<T>::Polynome(vector<T> poly) {
  coefficients = poly;
  deg = poly.size() - 1;
}

// Horner's Method
template <typename T> T Polynome<T>::eval(T x) {
  T res;
  if (coefficients.empty()) {
    res = 0;
  } else {
    res = coefficients[0];
    for (int i = 1; i < coefficients.size(); i++) {
      res = res * x + coefficients[i];
    }
  }
  return res;
}

template <typename T> Polynome<T> Polynome<T>::deriv() {
  int n = deg;
  vector<T> poly_deriv;
  for (auto it = coefficients.begin(); it != coefficients.end() - 1; ++it) {
    poly_deriv.push_back(coefficients[it] * n);
    n = n - 1;
  }
  return Polynome(poly_deriv);
}