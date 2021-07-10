#include <iostream>
#include <cmath>
#include <omp.h>
#include <complex>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void display(Mat image)
{
  imshow("image", image);
  waitKey(0);
  destroyAllWindows();
}

complex<double> get_C_number_from_pixel(int i, int j, const Mat *image)
{
  float zoom_factor = 3.5;
  double realpart = -0.7 + zoom_factor * (j - ((image->cols) / 2.0)) / (image->cols);
  double impart = zoom_factor * (-(i - (image->rows) / 2.0)) / (image->rows);
  complex<double> result(realpart, impart);
  return result;
}

Vec3f get_color_from_nb_iteration(int nb)
{
  float value_r;
  float value_g;
  float value_b;
  if ((nb % 300) < 50)
  {
    //Dégradé de noir vers blanc
    value_r = (nb % 50) / 50.0;
    value_g = (nb % 50) / 50.0;
    value_b = (nb % 50) / 50.0;
  }
  else if ((nb % 300) < 100)
  {
    //Dégradé de blanc vers bleu
    value_r = 1 - (nb % 50) / 50.0;
    value_g = 1 - (nb % 50) / 50.0;
    value_b = 1;
  }
  else if ((nb % 300) < 150)
  {
    //Dégradé de bleu vers violet
    value_r = (nb % 50) / 50.0;
    value_g = 0;
    value_b = 1;
  }
  else if ((nb % 300) < 200)
  {
    //Dégradé de violet vers rouge
    value_r = 1;
    value_g = 0;
    value_b = 1 - (nb % 50) / 50.00;
  }
  else if ((nb % 300) < 250)
  {
    //Dégradé de rouge vers orange
    value_r = 1;
    value_g = (165 / 255.0) * (nb % 50) / 50.0;
    value_b = 0;
  }
  else if ((nb % 300) < 300)
  {
    //Dégradé de orange vers noir
    value_r = 1 - (nb % 50) / 50.00;
    value_g = (165 / 255.0) - (165 / 255.0) * (nb % 50) / 50.0;
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
  pallette.convertTo(pallette, CV_32FC3, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1
#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < pallette.cols; j++)
  {
    Vec3f color = get_color_from_nb_iteration(j);
    for (int i = 0; i < pallette.rows; i++)
    {
      pallette.at<Vec3f>(i, j) = color;
    }
  }
  display(pallette);
  imwrite("images/pallette.tiff", pallette);
  cout << "Voila." << endl
       << "---------" << endl;
}

int main()
{
  int nb_iterations = 300;
  int NTHREADS = 1;
  int make_pal = 0;
  int dim_x = 1000;
  int dim_y = 1000;
  int option = 0;
  cout << "----------------------------------------" << endl
       << "Hello" << endl
       << "----------------------------------------" << endl
       << "Merci de ne pas faire l'autiste et de mettre des entiers autorisés." << endl
       << "Options:" << endl
       << "1 pour Mendelbrot" << endl
       << "2 pour la pallette de couleurs" << endl
       << "--------" << endl;
  cin >> option;
  if (option == 1)
  {
    cout << "Mendelbrot options: " << endl
         << "Largeur image en pixels ? " << endl;
    cin >> dim_x;
    cout << "Hauteur image en pixels ? " << endl;
    cin >> dim_y;
    cout << "Nombre d'itérations ?" << endl;
    cin >> nb_iterations;
    cout << "Nombre de threads ?" << endl;
    cin >> NTHREADS;
  }
  else if (option == 2)
  {
    make_pallette();
    return 0;
  }

  Mat img = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
  img.convertTo(img, CV_32FC3, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1

  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < img.rows; i++)
  {
    for (int j = 0; j < img.cols; j++)
    {
      complex<double> z = get_C_number_from_pixel(i, j, &img);
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
