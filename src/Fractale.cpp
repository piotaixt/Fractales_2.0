

#include <iostream>

#include "Fractale.hpp"
#include "calculs.hpp"

using namespace std;
using namespace cv;

template <typename T> Fractale<T>::Fractale() {
  colors = {Vec3f(1, 1, 1), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0),
            Vec3f(0, 0, 0), Vec3f(0, 0, 0), Vec3f(0, 0, 0)};
  // colors = {Vec3f(0, 0, 0), Vec3f(0, 0, 1),   Vec3f(0, 0.5, 1), Vec3f(0, 1,
  // 1),
  //           Vec3f(1, 1, 1), Vec3f(1, 0.3, 0), Vec3f(1, 0, 0)};
  length_color = 75;
  nb_iterations = 3500;
  NTHREADS = 16;
  dim_x = 3000;
  dim_y = 2000;
  center.real(0.0);
  center.imag(0.0);
  image = (Mat::ones(Size(dim_x, dim_y), CV_32FC3));
  // image.convertTo(image, CV_32FC3, 1 / 255.0);

  // center(-0.743643887037151, 0.13182590420533);
  space = 2;
}

template <typename T> void Fractale<T>::display_parameters() {
  // Pour que "cout" ne fasse pas un arrondi lorsqu'il affiche un double.
  typedef numeric_limits<double> dbl;
  cout.precision(dbl::max_digits10);
  cout << "Taille de l'image : " << dim_x << " x " << dim_y << endl
       << "Nombre d'itérations : " << nb_iterations << endl
       << "Nombre de threads : " << NTHREADS << endl
       << "Taille de la zoombox : " << 2 * space << " x "
       << 2 * space * dim_y / dim_x << endl
       << "Persistance d'une couleur : " << length_color << endl
       << "Centre du zoom : " << center.real() << " + " << center.imag() << "i"
       << endl;
}

template <typename T> void Fractale<T>::display_image() {
  imshow("image", image);
  waitKey(0);
  destroyAllWindows();
}

template <typename T> Mendelbrot<T>::Mendelbrot() : Fractale<T>() {}

template <typename T> void Mendelbrot<T>::compute() {
  omp_set_num_threads(this->NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < this->image.rows; i++) {
    int n_iter = 0;
    for (int j = 0; j < this->image.cols; j++) {
      complex<T> z =
          get_C_number_from_pixel(i, j, this->image, this->center, this->space);
      n_iter = mendelbrot_iterations_of_C_number(z, this->nb_iterations);

      this->image.at(i, j) = get_color_from_nb_iteration(
          n_iter, this->nb_iterations, this->colors, this->length_color);
    }
  }

  auto stop = omp_get_wtime();
  auto elapsed = chrono::duration<double>(stop - start).count();
  cout << "----------------------------------------" << endl
       << "Calculs faits en " << elapsed << " secondes." << endl
       << "----------------------------------------" << endl;

  // if (vm.count("write_img")) {
  //   string name = "images/" + vm["write_img"].as<string>() + ".tiff";
  //   imwrite(name, img);
  //   A = false;
  // }

  // set_parameters_from_mouse(&center, &img, space);
  // space = space / 2;
  // display_param(dim_x, dim_y, nb_iterations, NTHREADS, space, length_color,
  //               center);
}

// template <typename T> Julia<T>::Julia() {
//   // Fractale();
//   c.real(-0.8);
//   c.imag(0.18);
// }

// template <typename T> void Julia<T>::compute() {
//   omp_set_num_threads(this->NTHREADS);
//   auto start = omp_get_wtime();

// #pragma omp parallel for schedule(dynamic, 1)
//   for (int i = 0; i < this->image.rows; i++) {
//     int n_iter = 0;
//     for (int j = 0; j < this->image.cols; j++) {
//       complex<double> z =
//           get_C_number_from_pixel(i, j, this->image, this->center,
//           this->space);
//       n_iter = julia_iterations_of_C_number(z, c, this->nb_iterations);
//       this->image.at(i, j) = get_color_from_nb_iteration(
//           n_iter, this->nb_iterations, this->colors, this->length_color);
//     }
//   }
//   auto stop = omp_get_wtime();
//   auto elapsed = chrono::duration<double>(stop - start).count();
//   cout << "----------------------------------------" << endl
//        << "Calculs faits en " << elapsed << " secondes." << endl
//        << "----------------------------------------" << endl;
// }

// template <typename T>
// Newton::Newton(Polynome<T> poly, std::vector<std::complex<double>> rac) {
//   polynome = poly;
//   racines = rac;
// }

// void Newton::compute() {
//   Polynome p_derive = polynome.deriv();
//   omp_set_num_threads(NTHREADS);
//   auto start = omp_get_wtime();

// #pragma omp parallel for schedule(dynamic, 1)
//   for (int i = 0; i < image.rows; i++) {
//     int n_iter = 0;
//     for (int j = 0; j < image.cols; j++) {
//       complex<double> z = get_C_number_from_pixel(i, j, &image, center,
//       space);

//       // for (int k = 1; k < nb_iterations; k++) {
//       //   z = z - (polynome.eval()));
//     }

//     // A FINIR

//     n_iter = julia_iterations_of_C_number(z, c, nb_iterations);

//     image.at<Vec3f>(i, j) =
//         newton_get_color(n_iter, nb_iterations, colors, length_color);
//   }
// }
// auto stop = omp_get_wtime();
// auto elapsed = chrono::duration<double>(stop - start).count();
// cout << "----------------------------------------" << endl
//      << "Calculs faits en " << elapsed << " secondes." << endl
//      << "----------------------------------------" << endl;
// }