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
#include "options.hpp"

using namespace std;
using namespace cv;
namespace po = boost::program_options;

int main(int argc, char **argv) {
  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;

  // Mendelbrot mendel_1;

  // // bool A = true;
  // // while (A) {
  // mendel_1.display_parameters();
  // mendel_1.compute();
  // mendel_1.display_image();
  // //   A = false;
  // // }

  Julia julia_1;
  julia_1.display_parameters();
  julia_1.compute();
  julia_1.display_image();

  return 0;
}
