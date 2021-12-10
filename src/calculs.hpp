#ifndef CALCULS_H
#define CALCULS_H

#include <boost/program_options/option.hpp>
#include <complex>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

double norme_2(std::complex<double> z);

template <typename T>
std::complex<T> get_C_number_from_pixel(int i, int j, const cv::Mat *image,
                                        std::complex<T> center, float space) {
  // space donne la dimension de la zoombox (écart entre le centre et le bord de
  // la "zoombox" en x)
  float ratio =
      image->rows /
      (float)image->cols; // Pour gérer le ratio de la taille de l'image
  double realpart =
      center.real() + 2 * space * (j - ((image->cols) / 2.0)) / (image->cols);
  double impart = center.imag() + 2 * space * ratio *
                                      (-(i - (image->rows) / 2.0)) /
                                      (image->rows);
  std::complex<T> result(realpart, impart);
  return result;
}

template <typename T>
uint julia_iterations_of_C_number(std::complex<T> z, std::complex<T> c,
                                  int nb_iteration_max) {
  for (int i = 0; i < nb_iteration_max; i++) {
    z = z * z + c;
    if (abs(z) > 2) {
      return i;
    }
  }
  return nb_iteration_max;
}

cv::Vec3f get_color_from_nb_iteration(uint nb, uint nb_iteration_max,
                                      std::vector<cv::Vec3f> colors,
                                      uint length_color);

uint mendelbrot_iterations_of_C_number(std::complex<double> z,
                                       int nb_iteration_max);
// uint julia_iterations_of_C_number(std::complex<double> z,
//                                   std::complex<double> c, int
//                                   nb_iteration_max);
// void make_pallette(std::vector<cv::Vec3f> colors, uint length_color,
//                    uint nb_iteration_max);

#endif