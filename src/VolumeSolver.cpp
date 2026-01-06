#include "barrel/VolumeSolver.hpp"

#include <boost/math/tools/roots.hpp> //Used for root finding

#include <algorithm>    //Used for std::clamp, std::max
#include <cmath>        //Used for std::acos, M_PI
#include <cstdint>      //Used for std::uintmax_t
#include <stdexcept>    //Used for std::invalid_argument

namespace barrel {

namespace {

// Volume filled at height h in a horizontal cylinder
double volume_filled(double R, double L, double h){
    const double full = M_PI * R * R * L;

    // Boundary conditions for completely filled and completely empty
    if (h <= 0.0) return 0.0;
    if (h >= 2.0 * R) return full;

    // We will compute volume for [0, R] and use symmetry for > R
    if (h > R) {
        return full - volume_filled(R, L, 2.0 * R - h);
    }

    // Distance from centre to liquid height
    const double d = R - h;

    // Clamp to protect arc-cos from floating drift
    const double x = std::clamp(d/R, -1.0, 1.0);

    // under = 2Rh - h^2 = h(2R-h) >= 0 in [0,2R]
    const double under = std::max(0.0, 2.0 * R * h - h * h);

    // circular segment area below the chord
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

    //Exact end points for boundary cases
    if (volume_removed == 0.0) {
        return 2.0 * R; // barrel is full
    }
    if (volume_removed == full) {
        return 0.0; // empty barrel
    }

    const double V_remaining = full - volume_removed;

    //Root function: f(h) = V_filled(h) - V_remaining
    auto f = [&](double h){
        return volume_filled(R, L , h) - V_remaining;
    };
    
    //Root must lie in [0, 2*R]
    const double lower = 0.0;
    const double upper = 2.0 * R;

    //Ensuring the limits are valid for f(lower) <=0 or f(upper) >=0
    const double f_lo = f(lower);
    const double f_hi = f(upper);
    if (!(f_lo <= 0.0 && f_hi >= 0.0)) {
        throw std::runtime_error("Internal error: root not bracketed in [0, 2*R]");
    }

    //Accuracy requirement: 0.001% or 10um (whichever is more accurate)
    //Define tolerances and use the more accurate one
    const double rel_tol = 1e-5; // 0.001%
    const double abs_tol = 1e-5; // ex: 10 um in metres

    // Boost bisect needs a termination predicate + max iterations.
    // We'll stop when interval width is within max(abs_tol, rel_tol*scale)
    std::uintmax_t max_iter = 200;

    //Stopping condition for the bisection method
    auto stop = [&](double a, double b) {
        const double width = std::abs(b-a);
        const double scale = std::max(1.0, std::max(std::abs(a), std::abs(b)));
        const double tol = std::max(abs_tol, rel_tol * scale);
        return width <= tol;
    };

    //Find the root brackets and calculate the mean
    auto bracket = boost::math::tools::bisect(f, lower, upper, stop, max_iter);
    const double h = 0.5 * (bracket.first + bracket.second);

    //Return exact heights for boundary cases
    if (h < abs_tol) return 0.0;
    if (h > 2.0 * R - abs_tol) return 2.0 * R;

    return h;
} 

} //namespace barrel