
// #include <boost/program_options/option.hpp>
// #include <boost/program_options/options_description.hpp>
// #include <boost/program_options/parsers.hpp>
// #include <boost/program_options/variables_map.hpp>
#include <opencv2/opencv.hpp>

#include "Julia.hpp"
#include "Mendelbrot.hpp"
#include "Newton.hpp"
#include "Polynome.hpp"
#include "interface.hpp"
// #include "options.hpp"

using namespace std;
using namespace cv;
// namespace po = boost::program_options;

int main(int argc, char **argv) {
  cout << "----------------------------------------" << endl
       << "Fractales 2.0" << endl
       << "----------------------------------------" << endl;

  srand(time(NULL));

  // Mendelbrot<double> mendel_1;
  // mendel_1.display_parameters();
  // mendel_1.compute();
  // mendel_1.display_image();

  // Mendelbrot<float> mendel_2;
  // mendel_2.display_parameters();
  // mendel_2.compute();
  // mendel_2.display_image();

  // Mendelbrot<double> mendel_3(8, 500, 500);
  // mendel_3.display_parameters();
  // mendel_3.compute();
  // mendel_3.display_image();

  // complex<double> c;
  // c.real(-0.70176);
  // c.imag(-0.3842);
  // Julia<double> julia_1(c);
  // julia_1.display_parameters();
  // julia_1.compute();
  // julia_1.display_image();

  // std::vector<double> coeff = {-2, 0, 8, 7, 0, -1, 1, -1, 1, -1, 1, -1, -1,
  // 3, -14};
  std::vector<double> coeff = {1, 0, 0, -1};
  // vector<double> coeff = {1, 0, -2, +2};
  vector<double> coeff = {1, -2, -3, 4, 4};

  Polynome P(coeff);
  Polynome P_d = P.deriv();
  P.print();
  // P_d.print();

  vector<complex<double>> racines = P.racines();

  cout << racines.size() << endl;
  for (auto const &e : racines) {
    cout << e << endl;
  }

  Newton frac_N(P, racines, 16, 1000, 1000);
  frac_N.compute();
  frac_N.display_image();
  // frac_N.write_image("x³-1 classic ");
  return 0;
}
