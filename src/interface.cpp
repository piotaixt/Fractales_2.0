#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "interface.hpp"
#include "calculs.hpp"

using namespace std;
using namespace cv;
namespace po = boost::program_options;

void CallBackFunc(int event, int x, int y, int, void *input)
{
  if (event == EVENT_LBUTTONDOWN)
  {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ") - Press any key to valid" << endl;
    Point P(x, y);
    Point *p = (Point *)input;
    *p = P;
  }
  else if (event == EVENT_RBUTTONDOWN)
  {
    // cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  }
  else if (event == EVENT_MBUTTONDOWN)
  {
    // cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
  }
  else if (event == EVENT_MOUSEMOVE)
  {
    // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
  }
}

void set_parameters_from_mouse(complex<double> *center, const Mat *img, float space)
{
  Point pixel;
  //Create a window
  namedWindow("My Window", 1);
  //set the callback function for any mouse event
  setMouseCallback("My Window", CallBackFunc, &pixel);
  //show the image
  imshow("My Window", *img);
  // Wait until user press some key
  waitKey(0);

  complex<double> new_center = get_C_number_from_pixel(pixel.y, pixel.x, img, *center, space);
  *center = new_center;

}

void display(Mat image)
{
  imshow("image", image);
  waitKey(0);
  destroyAllWindows();
}

void display_param(int dim_x, int dim_y, int nb_iterations, int NTHREADS, float space, uint length_color, complex<double> center)
{
  cout << "Taille de l'image : " << dim_x << " x " << dim_y << endl
       << "Nombre d'itérations : " << nb_iterations << endl
       << "Nombre de threads : " << NTHREADS << endl
       << "Taille de la zoombox : " << 2 * space << " x " << 2 * space * dim_y / dim_x << endl
       << "Persistance d'une couleur : " << length_color << endl
       << "Centre du zoom : " << center.real() << " + " << center.imag() << "i" << endl;
}