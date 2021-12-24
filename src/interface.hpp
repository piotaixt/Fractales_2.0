#ifndef INTERFACE_H
#define INTERFACE_H

void CallBackFunc(int event, int x, int y, int, void *input);
void set_parameters_from_mouse(std::complex<double> *center, const cv::Mat *img,
                               float space);
void display(cv::Mat image);

#endif