#ifndef FRACTALE_H
#define FRACTALE_H

#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <math.h>
#include <omp.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <vector>

#include "Polynome.hpp"

template <typename T> class Fractale {

protected:
  cv::Mat image;
  std::vector<cv::Vec3f> colors;
  int length_color;
  int nb_iterations;
  int NTHREADS;
  int dim_x;
  int dim_y;
  float space;
  std::complex<T> center;

public:
  Fractale();
  void display_parameters();
  void display_image();
};

template <typename T> class Mendelbrot : public Fractale<T> {
public:
  Mendelbrot();
  void compute();
};

// template <typename T> class Julia : public Fractale<T> {
// private:
//   std::complex<T> c;

// public:
//   Julia();
//   void compute();
// };

// template <typename T> class Newton : public Fractale<T> {
// private:
//   Polynome<T> P;
//   std::vector<std::complex<T>> racines;

// public:
//   Newton(Polynome<T> poly, std::vector<std::complex<T>> racines);
//   compute();
// };

template <typename T> class Lyapunov : public Fractale<T> {
public:
  // compute(); // version Lyapunov
};

#endif