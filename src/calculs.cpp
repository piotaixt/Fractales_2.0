#include <math.h>

#include "calculs.hpp"
#include "interface.hpp"

using namespace std;
using namespace cv;
namespace po = boost::program_options;

double norme_2(complex<double> z) {
  return sqrt(pow(z.real(), 2) + pow(z.imag(), 2));
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// template <typename T>
// complex<T> get_C_number_from_pixel(int i, int j, const Mat *image,
//                                    complex<T> center, float space) {
//   // space donne la dimension de la zoombox (écart entre le centre et le bord
//   de
//   // la "zoombox" en x)
//   float ratio =
//       image->rows /
//       (float)image->cols; // Pour gérer le ratio de la taille de l'image
//   double realpart =
//       center.real() + 2 * space * (j - ((image->cols) / 2.0)) /
//       (image->cols);
//   double impart = center.imag() + 2 * space * ratio *
//                                       (-(i - (image->rows) / 2.0)) /
//                                       (image->rows);
//   complex<T> result(realpart, impart);
//   return result;
// }

Vec3f get_color_from_nb_iteration(uint nb, uint nb_iteration_max,
                                  vector<Vec3f> colors, uint length_color) {
  if (nb == nb_iteration_max) {
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
  if (num_c < colors.size() - 1) {
    diff_b = colors.at(num_c + 1)(0) - colors.at(num_c)(0);
    diff_g = colors.at(num_c + 1)(1) - colors.at(num_c)(1);
    diff_r = colors.at(num_c + 1)(2) - colors.at(num_c)(2);
  } else {
    diff_b = colors.at(0)(0) - colors.at(num_c)(0);
    diff_g = colors.at(0)(1) - colors.at(num_c)(1);
    diff_r = colors.at(0)(2) - colors.at(num_c)(2);
  }
  value_b =
      colors.at(num_c)(0) + diff_b * (nb % length_color) / (float)length_color;
  value_g =
      colors.at(num_c)(1) + diff_g * (nb % length_color) / (float)length_color;
  value_r =
      colors.at(num_c)(2) + diff_r * (nb % length_color) / (float)length_color;
  Vec3f intensity(value_b, value_g, value_r);
  return intensity;
}

uint mendelbrot_iterations_of_C_number(complex<double> z,
                                       int nb_iteration_max) {
  complex<double> c = z;
  for (int i = 0; i < nb_iteration_max; i++) {
    z = z * z + c;
    if (abs(z) > 2) {
      return i;
    }
  }
  return nb_iteration_max;
}

// void make_pallette(vector<Vec3f> colors, uint length_color,
//                    uint nb_iteration_max) {
//   int dim_x = 3000;
//   int dim_y = 200;
//   Mat pallette = Mat::ones(Size(dim_x, dim_y), CV_32FC3);
//   // pallette.convertTo(pallette, CV_32FC3, 1 / 255.0); //Image avec trois
//   // channels B,G,R codés entre 0 et 1
// #pragma omp parallel for schedule(dynamic, 1)
//   for (int j = 0; j < pallette.cols; j++) {
//     Vec3f color =
//         get_color_from_nb_iteration(j, nb_iteration_max, colors,
//         length_color);
//     for (int i = 0; i < pallette.rows; i++) {
//       pallette.at<Vec3f>(i, j) = color;
//     }
//   }
//   // display(pallette);
//   imwrite("images/pallette.tiff", pallette);
//   auto truc = imread("images/pallette.tiff");
//   display(truc);
//   cout << "Voila." << endl << "---------" << endl;
// }