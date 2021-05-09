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

Vec3f get_color_from_nb_iteration(int nb, int nb_max)
{
  float value;
  if ((nb % 100) < 50)
  {
    value = (nb % 50) / 50.0;
  }
  else
  {
    value = 1 - (nb % 50) / 50.0;
  }

  Vec3f intensity(value, value, value);
  // Vec3f intensity = img.at<Vec3f>(j, i);
  // cout << intensity << endl;
  // float blue = intensity.val[0];
  // float green = intensity.val[1];
  // float red = intensity.val[2];
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
      return get_color_from_nb_iteration(i, nb_iteration_max);
    }
  }
  Vec3f intensity(0, 0, 0);
  return intensity;
}

int main()
{
  int nb_iterations = 1000;
  int NTHREADS;
  cout << "Nombre de threads ?" << endl;
  cin >> NTHREADS;

  Mat img = imread("resources/base10k.png");
  img.convertTo(img, CV_32F, 1 / 255.0); //Image avec trois channels B,G,R codés entre 0 et 1
  
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
  cout << "Calculs faits en " << elapsed << " secondes." << endl;

  // display(img);
  imwrite("images/Mendelbrot.tiff", img);
  return 0;
}
