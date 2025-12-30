#pragma once

namespace barrel {
    class Barrel {
    public:
        Barrel(double radius, double length);

        double radius() const noexcept;
        double length() const noexcept;

        // Full cylinder volume
        double full_volume() const noexcept;

    private:
        double radius_;
        double length_;
    };
} //namespace barrel
