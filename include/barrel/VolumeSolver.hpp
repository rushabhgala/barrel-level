# pragma once

#include "barrel/Barrel.hpp"

namespace barrel {

// compute the liquid height (h) after 
// removing a given volume from a full horizontal cylinder
class VolumeSolver {
public:
    explicit VolumeSolver(const Barrel& barrel);

    // volume removed must be in the range [0, full_volume]
    // returning height must be in the range [0, 2*R]
    double height_for_removed_volume(double volume_removed) const;

private:
    Barrel barrel_;
};

} //namespace barrel