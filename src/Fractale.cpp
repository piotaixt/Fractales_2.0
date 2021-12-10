

#include <iostream>

#include "Fractale.hpp"

using namespace std;
using namespace cv;

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