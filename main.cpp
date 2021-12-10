#include <math.h>
#include <omp.h>
#include <stdlib.h>

#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <opencv2/opencv.hpp>

#include "Fractale.hpp"
#include "Polynome.cpp"
#include "interface.hpp"
// #include "options.hpp"

using namespace std;
using namespace cv;
namespace po = boost::program_options;

int main(int argc, char **argv) {
  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;

  Mendelbrot<double> mendel_1;
  mendel_1.display_parameters();
  mendel_1.compute();
  mendel_1.display_image();

  Mendelbrot<float> mendel_2;
  mendel_2.display_parameters();
  mendel_2.compute();
  mendel_2.display_image();

  complex<double> c;
  c.real(-0.70176);
  c.imag(-0.3842);
  Julia<double> julia_1(c);
  julia_1.display_parameters();
  julia_1.compute();
  julia_1.display_image();

  return 0;
}
