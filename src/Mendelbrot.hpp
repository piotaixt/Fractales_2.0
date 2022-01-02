#ifndef MENDELBROT_H
#define MENDELBROT_H
#include <omp.h>
#include "Fractale.hpp"

template <typename T>
class Mendelbrot : public Fractale
{
private:
  std::vector<cv::Vec3f> colors;
  int nb_iterations;
  int length_color;
  float space;
  std::complex<T> center;

public:
  Mendelbrot(int NTHREADS = 16, int dim_x = 1500, int dim_y = 1000);
  void display_parameters();
  void compute();
};

template <typename T>
Mendelbrot<T>::Mendelbrot(int NTHREADS, int dim_x, int dim_y)
    : Fractale::Fractale(NTHREADS, dim_x, dim_y)
{
  colors = {cv::Vec3f(1, 1, 1), cv::Vec3f(0, 0, 0)};
  length_color = 30;
  nb_iterations = 3500;
  space = 2;
  center.real(-0.7);
  center.imag(0);
}

template <typename T>
void Mendelbrot<T>::display_parameters()
{
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

template <typename T>
void Mendelbrot<T>::compute()
{
  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < image.rows; i++)
  {
    int n_iter = 0;
    for (int j = 0; j < image.cols; j++)
    {
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

#endif