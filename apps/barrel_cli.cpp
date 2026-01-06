#include "barrel/Barrel.hpp"
#include "barrel/VolumeSolver.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

static double get_arg(int argc, char** argv, const std::string& name) {
    for (int i=1; i+1 < argc; ++i) {
        if (argv[i] == name) {
            return std::stod(argv[i+1]);
        }
    }
    throw std::invalid_argument("Missing argument: " + name);
}

int main(int argc, char** argv) {
    try{
        const double radius = get_arg(argc, argv, "--radius");
        const double length = get_arg(argc, argv, "--length");
        const double vol_removed = get_arg(argc, argv, "--removed_vol");

        barrel::Barrel b(radius, length);
        barrel::VolumeSolver solver(b);

        const double h = solver.height_for_removed_volume(vol_removed);
        std::cout << "height of filled liquid = " << h << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "Usage: barrel_cli --radius R --length L --removed_vol V_removed \n";
        return 1;
    }
}