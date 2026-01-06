Barrel Liquid Height Solver
===========================

This project computes the liquid height in a horizontal cylindrical barrel
required to achieve a desired liquid volume.

The problem arises when draining or filling a barrel lying on its side,
where the relationship between height and volume is non-linear.

Problem Description
-------------------

Given:
  - Barrel radius R
  - Barrel length L
  - Desired liquid volume V

Compute:
  - Liquid height h measured from the bottom of the barrel

The solution is based on the analytical area of a circular segment and
a robust numerical root-finding method.

Build Instructions
------------------

Requirements:
  - C++17 compatible compiler
  - CMake >= 3.20

Build commands:

```
  cmake -S . -B build
  cmake --build build
```

Command Line Usage
------------------
```
/path/to/build/dir/barrel_cli --radius x --length y --removed_vol z
```

Example:
```
  ./build/barrel_cli --radius 0.5 --length 1.0 --removed_vol 0.5
```
All units must be SI units (e.g. meters -> cubic meters).

Running Test
------------
A simple test is implemented for sanity check, run it using:

```
ctest --test-dir build --output-on-failure
```

Project Structure
-----------------

  include/   Header files
  src/       Geometry and Solver implementation
  apps/      Command-line interface
  tests/     Simple test

Notes
-----

The solver enforces both absolute and relative accuracy requirements.

