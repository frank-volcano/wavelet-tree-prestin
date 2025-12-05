Hello, user and welcome to the world of Prestin/Coarse Grain/MDAnalysis !!! :)

This project is the Wavelet Tree Prestin Analyzer. It demonstrates how a Wavelet Tree, a succinct data structure used for compressed sequence storage, 
can be applied to coarse-grained molecular dynamics (MD) data such as z-bin bead coordinates from simulations of the Prestin membrane protein.

The purpose of the project is to allow fast queries on large sequences of integer bins without decompressing the data.

This is useful for MD analysis due to the simulation of frames being in the thousands of beads per frame and hundreds of frames per trajectory.

The program reads frame files containing lists of integer bin values. These bins represent regions of the membrane such as: 
cytoplasmic (negative), midplane (zero), or extracellular (positive). The Wavelet Tree supports fast rank queries, symbol counts in intervals, and region occupancy queries. 
The project also supports time-series analysis across multiple frames.

The project consists of three source files:
WaveletTree.h
WaveletTree.cpp
main.cpp

WaveletTree.h and WaveletTree.cpp implement the Wavelet Tree. main.cpp provides an interactive menu user interface for loading frames, 
viewing summaries, computing rank, counting symbols in a range, counting membrane regions, and computing region fractions over time.

To compile:

g++ -std=c++17 main.cpp WaveletTree.cpp -o wavelet_md

To run:

./wavelet_md

Example frame files:
frame0.txt (mostly negative bins)
frame1.txt (midplane-dominant)
frame2.txt (mostly positive bins)

Example automated test files are also included:
test_load_one.in
test_summary.in
test_rank.in
test_count_symbol.in
test_region.in
test_timeseries_midplane.in
test_timeseries_cyto.in

Run a test with:

./wavelet_md < test_filename.in

This allows reproducible testing of the program's features.

This project connects ideas from data structures (succinct trees, rank/select queries) with scientific computing and biophysics.
It demonstrates how compressed data structures can reduce computational cost while preserving analytical capability.