#include "barrel/Barrel.hpp"
#include "barrel/VolumeSolver.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

static void require(bool ok, const char* msg) {
    if(!ok) {
        std::cerr << "TEST FAILED" << msg << "\n";
        std::exit(1)
    }
}

static bool approx(double a, double b, double rel, double abs) {
    const double diff = std::abs(a - b);
    const double scale = std::max(1.0, std::max(std::abs(a), std::abs(b)));
    return diff <= std::max(abs, rel * scale);
}

int main() {
    const double R = 0.5;
    const double L = 1.0;

    barrel::Barrel b(R, L);
    barrel::VolumeSolver s(b);

    const double full = b.full_volume();

    // Remove nothing, volume is full, h = 2 * R
    require(approx(s.height_for_removed_volume(0.0), 2.0 * R, 1e-12, 1e-12), "removed = 0 => barrel full");

    // Remove full volume, volume is null, h = 0
    require(approx(s.height_for_removed_volume(full), 0.0, 1e-12, 1e-12), "removed = full => barrel empty");

    // Remove half volume, volume is half, h = R
    require(approx(s.height_for_removed_volume(0.5 * full), R, 1e-5, 1e-5), "half removed => barrel half");

    std::cout << "All tests passed. \n";
    return 0;
}