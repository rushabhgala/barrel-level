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

  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build

Command Line Usage
------------------

Example:

  ./build/barrel_cli --radius 0.5 --length 1.0 --volume 0.3

All units must be consistent (e.g. meters -> cubic meters).

Project Structure
-----------------

  include/   Public headers
  src/       Solver implementation
  apps/      Command-line interface
  tests/     Verification tests

Notes
-----

The solver enforces both absolute and relative accuracy requirements and
is designed to be robust near degenerate fill levels.

