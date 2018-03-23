Image.h
Earlier this month
Mar 1
T
You uploaded an item
C
Image.h

//================================================
// Image.h
//
// by Tejas Jha, Feb 1, 2018
// Adapted from HW03 written by Leland Piece
//
// Header file for the Image class.
//================================================
#ifndef IMAGE_H
#define IMAGE_H


#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

#include "H5Cpp.h"
using namespace H5;

class Slice;

//-----------------------------------------------------------------
// Image class:
// summary: An image is a 2D array of doubles.
//          This class provides a default constructor, along with
//          a way to read an image from a stream, write an image to a stream,
//          and a way to filter an image to create another image.
// typical usage:
//          #include "Image.h"
//          create an empty image:         Image im1;
//          read image data from a stream: instream >> im1;
//          write image to a stream:       outstream << im1;
//          threshhold an image (set anything <=4 to zero)
//                               Image im2;
//                               im2 = im1.filter("threshhold",4.);
//          obtain image dimensions:
//                               int nx = im1.xsize();
//                               int ny = im1.ysize();
// caveats:
//          So far there are no public methods for getting or setting
//          the image data.
//          Implements multiple channels
//
// internals:
//          This class uses 3 private instance variables for maintaining
//          the state of the object:
//          2 integers, keeping track of the size in each dimension: nx, ny
//          a vector of doubles, for storing a 1D representation of the
//            image data.
//            indexing is like: [iy*nx + ix] where we want (ix,iy)
//          The user is not allowed to set the dimensions.
//          Currently there is no way for the user to set or get
//            values within the image (pixels).
//
//-----------------------------------------------------------------


class Image {
    
private:
    vector <vector<double> > data;
    int nx;
    int ny;
    int xsize(int inx) {nx = inx;}
    int ysize(int iny) {ny = iny;}
    
public:
    Image() : nx(0), ny(0) {}
    void filter(string filter_name, double param);
    int xsize(){ return nx; }
    int ysize(){ return ny; }
    size_t ChannelCount() { return data.size();}
    // use default destructor
    
    //Currently ostream and istream do no function for multiple channels
    //friend ostream& operator<< (ostream &out, Image &im);
    //friend istream& operator>> (istream  &in, Image &im);
    
    void  read_file(const std::string &infile);
    void  write_file(const std::string &outfile);

    //Given values for a slice, creates a slice from current image
    Slice slice(unsigned int inx0, unsigned int iny0, unsigned int indx, unsigned int indy);

    //Helper function to check whether given slice is valid for current image
    bool valid_slice(Slice &slice);

    //Given (nx, ny), create an image of all zeros (also implemented as ctor above)
    void create_empty(unsigned int inx, unsigned int iny, unsigned int channels);

    //Given a value, set entire image to this value
    void set(double val);

    //Given a value and slice, set entire slice to value
    void set_slice(Slice &slc, double val);

    //Given a 2n sized vector of its, recode an integer image
    //The first value in every integer pair represents current value
    //The second value int he pair represents the changed to value
    //Assumption: each pixel with the same values will be changed the same way
    void recode(std::vector<int> &vec);

    //Given a slice and recode vector, performs similar function as above,
    //but only for the given slice
    void recode_slice(Slice &slc, std::vector<int> &vec);
    
};// end class Image



////////////////////////////////////////////////////////////////
// Slice Class
//-------------------------------------------------------------
// Overview: This class seperates an image slice from its 
// original image to allow for seperation of functionality.
// However, the slice still retains access to private members of
// Image
////////////////////////////////////////////////////////////////
class Slice {
friend class Image; //Friend of Image Class (access private members)

private:
unsigned int x0; //x-value of upper left corner of slice
unsigned int y0; //y-value of upper left corner of slice
unsigned int dx; //width of slice
unsigned int dy; //height of slice

public:
Slice() : x0(0), y0(0), dx(0), dy(0) {} //Default ctor 

};

////////////////////////////////////////////////////////////////
// HDF5File Class
//-------------------------------------------------------------
// Overview: This class performs basic tasks that are done with 
// an HDF5 File, as described in the HW04 assignment.
// Below, I have implemented this class to seperate it from the 
// already written Image class. 
////////////////////////////////////////////////////////////////
class HDF5File{
public:
// Below are the functions added to this this class for HW04
// These functions make use of the HDF5 libraries.
   
// Opens HDF5 image according to imputted filename
void open_hdf5();

//Reads HDF5 image
void read_hdf5();

//Reads line of HDF5 image
void read_hdf5_line();

// Writes line of HDF5 image
void write_hdf5_line();

//Closes HDF5 image
void close_hdf5();

// Performs thresh funciton on HDF5 image
// thresh function is similar to that found in the filter
// function above, but specifically in the threshhold case
void thresh_hdf5();
};

#endif
