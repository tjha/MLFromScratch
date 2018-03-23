#!/bin/bash

g++ -c -g -std=c++0x -o Image2.o Image2.cpp `gdal-config --cflags` `gdal-config --libs` `gdal-config --dep-libs` -I ~/umich/mdp/MLFromScratch/hdf5-1.10.0-patch1/include
g++ -g -std=c++0x -o main main.cpp Image2.o `gdal-config -DD-cflags` `gdal-config --libs` `gdal-config --dep-libs` ~/umich/mdp/MLFromScratch/hdf5-1.10.0-patch1/lib/libhdf5_cpp.a ~/umich/mdp/MLFromScratch/hdf5-1.10.0-patch1/lib/libhdf5.a -lrt -lz -ldl -lm -I ~/umich/mdp/MLFromScratch/hdf5-1.10.0-patch1/include

