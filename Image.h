//================================================
// Image.h
//
// by Tejas Jha 19 September 2018
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

    //Statistics to Collect for Multivariate Guassian
    //vector<string> classes(3);
    //vector<double> mean(3);
    //vector<double> stddev(3);
    
public:
    Image() : nx(0), ny(0) {}

	/** \brief filter: filters image
			filters image of the input number and anything lower
	\param[in] filter_name
		Gives the name a filter.
	\param[in] param
		The number used to determine which numbers get filtered.
	\returns
		nothing

	\par Exceptions
		bad exceptions when:
			nx <=0
			ny<=0
			image size is not nx*ny
	
	\par Invalid Argument
		Unknown Filter Name.

	\par Example
		Make a 10 by 10 image, fill it with values of 50. Get a slice and set all the
		values in that slice to 25. Then run the filter function with 30 as the input
		param.
		
	\code
		Image img;
		string ourFilter;
		img.create_empty(10, 10, 3);
		img.set(50);
		Slice slc = img.slice(1, 1, 4, 4);
		if(img.valid_slice(slc) == true){
			img.set_slice(slc, 25);
			img.filter(ourFilter, 30);
		}
	\endcode
	\par DETAILS
		Original code
	*/
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
    //
	/** \brief valid_slice: checks to see if slice is valid
			Helper function to check whether given slice is valid for current image
	\param[in] slc
		the inputted slice that gets checked for its validity
	\returns
		bool, True if success, False if failure

	\par Exceptions
		Bad Exceptions when:
			x-coordinate too large for image
			y-coordinate too large for image
			slice dx value is too large for image
			slice dy value is too large for image

	\par Example
		Create empty image, create a slice, check the slice by 
		running the function with values 1,1,4,4

	\code
		Image img;
		img.create_empty(10, 10, 3);
		Slice slc = img.slice(1, 1, 4, 4);
		bool val = img.valid_slice(slc);
	\endcode
	\par DETAILS
		
	*/
    bool valid_slice(Slice &slice);

    //Given (nx, ny), create an image of all zeros (also implemented as ctor above)
	/** \brief create_empty: creates an empty image
			Given (nx, ny), create an image of all zeros (also implemented as ctor above)
	\param[in] inx
		the width of the desired image
	\param[in] iny
		the height of the desired image
	\param[in] channels
		accounts for many channels
	\returns
		nothing

	\par Exceptions
		None

	\par Example
		Run the create image function

	\code
		Image img;
		img.create_empty(10, 10, 3);
	\endcode
	\par DETAILS
		
	*/
	//! Creates empty image
    void create_empty(unsigned int inx, unsigned int iny, unsigned int channels);

    //Given a value, set entire image to this value
	/** \brief get: gets value at coordinate (x, y)
	
	\param[in] x
		index in x-direction
	\param[in] y
		index in y-direction
	\param[in] band
		channel index
	\returns
		int

	\par Exceptions
		bad exception:
						index out of bounds

	\par Example
		Create a 10x10 empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Then use the get function to find the value at 
		coordinate (4, 4) and set an int variable to be initialized with it.

	\code
		Image img;
		img.create_empty(10, 10, 3);
		int value = img.get(4, 4, 2);
	\endcode
	\par DETAILS
		
	*/
    int get(int x, int y, int band);

	/** \brief set: sets all values in the image to the given input double
			Given a value, set entire image to this value
	\param[in] val
		the value that fills the entire image
	\returns
		nothing

	\par Exceptions
		None

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. 

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
	\endcode
	\par DETAILS
		there very likely exists an algorithm in std algorithm to do this all in one line
		
	*/
    void set(double val);

    //Given a value and slice, set entire slice to value
	/** \brief set_slice: modifies given slice to the given value
			Given a value and slice, set entire slice to value
	\param[in] slc
		the slice to be modified by reference
	\param[in] val
		the value that should fill the given slice
	\returns
		nothing

	\par Exceptions
		Bad Exception when:
			the x-coordinate is larger than the Image
			the y-coordinate is larger than the Image
			width of the slice is larger than the image
			height of the slice is larger than the image

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50, from there create a slice object. Then use the 
		set_slice function to fill the specific slice of the image with 25.

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		Slice newslc = img.slice(1, 1, 4, 4);
		if(img.valid_slice(slc)==true){
			
			img.set_slice(newslc, 25);
		}
	\endcode
	\par DETAILS
		
		
	*/
    void set_slice(Slice &slc, double val);

    //Given a 2n sized vector of its, recode an integer image
    //The first value in every integer pair represents current value
    //The second value int he pair represents the changed to value
    //Assumption: each pixel with the same values will be changed the same way

	/** \brief recode: replaces an integer image
			Given a 2n sized vector of its, recode an integer image
			The first value in every integer pair represents current value
			The second value in the pair represents the changed to value
	\param[in] vec
		the vector that holds the value of the original integer 
		and the new integer 
	\returns
		nothing

	\par Exceptions
		Bad Exception when:
			inputted vector is not of an even length

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Create a vector of even length that has  
		values to be modified followed by their modified values

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		vector<int> v;
		v.push_back(50);
		v.push_back(33);
		img.recode(v);
	\endcode
	\par DETAILS
		Assumption: each pixel with the same values will be changed the same way
		May need more experimentations to ensure good complexity
		is it ok for vec to have "current values" not present?
		or should this be flagged as an error? 
		
	*/
    void recode(std::vector<int> &vec);

    //Given a slice and recode vector, performs similar function as above,
    //but only for the given slice
	/** \brief recode: replaces an integer image of a slice
			Given a 2n sized vector of its, recode an integer image slice
			The first value in every integer pair represents current value
			The second value in the pair represents the changed to value
	\param[in] slc
		the given slice that needs to be changed the vector
	\param[in] vec
		the vector that holds the value of the original integer 
		and the new integer 
	\returns
		nothing

	\par Exceptions
		Bad Exception when:
			inputted vector is not of an even length
			invalid slice

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Find a slice to modify using the 
		slice function. Create a vector of even length that has values 
		to be modified followed by their modified values

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		Slice slc = img.slice(1, 1, 4, 4);
		vector<int> v;
		v.push_back(50);
		v.push_back(33);
		if(img.valid_slice(slc) == true){
			img.recode_slice(slc, v);
		}
	\endcode
	\par DETAILS
		Currently assuming lack of appearance of "current value" in image 
        is not an issue
	*/
    void recode_slice(Slice &slc, std::vector<int> &vec);

    // Calculates the average using nXm boxes where 
    // n is the number of rows and m is the number of columns
    // they are represented in the function below as row
    // and column
    // The function returns another Image that corresponds to 
    // the corresponding averages done
    // Note, edges are calculated by coppying from the nearest side
    // element
    // ASSUME that rows and columns are less than size of image
    // Assume rows and columns are odd
    // If not, reject

	/** \brief box_average: finds average of nxm of image
			Calculates the average using nXm boxes where 
    		 n is the number of rows and m is the number of columns
    		 they are represented in the function below as row
    		 and column
    		 The function returns another Image that corresponds to 
     		the corresponding averages done
	\param[in] rows
		the number of rows to set the boundaries for the average 
	\param[in] columns
		the number of columns to set the boundaries for the average
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Specify a certain number of rows and columns(5x5)
		and then use the box_average function.

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		int rows = 5;
		int columns = 5;
		Image newImg = img.box_average(rows, columns);
	\endcode
	\par DETAILS
		Note, edges are calculated by copying from the nearest side
     	element
     	ASSUME that rows and columns are less than size of image
     	Assume rows and columns are odd
     	If not, reject
	*/
    Image box_average(int rows, int columns);

    // Calculates the maximum using nXm boxes where 
    // n is the number of rows and m is the number of columns
    // they are represented in the function below as row
    // and column
    // The function returns another Image that corresponds to 
    // the corresponding averages done
    // Note, edges are calculated by copying from the nearest side
    // element
    // ASSUME that rows and columns are less than size of image
    // Assume rows and columns are odd
    // If not, reject

	/** \brief box_max: finds max of nxm of image
			n is the number of rows and m is the number of columns
    		they are represented in the function below as row
    		and column
    		The function returns another Image that corresponds to 
    		the corresponding maximum done
	\param[in] rows
		the number of rows to set the boundaries for the box 
	\param[in] columns
		the number of columns to set the boundaries for the box
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Specify a certain number of rows and columns(5x5)
		and then use the box_max function.

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		int rows = 5;
		int columns = 5;
		Image newImg = img.box_max(rows, columns);
	\endcode
	\par DETAILS
		Note, edges are calculated by copying from the nearest side
     	element
     	ASSUME that rows and columns are less than size of image
     	Assume rows and columns are odd
     	If not, reject
	*/
    Image box_max(int rows, int columns);

    // Calculates the minimum using nXm boxes where 
    // n is the number of rows and m is the number of columns
    // they are represented in the function below as row
    // and column
    // The function returns another Image that corresponds to 
    // the corresponding averages done
    // Note, edges are calculated by coppying from the nearest side
    // element
    // ASSUME that rows and columns are less than size of image
    // Assume rows and columns are odd
    // If not, reject

	/** \brief box_min: finds min of nxm of image
			n is the number of rows and m is the number of columns
    		they are represented in the function below as row
    		and column
    		The function returns another Image that corresponds to 
    		the corresponding minimum done
	\param[in] rows
		the number of rows to set the boundaries for the box 
	\param[in] columns
		the number of columns to set the boundaries for the box
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Specify a certain number of rows and columns(5x5)
		and then use the box_min function.

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		int rows = 5;
		int columns = 5;
		Image newImg = img.box_min(rows, columns);
	\endcode
	\par DETAILS
		Note, edges are calculated by copying from the nearest side
     	element
     	ASSUME that rows and columns are less than size of image
     	Assume rows and columns are odd
     	If not, reject
	*/
    Image box_min(int rows, int columns);

    //shrink/grow: given an image and new nx and ny, shrink and/or grow the image using bi-linear interpolation.

	/** \brief shrink_grow: shrinks or grows the image
			given an image and new nx and ny, shrink and/or 
			grow the image using bi-linear interpolation.

	\param[in] new_nx
		the new image width
	\param[in] new_ny
		the new image height
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create an empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Specify the new images width and height and call
		the  function

	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		int nx = 5;
		int ny = 5;
		Image newImg = img.shrink_grow(nx, ny);
	\endcode
	\par DETAILS

	*/
    Image shrink_grow(int new_nx, int new_ny);

	//Image: stretch
	// Using two provided quadratic equations (functions) for each x and y coords,
	// calculates the new coordinates and determines new size of the image
	// Returns new image
	// Assuming the integrity of the image is maintained (corners are ins same
	// positions relative to each other)

	/** \brief stretch: Using two provided quadratic equations (functions) for each x and y coords,
						calculates the new coordinates and determines new size of the image
						Returns new image
						Assuming the integrity of the image is maintained (corners are ins same
						positions relative to each other)

	\param[in] (*fx)(double)
		quadratic equation for the x-coordinate to calculate the new coordinate 
	\param[in] (*fy)(double)
		quadratic equation for the y-coordinate to calculate the new coordinate
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create a 10x10 empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Specify the x-coordinate quadratic equation and 	
		y-coordinate quadratic equation, then call the stretch function.
	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		double (*fx)(0) = 0;
		double (*fy)(0) = 0;
		Image newImg = img.shrink_grow((*fx)(0), (*fx)(0));
	\endcode
	\par DETAILS

	*/
	Image stretch( double (*fx)(double), double (*fy)(double) );

	// Skew Function
	// Given: a real number of pixels to shift per row (assume integer)
	// create a new image that's big enough
	// shift each row into the new image
	// Assume positive shift is right and negative shift is left

	/** \brief skew: create a new image that's big enough
						shift each row into the new image

	\param[in] shift
		a vector of ints that contains the real number of pixels to shift
		per row
	\returns
		Image

	\par Exceptions
		None

	\par Example
		Create a 10x10 empty image using the create_empty function, then use set to set all 
		values in that empty image to 50. Fill the vector with 4's until 10 spots fill
		up(corresponds with each row). Then call the skew function to create a skewed image
	\code
		Image img;
		img.create_empty(10, 10, 3);
		img.set(50);
		vector<int> row = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
		Image newImg = img.skew(row);
	\endcode
	\par DETAILS

	*/
	Image skew(vector<int> &shift);
	
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


#endif
