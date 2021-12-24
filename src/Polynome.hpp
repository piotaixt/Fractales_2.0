#ifndef POLYNOME_H
#define POLYNOME_H

// #include <math.h>
// #include <stdlib.h>
// #include <exception>

// #include <cmath>
// #include <complex>
// #include <iostream>
// #include <limits>
// #include <vector>

#include "calculs.hpp"

template <class T>
class Polynome
{
private:
  // Le premier coeff est le dominant, le dernier est le terme constant
  std::vector<T> coefficients;
  int deg;

public:
  Polynome();
  Polynome(std::vector<T> poly);
  std::complex<T> eval(std::complex<T> x);
  Polynome deriv();
  std::vector<std::complex<T>> racines();
  int getDeg();
  void print();
  // void display();
};

///// Implémentation

template <typename T>
Polynome<T>::Polynome()
{
  coefficients = {0};
  deg = 0;
}

template <typename T>
Polynome<T>::Polynome(std::vector<T> poly)
{
  coefficients = poly;
  deg = poly.size() - 1;
}

template <typename T>
int Polynome<T>::getDeg()
{
  return deg;
}

template <typename T>
void Polynome<T>::print()
{
  int n = deg;
  for (auto const &e : coefficients)
  {
    if (n > 0)
    {
      std::cout << e << ".X^" << n << " + ";
      n = n - 1;
    }
    else
    {
      std::cout << e;
    }
  }
  std::cout << std::endl;
}

// template <typename T> void Polynome<T>::display() {
//   std::cout << "Polynome:";
//   for (T it = begin(coefficients); it != end(coefficients); ++it) {
//     std::cout << it;
//   }
//   std::cout << std::endl;
// }

// Horner's Method
template <typename T>
std::complex<T> Polynome<T>::eval(std::complex<T> x)
{
  std::complex<T> res;
  res.real(0);
  res.imag(0);
  if (!coefficients.empty())
  {
    res += coefficients[0];
    for (int i = 1; i < coefficients.size(); i++)
    {
      res = res * x + coefficients[i];
    }
  }
  return res;
}

template <typename T>
Polynome<T> Polynome<T>::deriv()
{
  int n = deg;
  std::vector<T> poly_deriv;
  for (auto const &e : coefficients)
  {
    if (n>0)
    {
      poly_deriv.push_back(e * n);
      n = n - 1;
    }
  }
  return Polynome(poly_deriv);
}

template <typename T>
std::vector<std::complex<T>> Polynome<T>::racines()
{
  Polynome<T> P(coefficients);
  Polynome<T> p_deriv = P.deriv();

  double prec = 0.0001;
  std::vector<std::complex<T>> vec_rac;
  std::complex<T> z;
  std::complex<T> z_prev;
  int M = 1;
  int nb_iterations_max = 10000;
  int nb_points = 100000;
  bool first_rac = true;
  bool rac_is_found;
  while (M < 50)
  {
    for (int i = 0; i < nb_points; i++)
    {
      z.real(fRand(-M, M));
      z.imag(fRand(-M, M));

      rac_is_found = false;
      int k = 0;
      while (!rac_is_found && k < nb_iterations_max)
      {
        z = z - (P.eval(z) / p_deriv.eval(z));

        if (first_rac && (norme_2(P.eval(z)) < prec))
        {
          vec_rac.push_back(z);
          first_rac = false;
        }

        if (!first_rac)
        {

          for (int l = 0; l < vec_rac.size(); ++l)
          {
            if (norme_2(z - vec_rac[l]) < prec)
            {
              rac_is_found = true;
              break;
            }
          }

          if (!rac_is_found && norme_2(P.eval(z)) < prec)
          {
            vec_rac.push_back(z);
          }
        }
        if (norme_2(P.eval(z)) > 100)
        {
          break;
        }

        k++;
      }

      if (vec_rac.size() == P.deg)
      {
        return vec_rac;
      }
    }
    M++;
  }

  if (vec_rac.size() != P.deg)
  {
    std::cout << "Erreur de calcul des racines." << std::endl;
  }

  return vec_rac;
}

#endif