#ifndef NEWTON_H
#define NEWTON_H
#include <omp.h>
#include "Fractale.hpp"
#include "Polynome.hpp"

template <typename T>
class Newton : public Fractale
{
private:
    Polynome<T> P;
    std::vector<cv::Vec3f> colors;
    int nb_iterations;
    float space;
    std::complex<T> center;
    std::vector<std::complex<T>> racines;

public:
    Newton(Polynome<T> poly, std::vector<std::complex<T>> racines,
           int NTHREADS = 16, int dim_x = 1500, int dim_y = 1000);
    //  std::vector<std::tuple<std::complex<T>, cv::Vec3f color>> racines,
    void compute();
};

template <typename T>
Newton<T>::Newton(Polynome<T> poly, std::vector<std::complex<T>> rac,
                  int NTHREADS, int dim_x, int dim_y)
    : Fractale::Fractale(NTHREADS, dim_x, dim_y)
{
    P = poly;
    racines = rac;
    space = 2;
    center.real(0);
    center.imag(0);
}

template <typename T>
void Newton<T>::compute()
{
    T alpha = 1.2;
    float prec = 0.001;
    int nb_iterations_max = 300;
    Polynome p_deriv = P.deriv();
    float r = 0;
    float g = 0;
    float b = 0;

    for (int i = 0; i <= P.getDeg(); i++)
    {
        r = fRand(0, 1);
        g = fRand(0, 1);
        b = fRand(0, 1);
        this->colors.push_back(cv::Vec3f(r, g, b));
    }

    omp_set_num_threads(NTHREADS);
    auto start = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < image.rows; i++)
    {
        int n_iter = 0;
        bool A = true;
        int color_pix = 0;
        for (int j = 0; j < image.cols; j++)
        {
            color_pix = 0;
            A = true;

            std::complex<T> z = get_C_number_from_pixel(i, j, &image, center, space);
            int k = 0;
            while (A and k < nb_iterations_max)
            {
                z = z - alpha * (P.eval(z) / p_deriv.eval(z));
                for (int l = 0; l < racines.size(); ++l)
                {
                    if (norme_2(z - racines[l]) < prec)
                    {
                        n_iter = k;
                        color_pix = l;
                        A = false;
                        break;
                    }
                }
                k++;
            }
            image.at<cv::Vec3f>(i, j) = colors[color_pix] * 1.5 / (log(2 + n_iter));
            if (k == nb_iterations_max)
            {
                image.at<cv::Vec3f>(i, j) = colors[P.getDeg()];
            }
        }
    }

    auto stop = omp_get_wtime();
    auto elapsed = std::chrono::duration<double>(stop - start).count();
    std::cout << "----------------------------------------" << std::endl
              << "Calculs faits en " << elapsed << " secondes." << std::endl
              << "----------------------------------------" << std::endl;
}

#endif