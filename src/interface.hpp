#ifndef INTERFACE_H
#define INTERFACE_H

#include <complex>
#include <opencv2/opencv.hpp>

void CallBackFunc(int event, int x, int y, int, void *input);
void set_parameters_from_mouse(std::complex<double> *center, const cv::Mat *img, float space);
void display(cv::Mat image);
void display_param(int dim_x, int dim_y, int nb_iterations, int NTHREADS, float space, uint length_color, std::complex<double> center);


#endif