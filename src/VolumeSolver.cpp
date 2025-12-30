#include "barrel/VolumeSolver.hpp"

#include <boost/math/tools/roots.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace barrel {

namespace {

// Volume remaining at height h in a horizontal cylinder
double volume_for_height(double R, double L, double H){
    const double full = M_PI * R * R * L;

    if (h <= 0.0) return 0.0;
    if (h >= 2.0 * R) return full;

    // We will compute volume for [0, R] and use symmetry for > R
    if (h > R) {
        return full - volume_for_height(R, L, 2.0*R - h);
    }

    const double d = R - h;

    // Clamp to protect arc-cos from floating drift
    const double x = std::clamp(d/R, -1.0, 1.0);

    // under = 2Rh - h^2 = h(2R-h) >= 0 in [0,2R]
    const double under = std::max(0.0, 2.0 * R * h - h * h);

    const double area = R * R * std::acos(x) - d * std::sqrt(under);
    return area * L;
}
}// anonymous namespace

VolumeSolver::VolumeSolver(const Barrel& barrel): barrel_(barrel)
{
}

double VolumeSolver::height_for_removed_volume(double volume_removed) const {
    const double R = barrel_.radius();
    const double L = barrel_.length();
    const double full = barrel_.full_volume();

    if (volume_removed < 0.0 || volume_removed > full) {
        throw std::invalid_argument("Volume removed must be within [0, full_volume]");
    }

    const double V_remaining = full - volume_removed;

    //f(h) = V(h) - V_remaining
    auto f = [&](double h){
        return volume_for_height(R, L , h) - V_remaining;
    };

    //Root must lie in [0, 2*R]
    const double lower = 0.0;
    const double upper = 2.0 * R;

    //Accuracy requirement: 0.001% or 10um (whichever is more accurate)
    //Define tolerances and use the more accurate one
    const double rel_tol = 1e-5; // 0.001%
    const double abs_tol = 1e-5; // 10 um in metres

    const double rel_from_abs = abs_tol / std::max(1.0, upper);
    const double effective_rel = std::max(rel_tol, rel_from_abs);

    boost::math::tools::eps_tolerance<double> tol(effective_rel);

    auto bracket = boost::math::tools::brent_find_root(f, lower, upper, tol);

    return 0.5 * (bracket.first + bracket.second);
} 

} //namespace barrel