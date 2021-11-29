

#include <iostream>

#include "Fractale.hpp"
#include "calculs.hpp"

using namespace std;
using namespace cv;

Fractale::Fractale() {
  colors = {Vec3f(1, 1, 1), Vec3f(0, 0, 0)};
  // colors = {Vec3f(0, 0, 0), Vec3f(0, 0, 1),   Vec3f(0, 0.5, 1), Vec3f(0, 1,
  // 1),
  //           Vec3f(1, 1, 1), Vec3f(1, 0.3, 0), Vec3f(1, 0, 0)};
  length_color = 300;
  nb_iterations = 500;
  NTHREADS = 16;
  dim_x = 3000;
  dim_y = 1500;
  center.real(0);
  center.imag(0.0);
  image = (Mat::ones(Size(dim_x, dim_y), CV_32FC3));
  image.convertTo(image, CV_32FC3, 1 / 255.0);

  // center(-0.743643887037151, 0.13182590420533);
  space = 2;
}

void Fractale::display_parameters() {
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

void Fractale::display_image() {
  imshow("image", image);
  waitKey(0);
  destroyAllWindows();
}

Mendelbrot::Mendelbrot() : Fractale() {}

void Mendelbrot::compute() {
  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < image.rows; i++) {
    int n_iter = 0;
    for (int j = 0; j < image.cols; j++) {
      complex<double> z = get_C_number_from_pixel(i, j, &image, center, space);
      n_iter = mendelbrot_iterations_of_C_number(z, nb_iterations);
      image.at<Vec3f>(i, j) = get_color_from_nb_iteration(n_iter, nb_iterations,
                                                          colors, length_color);
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

Julia::Julia() : Fractale() {
  c.real(-0.8);
  c.imag(0.156);
}

void Julia::compute() {
  omp_set_num_threads(NTHREADS);
  auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < image.rows; i++) {
    int n_iter = 0;
    for (int j = 0; j < image.cols; j++) {
      complex<double> z = get_C_number_from_pixel(i, j, &image, center, space);
      n_iter = julia_iterations_of_C_number(z, c, nb_iterations);
      image.at<Vec3f>(i, j) = get_color_from_nb_iteration(n_iter, nb_iterations,
                                                          colors, length_color);
    }
  }
  auto stop = omp_get_wtime();
  auto elapsed = chrono::duration<double>(stop - start).count();
  cout << "----------------------------------------" << endl
       << "Calculs faits en " << elapsed << " secondes." << endl
       << "----------------------------------------" << endl;
}

// Newton::Newton(std::vector<double> poly,
//                std::vector<std::complex<double>> rac) {
//   polynome = poly;
//   racines = rac;
// }

// void Newton::compute() {
//   omp_set_num_threads(NTHREADS);
//   auto start = omp_get_wtime();

// #pragma omp parallel for schedule(dynamic, 1)
//   for (int i = 0; i < image.rows; i++) {
//     int n_iter = 0;
//     for (int j = 0; j < image.cols; j++) {
//       complex<double> z = get_C_number_from_pixel(i, j, &image, center,
//       space);

//       for (int k = 1; k < nb_iterations; k++) {
//       }

//       // A FINIR

//       n_iter = julia_iterations_of_C_number(z, c, nb_iterations);

//       image.at<Vec3f>(i, j) =
//           newton_get_color(n_iter, nb_iterations, colors, length_color);
//     }
//   }
//   auto stop = omp_get_wtime();
//   auto elapsed = chrono::duration<double>(stop - start).count();
//   cout << "----------------------------------------" << endl
//        << "Calculs faits en " << elapsed << " secondes." << endl
//        << "----------------------------------------" << endl;
// }