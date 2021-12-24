#ifndef FRACTALE_H
#define FRACTALE_H

#include <opencv2/opencv.hpp>

class Fractale
{
protected:
  cv::Mat image;
  int NTHREADS;
  int dim_x;
  int dim_y;

public:
  Fractale(int NTHREADS = 16, int dim_x = 1500, int dim_y = 1000);
  void display_image();
  void write_image(std::string str);
};

//// Implémentations :

Fractale::Fractale(int nthreads, int d_x, int d_y)
    : NTHREADS(nthreads), dim_x(d_x), dim_y(d_y)
{
  image = (cv::Mat::ones(cv::Size(dim_x, dim_y), CV_32FC3));
  image.convertTo(image, CV_32FC3, 1 / 255.0);
}

void Fractale::display_image()
{
  imshow("image", image);
  cv::waitKey(0);
  cv::destroyAllWindows();
}

void Fractale::write_image(std::string str)
{
  std::string name = "images/" + str + ".tiff";
  cv::imwrite(name, image);
}

//   // set_parameters_from_mouse(&center, &img, space);
//   // space = space / 2;
//   // display_param(dim_x, dim_y, nb_iterations, NTHREADS,
//   space,length_color,
//   //               center);
// }

#endif