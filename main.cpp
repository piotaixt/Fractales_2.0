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

#include "calculs.hpp"
#include "interface.hpp"
#include "options.hpp"

using namespace std;
using namespace cv;
namespace po = boost::program_options;


int main(int argc, char **argv)
{
  // Pour que "cout" ne fasse pas un arrondi lorsqu'il affiche un double.
  typedef numeric_limits< double > dbl;
  cout.precision(dbl::max_digits10);


  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;


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

  // Declare the supported options.
  po::options_description desc("Options possibles : ");
  desc.add_options()("help", "Afficher les options")("size_x", po::value<int>(), "int : Largeur de l'image en pixels")("size_y", po::value<int>(), "int : Hauteur de l'image en pixels")("nbr_iter", po::value<int>(), "int : Nombre maximal d'itérations")("nbr_threads", po::value<int>(), "int : Nombre de threads utilisés")("size_zoombox", po::value<double>(), "double : Demi largeur de la zoombox (conseillé de prendre dans l'intervalle [0,2])")("center_x", po::value<double>(), "double : Centre du zoom en x")("center_y", po::value<double>(), "double : Centre du zoom en y")("pallette", "Afficher la pallette de couleurs")("length_color", po::value<uint>(), "int : Nombre d'itérations par changement de couleurs")("write_img", po::value<string>(), "string : Mode ecriture d'une image, donner le nom souhaité");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << "\n";
    return 1;
  }

  if (vm.count("size_x"))
  {
    dim_x = vm["size_x"].as<int>();
  }

  if (vm.count("size_y"))
  {
    dim_y = vm["size_y"].as<int>();
  }

  if (vm.count("nbr_iter"))
  {
    nb_iterations = vm["nbr_iter"].as<int>();
  }

  if (vm.count("nbr_threads"))
  {
    NTHREADS = vm["nbr_threads"].as<int>();
  }

  if (vm.count("size_zoombox"))
  {
    space = vm["size_zoombox"].as<double>();
  }

  if (vm.count("center_x"))
  {
    complex<double> c1(vm["center_x"].as<double>(), center.imag());
    center = c1;
  }

  if (vm.count("center_y"))
  {
    complex<double> c2(center.real(), vm["center_y"].as<double>());
    center = c2;
  }

  if (vm.count("length_color"))
  {
    length_color = vm["length_color"].as<uint>();
  }

  if (vm.count("pallette"))
  {
    make_pallette(colors, length_color, nb_iterations);
    return 1;
  }

  display_param(dim_x, dim_y, nb_iterations, NTHREADS, space, length_color, center);

  Mat img = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
  img.convertTo(img, CV_32FC3, 1 / 255.0);

  omp_set_num_threads(NTHREADS);

  bool A = true;
  while (A)
  {
    auto start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.rows; i++)
    {
      int n_iter = 0;
      for (int j = 0; j < img.cols; j++)
      {
        complex<double> z = get_C_number_from_pixel(i, j, &img, center, space);
        n_iter = iterations_of_C_number(z, nb_iterations);
        img.at<Vec3f>(i, j) = get_color_from_nb_iteration(n_iter, nb_iterations, colors, length_color);
      }
    }

    auto stop = omp_get_wtime();
    auto elapsed = chrono::duration<double>(stop - start).count();
    cout << "----------------------------------------" << endl
         << "Calculs faits en " << elapsed << " secondes." << endl
         << "----------------------------------------" << endl;

    if (vm.count("write_img"))
    {
      string name = "images/" + vm["write_img"].as<string>() + ".tiff";
      imwrite(name, img);
      A=false;
    }
    

    set_parameters_from_mouse(&center, &img, space);
    space = space / 2;
    display_param(dim_x, dim_y, nb_iterations, NTHREADS, space, length_color, center);
  }

  return 0;
}
