#ifndef POLYNOME_H
#define POLYNOME_H

#include <math.h>
#include <stdlib.h>

#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <vector>

template <class T> class Polynome {
private:
  // Le premier coeff est le dominant, le dernier est le terme constant
  std::vector<T> coefficients;
  int deg;

public:
  Polynome();
  Polynome(std::vector<T> poly);
  std::complex<T> eval(std::complex<T> x);
  Polynome deriv();
  // void display();
};

///// Implémentation

template <typename T> Polynome<T>::Polynome() {
  coefficients = {0};
  deg = 0;
}

template <typename T> Polynome<T>::Polynome(std::vector<T> poly) {
  coefficients = poly;
  deg = poly.size() - 1;
}

// template <typename T> void Polynome<T>::display() {
//   std::cout << "Polynome:";
//   for (T it = begin(coefficients); it != end(coefficients); ++it) {
//     std::cout << it;
//   }
//   std::cout << std::endl;
// }

// Horner's Method
template <typename T> std::complex<T> Polynome<T>::eval(std::complex<T> x) {
  std::complex<T> res;
  res.real(0);
  res.imag(0);
  if (!coefficients.empty()) {
    res += coefficients[0];
    for (int i = 1; i < coefficients.size(); i++) {
      res = res * x + coefficients[i];
    }
  }
  return res;
}

template <typename T> Polynome<T> Polynome<T>::deriv() {
  int n = deg;
  std::vector<T> poly_deriv;
  for (int i = 0; i <= coefficients.size(); ++i) {
    poly_deriv.push_back(coefficients[i] * n);
    n = n - 1;
  }
  return Polynome(poly_deriv);
}

#endif