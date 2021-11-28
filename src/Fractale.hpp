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

class Fractale {

protected:
  cv::Mat image;
  std::vector<cv::Vec3f> colors;
  int length_color;
  int nb_iterations;
  int NTHREADS;
  int dim_x;
  int dim_y;
  float space;
  std::complex<double> center;

public:
  Fractale();
  void display_parameters();
  void display_image();
};

class Mendelbrot : public Fractale {
public:
  Mendelbrot();
  void compute();
};

class Julia : public Fractale {
private:
  std::complex<double> c;

public:
  Julia();
  void compute();
};

class Newton : public Fractale {
public:
  // compute(); // version newton
};

class Lyapunov : public Fractale {
public:
  // compute(); // version Lyapunov
};

#endif