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
  Polynome(std::vector<T> poly);
  T eval(T x);
  Polynome deriv();
};

#endif