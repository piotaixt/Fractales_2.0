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

using namespace std;
using namespace cv;
namespace po = boost::program_options;

complex<double> get_C_number_from_pixel(int i, int j, const Mat *image, complex<double> center, float space);

void CallBackFunc(int event, int x, int y, int, void *input)
{
  if (event == EVENT_LBUTTONDOWN)
  {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ") - Press any key to valid" << endl;
    Point P(x, y);
    Point *p = (Point *)input;
    *p = P;
  }
  else if (event == EVENT_RBUTTONDOWN)
  {
    // cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  }
  else if (event == EVENT_MBUTTONDOWN)
  {
    // cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  }
  else if (event == EVENT_MOUSEMOVE)
  {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
  }
}

void set_parameters_from_mouse(complex<double> *center, const Mat *img, float space)
{
  Point pixel;
  //Create a window
  namedWindow("My Window", 1);
  //set the callback function for any mouse event
  setMouseCallback("My Window", CallBackFunc, &pixel);
  //show the image
  imshow("My Window", *img);
  // Wait until user press some key
  waitKey(0);

  complex<double> new_center = get_C_number_from_pixel(pixel.y, pixel.x, img, *center, space);
  *center = new_center;

}

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

Vec3f get_color_from_nb_iteration(uint nb, uint nb_iteration_max, vector<Vec3f> colors, uint length_color)
{
  if (nb == nb_iteration_max)
  {
    Vec3f intensity(0, 0, 0);
    return intensity;
  }

  float value_r;
  float value_g;
  float value_b;
  int total = length_color * colors.size();
  int num_c = (nb % total) / length_color;
  float diff_b;
  float diff_g;
  float diff_r;
  if (num_c < colors.size() - 1)
  {
    diff_b = colors.at(num_c + 1)(0) - colors.at(num_c)(0);
    diff_g = colors.at(num_c + 1)(1) - colors.at(num_c)(1);
    diff_r = colors.at(num_c + 1)(2) - colors.at(num_c)(2);
  }
  else
  {
    diff_b = colors.at(0)(0) - colors.at(num_c)(0);
    diff_g = colors.at(0)(1) - colors.at(num_c)(1);
    diff_r = colors.at(0)(2) - colors.at(num_c)(2);
  }
  value_b = colors.at(num_c)(0) + diff_b * (nb % length_color) / (float)length_color;
  value_g = colors.at(num_c)(1) + diff_g * (nb % length_color) / (float)length_color;
  value_r = colors.at(num_c)(2) + diff_r * (nb % length_color) / (float)length_color;
  Vec3f intensity(value_b, value_g, value_r);
  return intensity;
}

uint iterations_of_C_number(complex<double> z, int nb_iteration_max)
{
  complex<double> c = z;
  for (int i = 0; i < nb_iteration_max; i++)
  {
    z = z * z + c;
    if (abs(z) > 2)
    {
      return i;
    }
  }
  return nb_iteration_max;
}

void make_pallette(vector<Vec3f> colors, uint length_color, uint nb_iteration_max)
{
  int dim_x = 1000;
  int dim_y = 200;
  Mat pallette = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
  // pallette.convertTo(pallette, CV_32FC3, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1
#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < pallette.cols; j++)
  {
    Vec3f color = get_color_from_nb_iteration(j, nb_iteration_max, colors, length_color);
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

void display_param(int dim_x, int dim_y, int nb_iterations, int NTHREADS, float space, uint length_color, complex<double> center)
{
  cout << "Taille de l'image : " << dim_x << " x " << dim_y << endl
       << "Nombre d'itérations : " << nb_iterations << endl
       << "Nombre de threads : " << NTHREADS << endl
       << "Taille de la zoombox : " << 2 * space << " x " << 2 * space * dim_y / dim_x << endl
       << "Persistance d'une couleur : " << length_color << endl
       << "Centre du zoom : " << center.real() << " + " << center.imag() << "i" << endl;
}

int main(int argc, char **argv)
{
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
  uint length_color = 25;

  //Param de calcul
  int nb_iterations = 300;
  int NTHREADS = 1;

  //Param de l'image
  int dim_x = 1000;
  int dim_y = 1000;

  //Param du zoom
  // complex<double> center(-0.7, 0); // Centre approx figure globale
  complex<double> center(-0.743643887037151, 0.13182590420533);
  float space = 0.1;

  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;

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
      break;
    }

    set_parameters_from_mouse(&center, &img, space);
    space = space / 2;
    display_param(dim_x, dim_y, nb_iterations, NTHREADS, space, length_color, center);
  }

  return 0;
}
