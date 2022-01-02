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
  
  int nb_iterations_max = 10000;
  int nb_points = 1000;
  T module_z;
  bool first_rac = true;
  bool rac_is_found;
  
  int M = 1;
  while (M < 50)
  {
    // Pour tous les points tirrés au hasard
    for (int i = 0; i < nb_points; i++)
    {
      z.real(fRand(-M, M));
      z.imag(fRand(-M, M));
      rac_is_found = false;
      // Tant qu'on trouve pas une racine on itère la méthode de Newton
      int k = 0;
      while (!rac_is_found && k < nb_iterations_max)
      {
        z = z - (P.eval(z) / p_deriv.eval(z));
        module_z = norme_2(P.eval(z));
        // On recherche la première racine si ça converge vers un antécédent de 0
        if (first_rac && module_z < prec)
        {
          vec_rac.push_back(z);
          first_rac = false;
        }
        //Si on a déjà trouvé une racine on regarde vers où ça converge
        if (!first_rac)
        {
          rac_is_found = is_already_found(vec_rac, z, prec);
          if (!rac_is_found && module_z < prec)
            vec_rac.push_back(z);
        }
        // Si ça diverge trop loin on s'arrète
        rac_is_found = module_z > 100 ? true : false;
        k++;
      }
      if (vec_rac.size() == P.deg)
      {
        return vec_rac;
      }
    }
    M++;
  }
  return vec_rac;
}

#endif