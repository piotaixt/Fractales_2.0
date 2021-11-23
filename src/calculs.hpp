#ifndef CALCULS_H
#define CALCULS_H

#include <stdlib.h>
#include <complex>
#include <opencv2/opencv.hpp>
#include <boost/program_options/option.hpp>

std::complex<double> get_C_number_from_pixel(int i, int j, const cv::Mat *image, std::complex<double> center, float space);
cv::Vec3f get_color_from_nb_iteration(uint nb, uint nb_iteration_max, std::vector<cv::Vec3f> colors, uint length_color);
uint iterations_of_C_number(std::complex<double> z, int nb_iteration_max);
void make_pallette(std::vector<cv::Vec3f> colors, uint length_color, uint nb_iteration_max);


#endif