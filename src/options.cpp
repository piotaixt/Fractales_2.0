#include "options.hpp"

// // Declare the supported options.
// po::options_description desc("Options possibles : ");
// desc.add_options()("help", "Afficher les options")(
//     "size_x", po::value<int>(), "int : Largeur de l'image en pixels")(
//     "size_y", po::value<int>(), "int : Hauteur de l'image en pixels")(
//     "nbr_iter", po::value<int>(), "int : Nombre maximal d'itérations")(
//     "nbr_threads", po::value<int>(), "int : Nombre de threads
//     utilisés")( "size_zoombox", po::value<double>(), "double : Demi
//     largeur de la zoombox (conseillé de prendre dans " "l'intervalle
//     [0,2])")("center_x", po::value<double>(),
//                            "double : Centre du zoom en x")(
//     "center_y", po::value<double>(), "double : Centre du zoom en y")(
//     "pallette", "Afficher la pallette de couleurs")(
//     "length_color", po::value<uint>(),
//     "int : Nombre d'itérations par changement de couleurs")(
//     "write_img", po::value<string>(),
//     "string : Mode ecriture d'une image, donner le nom souhaité");

// po::variables_map vm;
// po::store(po::parse_command_line(argc, argv, desc), vm);
// po::notify(vm);

// if (vm.count("help")) {
//   cout << desc << "\n";
//   return 1;
// }

// if (vm.count("size_x")) {
//   dim_x = vm["size_x"].as<int>();
// }

// if (vm.count("size_y")) {
//   dim_y = vm["size_y"].as<int>();
// }

// if (vm.count("nbr_iter")) {
//   nb_iterations = vm["nbr_iter"].as<int>();
// }

// if (vm.count("nbr_threads")) {
//   NTHREADS = vm["nbr_threads"].as<int>();
// }

// if (vm.count("size_zoombox")) {
//   space = vm["size_zoombox"].as<double>();
// }

// if (vm.count("center_x")) {
//   complex<double> c1(vm["center_x"].as<double>(), center.imag());
//   center = c1;
// }

// if (vm.count("center_y")) {
//   complex<double> c2(center.real(), vm["center_y"].as<double>());
//   center = c2;
// }

// if (vm.count("length_color")) {
//   length_color = vm["length_color"].as<uint>();
// }

// if (vm.count("pallette")) {
//   make_pallette(colors, length_color, nb_iterations);
//   return 1;
// }
