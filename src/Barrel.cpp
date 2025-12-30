#include "barrel/Barrel.hpp"

#include <cmath>
#include <stdexcept>

namespace barrel {
    
Barrel::Barrel(double radius, double length): radius_(radius), length_(length)
{
    if (!(radius_) > 0.0 || !(length_ > 0.0)){
        throw std::invalid_argument("radius and length must be positive");
    }
}

double Barrel::radius() const noexcept {
    return radius_;
}

double Barrel::length() const noexcept {
    return length_;
}

double Barrel::full_volume const noexcept {
    return M_PI * radius_ * radius_ * length_;
}

} //namespace barrel
