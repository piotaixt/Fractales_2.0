#include <iostream>
#include <cmath>
#include <omp.h>
#include <complex>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;
using namespace cv;
namespace po = boost::program_options;

void display(Mat image)
{
  imshow("image", image);
  waitKey(0);
  destroyAllWindows();
}

complex<double> get_C_number_from_pixel(int i, int j, const Mat *image, complex<double> center, float space)
{
  //space donne la dimension de la zoombox (écart entre le centre et le bord de la "zoombox" en x)
  float ratio = image->rows / (float)image->cols; // Pour gérer le ratio de la taille de l'image
  double realpart = center.real() + 2 * space * (j - ((image->cols) / 2.0)) / (image->cols);
  double impart = center.imag() + 2 * space * ratio * (-(i - (image->rows) / 2.0)) / (image->rows);
  complex<double> result(realpart, impart);
  return result;
}

Vec3f get_color_from_nb_iteration(int nb)
{
  float value_r;
  float value_g;
  float value_b;
  int total = 150;
  int m = 25;
  if ((nb % total) < (float)m)
  {
    //Dégradé de noir vers blanc
    value_r = (nb % m) / (float)m;
    value_g = (nb % m) / (float)m;
    value_b = (nb % m) / (float)m;
  }
  else if ((nb % total) < 2 * (float)m)
  {
    //Dégradé de blanc vers bleu
    value_r = 1 - (nb % m) / (float)m;
    value_g = 1 - (nb % m) / (float)m;
    value_b = 1;
  }
  else if ((nb % total) < 3 * (float)m)
  {
    //Dégradé de bleu vers noir
    value_r = 0;
    value_g = 0;
    value_b = 1 - (nb % m) / (float)m;
  }
  else if ((nb % total) < 4 * (float)m)
  {
    //Dégradé de noir vers rouge
    value_r = (nb % m) / (float)m;
    value_g = 0;
    value_b = 0;
  }
  else if ((nb % total) < 5 * (float)m)
  {
    //Dégradé de rouge vers orange
    value_r = 1;
    value_g = (165 / 255.0) * (nb % m) / (float)m;
    value_b = 0;
  }
  else if ((nb % total) < 6 * (float)m)
  {
    //Dégradé de orange vers noir
    value_r = 1 - (nb % m) / (float)m;
    value_g = (165 / 255.0) - (165 / 255.0) * (nb % m) / (float)m;
    value_b = 0;
  }
  Vec3f intensity(value_b, value_g, value_r);
  return intensity;
}

Vec3f color_of_C_number(complex<double> z, int nb_iteration_max)
{
  complex<double> c = z;
  for (int i = 0; i < nb_iteration_max; i++)
  {
    z = z * z + c;
    if (abs(z) > 2)
    {
      return get_color_from_nb_iteration(i);
    }
  }
  Vec3f intensity(0, 0, 0);
  return intensity;
}

void make_pallette()
{
  int dim_x = 1000;
  int dim_y = 200;
  Mat pallette = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
  // pallette.convertTo(pallette, CV_32FC3, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1
#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < pallette.cols; j++)
  {
    Vec3f color = get_color_from_nb_iteration(j);
    for (int i = 0; i < pallette.rows; i++)
    {
      pallette.at<Vec3f>(i, j) = color;
    }
  }
  // display(pallette);
  imwrite("images/pallette.tiff", pallette);
  auto truc = imread("images/pallette.tiff");
  display(truc);
  cout << "Voila." << endl
       << "---------" << endl;
}

int main(int argc, char **argv)
{
  int option = 0;

  //Param de calcul
  int nb_iterations = 300;
  int NTHREADS = 1;

  //Param de l'image
  int dim_x = 1000;
  int dim_y = 1000;

  //Param du zoom
  // complex<double> center(-0.7, 0); // Centre approx figure globale
  complex<double> center(0.743643887037151, 0.13182590420533);
  float space = 0.1;

  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help", "Afficher les options")("size_x", po::value<int>(), "Largeur de l'image en pixels")("size_y", po::value<int>(), "Hauteur de l'image en pixels")("nbr_iter", po::value<int>(), "Nombre maximal d'itérations")("nbr_threads", po::value<int>(), "Nombre de threads utilisés")("size_zoombox", po::value<double>(), "Demi largeur de la zoombox")("center_x", po::value<double>(), "Centre du zoom en x")("center_y", po::value<double>(), "Centre du zoom en y")("pallette", po::value<bool>(), "Afficher la pallette de couleurs");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << "\n";
    return 1;
  }

  if (vm.count("pallette"))
  {
    make_pallette();
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

  cout << "Taille de l'image : " << dim_x << " x " << dim_y << endl
       << "Nombre d'itérations : " << nb_iterations << endl
       << "Nombre de threads : " << NTHREADS << endl
       << "Taille de la zoombox : " << 2 * space << " x " << 2 * space * dim_y / dim_x << endl
       << "Centre du zoom : " << center.real() << " + " << center.imag() << "i" << endl;

  Mat img = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
  img.convertTo(img, CV_32FC3, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1

  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < img.rows; i++)
  {
    for (int j = 0; j < img.cols; j++)
    {
      complex<double> z = get_C_number_from_pixel(i, j, &img, center, space);
      img.at<Vec3f>(i, j) = color_of_C_number(z, nb_iterations);
    }
  }

  auto stop = omp_get_wtime();
  auto elapsed = chrono::duration<double>(stop - start).count();
  cout << "----------------------------------------" << endl
       << "Calculs faits en " << elapsed << " secondes." << endl
       << "----------------------------------------" << endl;

  // display(img);
  imwrite("images/Mendelbrot.tiff", img);

  return 0;
}
