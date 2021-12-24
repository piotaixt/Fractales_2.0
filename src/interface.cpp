// #include <boost/program_options/option.hpp>
// #include <boost/program_options/options_description.hpp>
// #include <boost/program_options/parsers.hpp>
// #include <boost/program_options/variables_map.hpp>

#include <opencv2/opencv.hpp>
#include "interface.hpp"
#include "calculs.hpp"

using namespace std;
using namespace cv;
// namespace po = boost::program_options;

void CallBackFunc(int event, int x, int y, int, void *input) {
  if (event == EVENT_LBUTTONDOWN) {
    cout << "Left button of the mouse is clicked - position (" << x << ", " << y
         << ") - Press any key to valid" << endl;
    Point P(x, y);
    Point *p = (Point *)input;
    *p = P;
  } else if (event == EVENT_RBUTTONDOWN) {
    // cout << "Right button of the mouse is clicked - position (" << x << ", "
    // << y << ")" << endl;
  } else if (event == EVENT_MBUTTONDOWN) {
    // cout << "Middle button of the mouse is clicked - position (" << x << ", "
    // << y << ")" << endl;
  } else if (event == EVENT_MOUSEMOVE) {
    // cout << "Mouse move over the window - position (" << x << ", " << y <<
    // ")" << endl;
  }
}

void set_parameters_from_mouse(complex<double> *center, const Mat *img,
                               float space) {
  Point pixel;
  // Create a window
  namedWindow("My Window", 1);
  // set the callback function for any mouse event
  setMouseCallback("My Window", CallBackFunc, &pixel);
  // show the image
  imshow("My Window", *img);
  // Wait until user press some key
  waitKey(0);

  complex<double> new_center =
      get_C_number_from_pixel(pixel.y, pixel.x, img, *center, space);
  *center = new_center;
}

void display(cv::Mat image)
{
  imshow("image", image);
  cv::waitKey(0);
  cv::destroyAllWindows();
}