//========================================================================
// main.cpp
//
// by Tejas Jha, Feb 1, 2018
// Adapted from main.cpp used in hw03 by Leland Pierce
//
//======================================================================
//
// usage: Playing with hdf5 image
// where:
//    
//
//======================================================================

#include <iostream>

#include "Image.h"

#define ERROR 1
#define SUCCESS 0

void usage();

//#define DEBUG


//--------------------------------------------------------------------
int
main(int argc, char* argv[])
{
  return 0;
  /*
  double thresh;
  
#ifdef DEBUG
  cerr << "argc="<<argc<<endl; //debug
#endif

  // 1. must have one argument: the threshhold:
  if(argc != 2) {
    usage();
    exit(ERROR);
  }// endif


#ifdef DEBUG
  cerr << "at 2"<<endl;
#endif
  // 2. parse the value of threshhold, die if error
  thresh = atof(argv[1]);
  if(thresh == 0.0) {
    cerr << "Error specifying threshhold: "<<argv[1]<<endl;
    exit(ERROR);
  }// endif





#ifdef DEBUG
  cerr << "at 3"<<endl;
#endif
  // 3. read data from the input file:
  Image input_image;
  // Create a 3 channel empty image
  //input_image.create_empty(10,10,3);
  // cout << input_image
  try {
    //cin >> input_image;
    input_image.read_file("Emperor_penguins.jpg");
  }
  catch (...) {    
    cerr << "Error reading data"<<endl;
    exit(ERROR);
  }// endif


  //input_image.set(100);
  // cout << input_image



#ifdef DEBUG
  cerr << "at 4"<<endl;
#endif
  // 4. process the data:
  //Image output_image;
  try{
      input_image.filter("threshhold",thresh);    
  }// end-try
  catch (...){
    cerr << "Error filtering data"<<endl;
    exit(ERROR);
  }// end-catch
  
  
#ifdef DEBUG
  cerr << "at 5"<<endl;
#endif
  // 5. write data to output file:
  try{
    input_image.write_file("thresh_Penguins.tiff");
  }//end-try
  catch (...) {
    cerr << "Error writing data"<<endl;
    exit(ERROR);
  }//end-catch
*/
  return(SUCCESS);
  
}// end main()



//-----------------------------------------------------------------
// function usage:
// input parameters:  none
// output parameters: none
// return-value: none
// summary: writes a usage summary to stderr
//-----------------------------------------------------------------
void
usage()
{
    cerr<<"usage: ideal_freshman_program thresh < infile > outfile"<<endl;
    cerr<<"where:"<<endl;
    cerr<<"      thresh is a number (int or float) for the threshhold operation"<<endl;
    cerr<<"      infile is the name of an input file with image data in it"<<endl;
    cerr<<"              the input data is text separated by whitespace"<<endl;
    cerr<<"              the data is expected to multiple rows, with the same"<<endl;
    cerr<<"              number of entires on each line. May be ints or floats."<<endl;
    cerr<<"              anything else is an error."<<endl;
    cerr<<"       outfile is the name of the output file. will be over-written"<<endl;
    cerr<<"               if it already exists"<<endl;
    cerr<<" The result is printed to 'outfile', where any number less than or equal to"<<endl;
    cerr<<" the provided threshhold will be set to zero. other numbers remain the same."<<endl;
}// end: usage()
