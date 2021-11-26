
#include <iostream>
#include <cmath>
#include <math.h>
#include <omp.h>
#include <complex>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <limits>

using namespace std;
using namespace cv;
namespace po = boost::program_options;

class Fractale {

private:
    vector<Vec3f> colors;
    uint length_color;
    int nb_iterations = 300;
  int NTHREADS = 1;
    


  //Couleurs
  vector<Vec3f> colors = {
      Vec3f(0, 0, 0),
      Vec3f(0, 0, 0.01),
      Vec3f(0, 0, 1),
      Vec3f(0, 0.5, 1),
      Vec3f(0, 1, 1),
      Vec3f(1, 1, 1),
      Vec3f(1, 0.3, 0),
      Vec3f(1, 0, 0)};

  uint length_color = 2;

  //Param de calcul
  int nb_iterations = 300;
  int NTHREADS = 1;

  //Param de l'image
  int dim_x = 1000;
  int dim_y = 1000;

  //Param du zoom
  complex<double> center(-0.7, 0); // Centre approx figure globale
  // complex<double> center(-0.743643887037151, 0.13182590420533); // Un point particulierintéressant
  float space = 1.5;

}

class Mendelbrot : public Fractale{

}