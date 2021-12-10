#ifndef FRACTALE_H
#define FRACTALE_H

#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <limits>
#include <math.h>
#include <omp.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#include "Polynome.hpp"
#include "calculs.hpp"
#include "interface.hpp"

template <typename T> class Mendelbrot {

private:
  cv::Mat image;
  std::vector<cv::Vec3f> colors;
  int length_color;
  int nb_iterations;
  int NTHREADS;
  int dim_x;
  int dim_y;
  float space;
  std::complex<T> center;

public:
  Mendelbrot();

  void display_parameters();
  void compute();
  void display_image();
};

template <typename T> class Julia {
private:
  cv::Mat image;
  std::vector<cv::Vec3f> colors;
  int length_color;
  int nb_iterations;
  int NTHREADS;
  int dim_x;
  int dim_y;
  float space;
  std::complex<T> center;
  std::complex<T> c_julia;

public:
  Julia(std::complex<T> c);
  void compute();
  void display_image();
  void display_parameters();
};

// template <typename T> class Newton : public Fractale<T> {
// private:
//   Polynome<T> P;
//   std::vector<std::complex<T>> racines;

// public:
//   Newton(Polynome<T> poly, std::vector<std::complex<T>> racines);
//   compute();
// };

// template <typename T> class Lyapunov : public Fractale<T> {
// public:
//   // compute(); // version Lyapunov
// };

//// Implémentations :

template <typename T> Mendelbrot<T>::Mendelbrot() {
  colors = {cv::Vec3f(1, 1, 1), cv::Vec3f(0, 0, 0)};
  length_color = 30;
  nb_iterations = 3500;
  NTHREADS = 16;
  dim_x = 1500;
  dim_y = 1000;
  space = 2;
  image = (cv::Mat::ones(cv::Size(dim_x, dim_y), CV_32FC3));
  center.real(-0.7);
  center.imag(0);
  image.convertTo(image, CV_32FC3, 1 / 255.0);
}

template <typename T> void Mendelbrot<T>::display_parameters() {
  // Pour que "cout" ne fasse pas un arrondi lorsqu'il affiche un double.
  typedef std::numeric_limits<double> dbl;
  std::cout.precision(dbl::max_digits10);
  std::cout << "Taille de l'image : " << dim_x << " x " << dim_y << std::endl
            << "Nombre d'itérations : " << nb_iterations << std::endl
            << "Nombre de threads : " << NTHREADS << std::endl
            << "Taille de la zoombox : " << 2 * space << " x "
            << 2 * space * dim_y / dim_x << std::endl
            << "Persistance d'une couleur : " << length_color << std::endl
            << "Centre du zoom : " << center.real() << " + " << center.imag()
            << "i" << std::endl;
}

template <typename T> void Mendelbrot<T>::compute() {
  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < image.rows; i++) {
    int n_iter = 0;
    for (int j = 0; j < image.cols; j++) {
      std::complex<T> z = get_C_number_from_pixel(i, j, &image, center, space);
      n_iter = mendelbrot_iterations_of_C_number(z, nb_iterations);

      image.at<cv::Vec3f>(i, j) = get_color_from_nb_iteration(
          n_iter, nb_iterations, colors, length_color);
    }
  }

  auto stop = omp_get_wtime();
  auto elapsed = std::chrono::duration<double>(stop - start).count();
  std::cout << "----------------------------------------" << std::endl
            << "Calculs faits en " << elapsed << " secondes." << std::endl
            << "----------------------------------------" << std::endl;
}

template <typename T> void Mendelbrot<T>::display_image() {
  imshow("image", image);
  cv::waitKey(0);
  cv::destroyAllWindows();
}

template <typename T> Julia<T>::Julia(std::complex<T> c) {
  colors = {cv::Vec3f(1, 1, 1), cv::Vec3f(0, 0, 0), cv::Vec3f(0, 0, 0),
            cv::Vec3f(0, 0, 0), cv::Vec3f(0, 0, 0), cv::Vec3f(0, 0, 0)};
  length_color = 50;
  nb_iterations = 3500;
  NTHREADS = 16;
  dim_x = 1500;
  dim_y = 1000;
  space = 2;
  image = (cv::Mat::ones(cv::Size(dim_x, dim_y), CV_32FC3));
  center.real(0);
  center.imag(0);
  image.convertTo(image, CV_32FC3, 1 / 255.0);
  c_julia = c;
}

template <typename T> void Julia<T>::compute() {
  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < image.rows; i++) {
    int n_iter = 0;
    for (int j = 0; j < image.cols; j++) {
      std::complex<T> z = get_C_number_from_pixel(i, j, &image, center, space);
      n_iter = julia_iterations_of_C_number(z, c_julia, nb_iterations);
      image.at<cv::Vec3f>(i, j) = get_color_from_nb_iteration(
          n_iter, nb_iterations, colors, length_color);
    }
  }
  auto stop = omp_get_wtime();
  auto elapsed = std::chrono::duration<double>(stop - start).count();
  std::cout << "----------------------------------------" << std::endl
            << "Calculs faits en " << elapsed << " secondes." << std::endl
            << "----------------------------------------" << std::endl;
}

template <typename T> void Julia<T>::display_image() {
  imshow("image", image);
  cv::waitKey(0);
  cv::destroyAllWindows();
}

template <typename T> void Julia<T>::display_parameters() {
  // Pour que "cout" ne fasse pas un arrondi lorsqu'il affiche un double.
  typedef std::numeric_limits<double> dbl;
  std::cout.precision(dbl::max_digits10);
  std::cout << "Taille de l'image : " << dim_x << " x " << dim_y << std::endl
            << "Nombre d'itérations : " << nb_iterations << std::endl
            << "Nombre de threads : " << NTHREADS << std::endl
            << "Taille de la zoombox : " << 2 * space << " x "
            << 2 * space * dim_y / dim_x << std::endl
            << "Persistance d'une couleur : " << length_color << std::endl
            << "Centre du zoom : " << center.real() << " + " << center.imag()
            << "i" << std::endl;
}

// void write_image(string str) {
//   if (vm.count("write_img")) {
//     string name = "images/" + vm["write_img"].as<string>() + ".tiff";
//     imwrite(name, img);
//     A = false;
//   }

//   // set_parameters_from_mouse(&center, &img, space);
//   // space = space / 2;
//   // display_param(dim_x, dim_y, nb_iterations, NTHREADS,
//   space,length_color,
//   //               center);
// }

#endif