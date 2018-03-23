//================================================
// Image.cpp
//
// by Leland Pierce, Jan 18, 2018
//
// Implementation of the Image class.
//================================================
    
   
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <limits>
#include <algorithm>
	
#include "Image.h"
#include "Exceptions.hpp"
 
using namespace std;

/*
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>
*/

#define DEBUG

    
//-----------------------------------------------------------------
// Image constructor using input file:
// input parameter:   in: an already open istream
// summary: reads int or double ascii data from input stream
//          assumes data is arranged as ny rows each with nx numbers in it.
//          throws bad_exception if:
//               any data is not numbers
//               any row has a different number of values than the first
//               the istream is invalid
//               if there is no data
//-----------------------------------------------------------------
// Currently Implementation is commented out since it doesn't work for multiple channels
/*
    istream &operator>>( istream  &in, Image &im )
    {

        string input_line;
        int input_line_number=0;
        double d;
        
        // read input file line-by-line:
        in.exceptions ( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit );
        try {
            getline(in, input_line);
        }
        catch (...) {
            cerr << "Error in Image reader: reading from the input stream failed."<<endl;
            throw bad_exception();
        }// end-catch
        
#ifdef DEBUG
        cerr << "at 3.2"<<endl;
#endif
        
        while(!in.fail() && !in.eof() ){
            // parse the line assuming its a bunch of numbers
            size_t idx=0;
            int count_this_line = 0;
            string input_piece = input_line;
            
#ifdef DEBUG
            cerr << "at 3.3: input_piece=|"<<input_piece<<"|"<<endl;
#endif
            
            // look for next number,
            // stopping when the rest of the string is empty or has no digits
            while(!input_piece.empty() &&
                  input_piece.find_first_of("0123456789") != string::npos ){
#ifdef DEBUG
                cerr << "at 3.4: input_piece=|"<<input_piece<<"|"<<endl;
#endif
                try {
                    // parse next number from the input line:
                    d = stod(input_piece, &idx);
                }
                catch(...){
                    cerr << "Error in Image reader: converting to double."<<endl;
                    cerr << "                input: "<< input_line << endl;
                    throw bad_exception();
                }// end catch
#ifdef DEBUG
                cerr << "at 3.4.1: d="<<d<<", idx="<<idx<<endl;
#endif
                // succeeded in parsing a number, keep track of it:
                count_this_line++;
                im.data.push_back(d); // store into Image private data
                
                // get the next piece of the input line:
                input_piece = input_piece.substr(idx);
            }//end while: input_piece not empty
            
#ifdef DEBUG
            cerr << "at 3.5"<<endl;
#endif
            
            //at this point, the line is parsed into a bunch of numbers.
            //remember how many if this is the first line:
            if(input_line_number==0) {
                im.nx = count_this_line;// store into Image private data
            } else {
                // signal error if count of numbers is different:
                if(im.nx != count_this_line) {
                    cerr << "Error in Image reader: nx not consistent for line #"<<input_line_number<<endl;
                    throw bad_exception();
                }//endif
            }// endif
            
#ifdef DEBUG
            cerr << "at 3.6, nx="<<im.nx<<endl;
#endif
            
            // get the next input line:
            try{
	      getline(in, input_line);
	    }//end-try
	    catch(...) {
	    }//end-catch
            input_line_number++;
            
#ifdef DEBUG    
            cerr << "at 3.7, input_line_number="<<input_line_number<<endl;
#endif
            
        }// end while: read succeeded
        
        // set the number of lines:
        im.ny = input_line_number;// store into Image private data
        
#ifdef DEBUG
        cerr << "at 3.8, ny="<<im.ny<<endl;
#endif

        return in;
        
        
    }//end Image()
    


*/




//-----------------------------------------------------------------
// function filter:
// input parameters:  filter_name: a string with one of these values:
//                                 "threshhold"
//                                 any other value is an error
//                    param: value of the parameter needed to run the
//                           filter specified by filter_name.
//                           for filter="threshhold", param=the threshhold
// returns: nothing
// summary: filters the image in place using the algorithm specified.
//          throws bad_exception if:
//               nx or ny are zero
//               size() of image is not equal to nx*ny
//          throws invalid_argument if:
//               unknown filter_name
//-----------------------------------------------------------------
    void Image::filter(string filter_name, double param) 
    {
        
        // error-check:
        if(nx<=0){
            cerr<<"Error in filter: image x-size is <= zero."<<endl;
            throw bad_exception();
        }//endif
        if(ny<=0){
            cerr<<"Error in filter: image y-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(data[0].size() != nx*ny) {
            cerr<<"Error in filter: image-size is not nx*ny."<<endl;
            throw bad_exception();
        }//endif
        
        // perform the filter operation
        if(filter_name.compare("threshhold")==0){
            
            double thresh = param; // for readability


            cout << "thresh = " << thresh << endl; //debug


            //Go through all channels
            for(int c = 0; c < data.size(); c ++){
                for(int i=0;i<nx*ny;i++){
                cout << data[c][i] << endl;//debug
                    if(data[c][i] <= thresh) {
                        data[c][i]=0.0;
                        cout <<"setting to 0" << endl;//debug
                    }// endif
                }//endfor
            }
            
            //add more comparisons with other strings to implement other filters here.....
            
        }
        else {
            cerr<<"Error in filter: unknown filter name: "<<filter_name<<endl;
            throw invalid_argument("");
        }//endif

    }// end filter()
    


//-----------------------------------------------------------------
// overloaded operator <<:
// input parameters:  out: an already open ostream
//                    im:  an image to write out
// summary: writes double ascii data to output stream
//          throws bad_exception if:
//               nx or ny are zero
//               size() of image is not equal to nx*ny
//-----------------------------------------------------------------   
// Currently Implementation should work for multiple channels
// However, it is giving me private variable accessed errors
/*
    ostream& operator<< (ostream &out, Image &im)
    {
        
        
        // other checks
        if(im.nx<=0){
            cerr<<"Error in Image output: image x-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(im.ny<=0){
            cerr<<"Error in Image output: image y-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(im.data.size() != im.nx*im.ny) {
            cerr<<"Error in Image output: image-size is not nx*ny."<<endl;
            throw bad_exception();
        }//endif
        
        
        for(int c = 0; c < im.data.size(); c++){
            cout << "Displaying Data for Channel: " << c+1 << endl;
            // write out: nx doubles per line:
            for(int j=0;j<im.ny;j++){
                for(int i=0;i<im.nx;i++){
                   out << im.data[c][ j*im.nx + i ]<<" ";          
                }//endfor: i
            out << endl;
        }//endfor: j
        }
        
        
        
        return out;
    }// end: operator<<

*/



// void Image::read_file(const std::string &infile){
//     hid_t file, dataset;

//     file = H5Fopen("a1.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
//     dataset = H5Dopen(file, "raster");

// }


//-----------------------------------------------------------------
    // read a single-channel image file
    // infile is the name of the intput image file
    // if the image is more than 1 channel, it just uses the first.
    // for now, it creates a float channel.
    // returns the newly-created Image
//-----------------------------------------------------------------

    void Image::read_file(const std::string &infile){
    // 1. open the data file
    // 2. figure out data characteristics
    // 3. create empty raster
    // 4. fill raster.
    FileOpenErrorException FileOpenError;

#ifdef DEBUG
    cout << "read_file: at 1"<<endl;
#endif
    // 1. open the data file
    /*
    GDALDataset *poDataset;
    GDALAllRegister();
    char cname[infile.length()+10]; // need a copy because it's declared const
    strcpy(cname, infile.c_str());
    poDataset = (GDALDataset *) GDALOpen(cname, GA_ReadOnly );
    if( poDataset == NULL ) throw FileOpenError;

#ifdef DEBUG
    cout << "read_file: at 2"<<endl;
#endif
    // 2. figure out data characteristics
    GDALRasterBand *poBand;
    int num_rasters = poDataset->GetRasterCount(); // Get number of Rasters in Image

    data.resize(num_rasters); // Make data Matrix have rows equal to number of Rasters


    // Go through each Raster in Image
    for(int c = 0; c < num_rasters; c++){
        poBand = poDataset->GetRasterBand( c + 1 ); // the first channel.
        int nx = poBand->GetXSize();
        int ny = poBand->GetYSize();

        // 3. create empty raster (for efficiency)
        data[c].resize(nx*ny);


        // 4. temporary place for one line of the image:
        float *buf;
        buf = (float *) CPLMalloc(sizeof(float)*nx);

        // 5. fill the data vector:
        int pix=0;
        for(int iy=0;iy<ny;++iy){
            CPLErr err = poBand->RasterIO( GF_Read, 
                            0, iy, nx, 1,
                            buf, nx, 1, GDT_Float32,
                            0, 0 );
            for(int ix=0;ix<nx;++ix) {
                data[c][pix++]=buf[ix];
            }//endfor: ix

        } // endfor: iy

               CPLFree(buf);
        // 6. finish specifiying the new image:
        this->nx = nx;
        this->ny = ny;

    }

        GDALClose(poDataset);


    */
    

    }// end: Image::read_file


//-----------------------------------------------------------------
// Write out the image data as a 1-channel jpeg.
//-----------------------------------------------------------------
    void Image::write_file(const std::string &outfile){
    
        /*
      const char *pszFormat = "GTiff";
      GDALDriver *poDriver;
      char **papszMetadata;
      poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
      if( poDriver == NULL ) throw std::runtime_error("invalid GDAL driver");

      papszMetadata = poDriver->GetMetadata();

      //temporary:
      //if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
      //  printf( "Driver %s supports Create() method.\n", pszFormat );
      //if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
      //  printf( "Driver %s supports CreateCopy() method.\n", pszFormat );


      // some fake, stupid stuff for now:
      GDALDataset *poDstDS;
      char **papszOptions = NULL;
      poDstDS = poDriver->Create( outfile.c_str(), nx, ny, 1, GDT_Byte,
                                  papszOptions );
      
      double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
      OGRSpatialReference oSRS;
      char *pszSRS_WKT = NULL;
      GDALRasterBand *poBand;

      poDstDS->SetGeoTransform( adfGeoTransform );
      oSRS.SetUTM( 11, TRUE );
      oSRS.SetWellKnownGeogCS( "NAD27" );
      oSRS.exportToWkt( &pszSRS_WKT );
      poDstDS->SetProjection( pszSRS_WKT );
      CPLFree( pszSRS_WKT );

    //Go through each Raster
      for(int c = 0; c < poDstDS->GetRasterCount(); c++){
            poBand = poDstDS->GetRasterBand(c+1);
            //poBand->RasterIO( GF_Write, 0, 0, 512, 512,
            //                  abyRaster, 512, 512, GDT_Byte, 0, 0 );


            // 4. temporary place for one line of the image:
            GByte buf[nx*ny];


            // 5. fill the output:
            int pix=0;
            int dd;
            for(int iy=0;iy<ny;++iy){
            
            for(int ix=0;ix<nx;++ix) {

                //force output to be a single byte per pixel:
                dd= data[c][iy*nx+ix];
                if(dd<0) {
                buf[ix]=0;
                } else if(dd>255) {
                buf[ix]=255;
                } else {
                buf[ix]=dd;
                }//endif

            }//endfor: ix

            CPLErr err = poBand->RasterIO( GF_Write, 
                            0, iy, nx, 1,
                            buf, nx, 1, GDT_Byte,
                            0, 0 );

            } // endfor: iy
      }



      GDALClose( (GDALDatasetH) poDstDS );
        */

    }// end: Image::write_file


/////////////////////////////////////////////////////////////
// Image: create_empty function
// Input Parameters:
//    1) nx (width of image)
//    2) ny (height of image)
// Overview: Generates an image with input size parameters
// with all 0 values. Returns an Image.
////////////////////////////////////////////////////////////

void Image::create_empty(unsigned int inx, unsigned int iny, unsigned int channels){
    if(inx <= 0 || iny <= 0 || channels <= 0){
        cerr << "invalid image dimensions" << endl;
        throw bad_exception();
    }

    Image img;
    img.nx = inx;
    img.ny = iny;
    img.data.resize(channels);
    for(size_t c = 0; c < img.data.size(); c++){
        img.data[c].resize(inx*iny);
    }
    *this = img;
}

/////////////////////////////////////////////////////////////
// Image: get function                                     //
// Input Parameters:                                       //
//    1) x    (index in x-direction)                       //
//    2) y    (index in y-direction)                       //
//    3) band (channel index)                              //
// Overview: gets value at coordinate x,y                  // 
/////////////////////////////////////////////////////////////
int Image::get(int x, int y, int band){
    if(x < 0 || y < 0 || band < 0 || band > ChannelCount() || x > xsize() || y > ysize()){
        cerr << "index out of bounds" << endl;
        throw bad_exception();
    }

    return data[band][y * xsize() + x];
}

/////////////////////////////////////////////////////////////
// Image: set function
// Input Parameter: double value
// Overview: Sets all values in image to input value (all channels)
/////////////////////////////////////////////////////////////
void Image::set(double val){
    //loop over all values and set to new val 
    // Note: there very likely exists an algorithm
    // in std algorithm to do this all in one line
    for(size_t c = 0; c < this->data.size();c++){
        for(size_t i = 0; i < this->data[c].size(); i++){
            this->data[c][i] = val;
        }
    }
}

/////////////////////////////////////////////////////////////
// Slice Validity Function (Recently Added)
// Overview: This function checks whther the inputted slice
// is valid for the current Image (within range)
// returns true if valid, false otherwise
/////////////////////////////////////////////////////////////
bool Image::valid_slice(Slice &slc){
    if(slc.x0 > nx){
        std::cerr << "slice x0 value too large for image" << std::endl;
        return false;
    }
    if(slc.y0 > ny){
        std::cerr << "slice y0 value too large for image" << std::endl;
        return false;
    }
    if(slc.x0 + slc.dx > nx) {
        std::cerr << "slice dx value is too large for image" << std::endl;
        return false;
    }
    if(slc.y0 + slc.dy > ny) {
        std::cerr << "slice dy value is too large for image" << std::endl;
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////
// Image: slice function
// Input Parameters:
//    1) inx0 (x-value of upper corner of slice)
//    2) iny0 (y-value of upper corner of slice)
//    3) indx (width of slice)
//    4) indy (height of slice)
// Overview: Creates Slice given slice paramers
// Exceptions: Ensures that slice does not extend past image
// height and width
/////////////////////////////////////////////////////////////
Slice Image::slice(unsigned int inx0, unsigned int iny0, unsigned int indx, unsigned int indy){
    Slice slc;
    if(inx0 > nx){
        std::cerr << "slice x0 value too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(iny0 > ny){
        std::cerr << "slice y0 value too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(inx0 + indx > nx) {
        std::cerr << "slice dx value is too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(iny0 + indy > ny) {
        std::cerr << "slice dy value is too large for image" << std::endl;
        throw std::bad_exception();
    }


    slc.x0 = inx0;
    slc.y0 = iny0;
    slc.dx = indx;
    slc.dy = indy;
    return slc;
} 

//////////////////////////////////////////////////////////////
// Image: set_slice function
// Input Parameters:
//    1) Slice slc
//    2) double val (value to which image's slice is set to)
// Overview: Modifies slice of current Image to value
// Exceptions: Ensure that provided slice is valid for image
//////////////////////////////////////////////////////////////
void Image::set_slice(Slice &slc, double val){
    //First verify that slice falls inside current image
    if(slc.x0 > nx){
        std::cerr << "slice x0 value too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(slc.y0 > ny){
        std::cerr << "slice y0 value too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(slc.x0 + slc.dx > nx) {
        std::cerr << "slice dx value is too large for image" << std::endl;
        throw std::bad_exception();
    }
    if(slc.y0 + slc.dy > ny) {
        std::cerr << "slice dy value is too large for image" << std::endl;
        throw std::bad_exception();
    }

    //Go Through each raster
    for(size_t c = 0; c < data.size(); c++){
        //Adjust data vector to new value (val) inside slice
        for(size_t h = slc.y0; h < slc.y0 + slc.dy; h++){
            for(size_t w = slc.x0; w < slc.x0 + slc.dx; w++){
             data[c][nx*h + w] = val;
            }
        }
    }
    
}

///////////////////////////////////////////////////////////////
// Image: recode function
// Input Parameter: vector of 2n ints representing current values
// and values to be changed to
// Overview: Modifies current image values to be new values 
// represented in the vector according to the integer pairs
// Exceptions: Ensures vec is of even length 
//             ignores values that are not present
// Note: May need more experimentations to ensure good complexity
// Also: is it ok for vec to have "current values" not present?
// or should this be flagged as an error? 
///////////////////////////////////////////////////////////////
void Image::recode(std::vector<int> &vec){
    if(vec.size() & 2 != 0){
        std::cerr << "Inputted vector in recode function not of even length" << std::endl;
        throw std::bad_exception();
    }

    //Go Through all Rasters
    for(size_t c = 0; c < data.size(); c++){
        for(size_t i = 0; i < vec.size(); i = i + 2){
        //bool found_value = false; //Store whether value in vec was seen
            for(size_t j = 0; j < data[c].size(); j++){
                if(data[c][j] == vec[i]){ //Find value in vec (first int in pair)
                    //found_value = true;
                    data[c][j] = vec[i+1];
                }
            }
        //Code below is included if "current values" not being
        // present is a problem for this function
        /*f(!found_value){ //if current value was not found
            std::cerr 
        }*/
        }
    }
    
}

//////////////////////////////////////////////////////////////////////
// Image: recode_slice
// All the same as the above recode function, but for a specified slice
///////////////////////////////////////////////////////////////////////

void Image::recode_slice(Slice &slc, std::vector<int> &vec){
    //Ensure slice is of even length
    if(vec.size() % 2 != 0){
        std::cerr << "Inputted vector in recode function not of even length" << std::endl;
        throw std::bad_exception();
    }
    //Ensure that slice is valid for current image
    if(!Image::valid_slice(slc)){
        std::cerr << "Invalid Slice used in recode_slice function" << std::endl;
        throw std::bad_exception();
    }


    //Go through each Raster
    for(size_t c = 0; c < data.size(); c++){
        //Go through and change values accroding to vec
        //Currently assuming lack of appearance of "current value" in image 
        //is not an issue
        for(size_t i = 0; i < vec.size(); i = i + 2){ //iterate through vec
           for(size_t h = slc.y0; h < slc.y0 + slc.dy; h++){ //iterate through height of slice
               for(size_t w = slc.x0; w < slc.x0 + slc.dx; w++){ //iterate through width of slcie
                   if(data[c][nx*h + w] == vec[i]){ //check if value equals vec[i] value
                       data[c][nx*h + w] = vec[i+1];
                   }
                }
            }
        }
    }


}

//////////////////////////////////////////////////////////////////////////////
// Image: box_average
// Finds average of nxM where n is rows and M is columns
// Returns new image
/////////////////////////////////////////////////////////////////////////////

Image Image::box_average(int rows, int columns){
    if(rows >= ny || columns >= nx){
        cerr << "boxcar dimensions cannot be larger than image" << endl;
        throw std::bad_exception();
    } // Assert even rows and column values for function and less than size of image
    if(rows <= 0 || columns <= 0 || (rows <= 1 && columns <= 1)){
        cerr << "Invalid boxcar must be greater than 0x0" << endl;
        throw std::bad_exception();
    }
    Image new_image;
    new_image.nx = nx;
    new_image.ny = ny;
    new_image.data = data;

/*
    for (int y = 0; y < data.size(); y++){
        for(int x = 0; x < data[0].size(); x++){
            cout << data[y][x];
        }
        cout << endl;
    }
    */


    // Set inner box

    for(int raster = 0; raster < data.size(); raster++){ // go through each raster
        for(int y = rows / 2; y < ny - rows / 2; y++){ // go through each row
            for(int x = columns / 2; x < nx - columns/2; x++){ // each column
            double sum = 0; // Get sum for box
            for(int y1 = 0; y1 < rows; y1++){ // get shift y
                for(int x1 = 0; x1 < columns; x1++){ // get shift x
                    int starting_location = nx*y + x;    
                    int vertical_shift = ((-1)*(rows/2) + y1)*nx; //calculate shifts
                    int horizontal_shift = (-1)*(columns/2) + x1;
                    sum += data[raster][starting_location + vertical_shift + horizontal_shift]; //index for sum on shift
                }
            }
            double average = sum / (rows * columns);
            new_image.data[raster][nx*y + x] = average; // save into new_image as average
            
            }

        }
    }
    
    //Fix Edges to be copies of closest edge
    for(int raster = 0; raster < data.size(); raster++){
        //Fill upper strip
        for(int y = 0; y < rows/2; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(rows/2) + x];
            }
        }
        //fill lower strip
        for(int y = ny - rows/2; y < ny; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(ny-rows/2) + x];
            }
        }

        //Fill left strip
        for(int y = 0; y < ny; y++){
            for (int x = 0; x < columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + columns/2];
            }
        }
        //FIll right strip
        for(int y = 0; y < ny; y++){
            for (int x = nx - columns/2; x < nx; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + nx - columns/2];
            }
        }
    }

    return new_image;

}

/////////////////////////////////////////////////////////////////////////////
// Image: box_max                                                          //
// Finds maximum of nxM where n is rows and M is columns                   //
// Returns new image                                                       //
/////////////////////////////////////////////////////////////////////////////
Image Image::box_max(int rows, int columns){
    assert(rows < ny);
    assert(columns < nx); 
    Image new_image;
    new_image.nx = nx;
    new_image.ny = ny;
    new_image.data = data;

    for(int raster = 0; raster < data.size(); raster++){ // go through each raster
        for(int y = rows / 2; y < ny - rows / 2; y++){ // go through each row
            for(int x = columns / 2; x < nx - columns/2; x++){ // each column
            double max = std::numeric_limits<double>::min(); //set min to be max value
            for(int y1 = 0; y1 < rows; y1++){ // get shift y
                for(int x1 = 0; x1 < columns; x1++){ // get shift x
                    int starting_location = nx*y + x;    
                    int vertical_shift = ((-1)*(rows/2) + y1)*nx; //calculate shifts
                    int horizontal_shift = (-1)*(columns/2) + x1;
                    double val = data[raster][starting_location + vertical_shift + horizontal_shift];
                    if(val > max){
                        max = val;
                    }
                }
            }
            new_image.data[raster][nx*y + x] = max; // save into new_image as average            
            }
        }
    }

    //Fix Edges to be copies of closest edge
    for(int raster = 0; raster < data.size(); raster++){
        //Fill upper strip
        for(int y = 0; y < rows/2; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(rows/2) + x];
            }
        }
        //fill lower strip
        for(int y = ny - rows/2; y < ny; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(ny-rows/2) + x];
            }
        }

        //Fill left strip
        for(int y = 0; y < ny; y++){
            for (int x = 0; x < columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + columns/2];
            }
        }
        //FIll right strip
        for(int y = 0; y < ny; y++){
            for (int x = nx - columns/2; x < nx; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + nx - columns/2];
            }
        }


    }

    return new_image;
}

//////////////////////////////////////////////////////////////////////////////
// Image: box_min
// Finds minimum of nxM where n is rows and M is columns
// Returns new image
/////////////////////////////////////////////////////////////////////////////
Image Image::box_min(int rows, int columns){
    assert(rows < ny);
    assert(columns < nx); 
    Image new_image;
    new_image.nx = nx;
    new_image.ny = ny;
    new_image.data = data;

    for(int raster = 0; raster < data.size(); raster++){ // go through each raster
        for(int y = rows / 2; y < ny - rows / 2; y++){ // go through each row
            for(int x = columns / 2; x < nx - columns/2; x++){ // each column
            double min = std::numeric_limits<double>::max(); //set min to be max value
            for(int y1 = 0; y1 < rows; y1++){ // get shift y
                for(int x1 = 0; x1 < columns; x1++){ // get shift x
                    int starting_location = nx*y + x;    
                    int vertical_shift = ((-1)*(rows/2) + y1)*nx; //calculate shifts
                    int horizontal_shift = (-1)*(columns/2) + x1;
                    double val = data[raster][starting_location + vertical_shift + horizontal_shift];
                    if(val < min){
                        min = val;
                    }
                }
            }
            new_image.data[raster][nx*y + x] = min; // save into new_image as average
            
            }

        }
    }

    //Fix Edges to be copies of closest edge
    for(int raster = 0; raster < data.size(); raster++){
        //Fill upper strip
        for(int y = 0; y < rows/2; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(rows/2) + x];
            }
        }
        //fill lower strip
        for(int y = ny - rows/2; y < ny; y++){
            for (int x = columns/2; x < nx - columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*(ny-rows/2) + x];
            }
        }

        //Fill left strip
        for(int y = 0; y < ny; y++){
            for (int x = 0; x < columns/2; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + columns/2];
            }
        }
        //FIll right strip
        for(int y = 0; y < ny; y++){
            for (int x = nx - columns/2; x < nx; x++){
                new_image.data[raster][nx*y + x] = new_image.data[raster][nx*y + nx - columns/2];
            }
        }


    }

    return new_image;
}

//shrink/grow: given an image and new nx and ny, shrink and/or grow the image using bi-linear interpolation.
Image Image::shrink_grow(int new_nx, int new_ny){
    if(new_nx <= 0 || new_ny <= 0){
        std::cerr << "image dimensions must be greater than zero" << std::endl;
        throw std::bad_exception();
    }  if(xsize() <= 0 || ysize() <= 0){
        cerr << "No image to resize" << endl;
        throw std::bad_exception();
    }

    Image new_image;
    new_image.nx = new_nx;
    new_image.ny = ny;
    bool enter_1 = false;
    bool enter_2 = false;

    if(new_nx == nx && new_ny == ny){
        return *this;
    }

    if(new_nx == 0 || new_ny == 0){
        return new_image;
    }

    new_image.data = vector < vector <double> > (3, vector<double>(new_nx*ny, 0));


    // Grow in x
    if(new_nx > nx){
        enter_1 = true;
        double ratio = (double)new_nx / (double)nx;
        double corresponding_old;
        //Go through new_image data and fill accroding to where it should be in old image
        for(int raster = 0; raster < 3; raster++){
            for(int x = 0; x < new_nx; x++){
                corresponding_old = (double)x / ratio;
                int lower_index = corresponding_old;
                int upper_index;
                if(lower_index == nx - 1){
                    upper_index = nx - 1;
                }
                else {
                    upper_index = lower_index + 1;
                }

                for(int y = 0; y < ny; y++){
                    double lower_val = data[raster][nx*y + lower_index];
                    double upper_val = data[raster][nx*y + upper_index];
                    double slope = upper_val - lower_val; 
                    double delta = corresponding_old - lower_index;
                    double new_val = slope*delta + lower_val;
                    new_image.data[raster][new_nx*y + x] = new_val;
                    //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                }
             }
        }
        
        
    }

    // Shrink in x
    if(new_nx < nx){
        enter_2 = true;
        double ratio = (double) new_nx / (double)nx;
        double corresponding_old;
        //Go through _image data and fill accroding to where it should be in new image
        for(int raster = 0; raster < 3; raster++){
            for(int x = 0; x < new_nx; x++){
                corresponding_old = x / ratio;
                int lower_index = corresponding_old;
                int upper_index;
                if((double)(x + 1)/ ratio > nx - 1){
                    upper_index = nx - 1;
                }
                else {
                    double upper_loc = (double)(x + 1) / ratio;
                    upper_index = upper_loc;
                }
                //cout << lower_index << ' ' << upper_index << endl;
                for(int y = 0; y < ny; y++){
                    double lower_val = data[raster][nx*y + lower_index];
                    double upper_val = data[raster][nx*y + upper_index];
                    double slope = upper_val - lower_val; 
                    double delta = corresponding_old - lower_index;
                    double new_val = slope*delta + lower_val;
                    new_image.data[raster][new_nx*y + x] = new_val;
                    //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                }
             }
        }

    }

    // Grow in y
    if(new_ny > ny){
        Image newer_image;
        newer_image.nx = new_nx;
        newer_image.ny = new_ny;
        newer_image.data = vector < vector <double> > (3, vector<double>(new_nx*new_ny, 0));

        //return new_image;


        if(enter_1){ // If grow in x
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if(lower_index >= ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        upper_index = lower_index + 1;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else if(enter_2) {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if(lower_index >= ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        upper_index = lower_index + 1;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = data[raster][new_nx*lower_index + x];
                        double upper_val = data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        
        return newer_image;
    }

    // Shrink in y
    if(new_ny < ny){
        Image newer_image;
        newer_image.nx = new_nx;
        newer_image.ny = new_ny;
        newer_image.data = vector < vector <double> > (3, vector<double>(new_nx*new_ny, 0));

        //return new_image;


        if(enter_1){ // If grow in x
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else if(enter_2) {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = data[raster][new_nx*lower_index + x];
                        double upper_val = data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        
        return newer_image;
    }




    return new_image;

}

/*
//////////////////////////////////////////////////////////////////////////////
// Image: stretch
// Using two provided quadratic equations (functions) for each x and y coords,
// calculates the new coordinates and determines new size of the image
// Returns new image
// Assuming the integrity of the image is maintained (corners are ins same
// positions relative to each other)
/////////////////////////////////////////////////////////////////////////////
Image Image::stretch( double (*fx)(double), double (*fy)(double) ){
    double new_upper_left_x = (*fx)(0);
    double new_upper_left_y = (*fy)(0);
    double new_upper_right_x = (*fx)(this->nx);
    double new_lower_left_y = (*fy)(this->ny);
    double new_nx = ((new_upper_right_x - new_upper_left_x) > 0) ? (new_upper_right_x - new_upper_left_x) : (new_upper_left_x - new_upper_right_x);
    double new_ny = ((new_lower_left_y - new_upper_left_y) > 0) ? (new_lower_left_y - new_upper_left_y) : (new_upper_left_y - new_lower_left_y); 

    //Temporary implementation below that is not completely correct, but should still
    // function similarily to expectation.
    Image new_image = 
    new_image.nx = new_nx;
    new_image.ny = ny;

     if(new_nx == nx && new_ny == ny){
        return *this;
    }

    if(new_nx == 0 || new_ny == 0){
        return new_image;
    }

    new_image.data = vector < vector <double> > (3, vector<double>(new_nx*ny, 0));

        // Grow in x
    if(new_nx > nx){
        enter_1 = true;
        double ratio = (double)new_nx / (double)nx;
        double corresponding_old;
        //Go through new_image data and fill accroding to where it should be in old image
        for(int raster = 0; raster < 3; raster++){
            for(int x = 0; x < new_nx; x++){
                corresponding_old = (double)x / ratio;
                int lower_index = corresponding_old;
                int upper_index;
                if(lower_index == nx - 1){
                    upper_index = nx - 1;
                }
                else {
                    upper_index = lower_index + 1;
                }

                for(int y = 0; y < ny; y++){
                    double lower_val = data[raster][nx*y + lower_index];
                    double upper_val = data[raster][nx*y + upper_index];
                    double slope = upper_val - lower_val; 
                    double delta = corresponding_old - lower_index;
                    double new_val = slope*delta + lower_val;
                    new_image.data[raster][new_nx*y + x] = new_val;
                    //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                }
             }
        }
        
        
    }

    // Shrink in x
    if(new_nx < nx){
        enter_2 = true;
        double ratio = (double) new_nx / (double)nx;
        double corresponding_old;
        //Go through _image data and fill accroding to where it should be in new image
        for(int raster = 0; raster < 3; raster++){
            for(int x = 0; x < new_nx; x++){
                corresponding_old = x / ratio;
                int lower_index = corresponding_old;
                int upper_index;
                if((double)(x + 1)/ ratio > nx - 1){
                    upper_index = nx - 1;
                }
                else {
                    double upper_loc = (double)(x + 1) / ratio;
                    upper_index = upper_loc;
                }
                //cout << lower_index << ' ' << upper_index << endl;
                for(int y = 0; y < ny; y++){
                    double lower_val = data[raster][nx*y + lower_index];
                    double upper_val = data[raster][nx*y + upper_index];
                    double slope = upper_val - lower_val; 
                    double delta = corresponding_old - lower_index;
                    double new_val = slope*delta + lower_val;
                    new_image.data[raster][new_nx*y + x] = new_val;
                    //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                }
             }
        }

    }

    // Grow in y
    if(new_ny > ny){
        Image newer_image;
        newer_image.nx = new_nx;
        newer_image.ny = new_ny;
        newer_image.data = vector < vector <double> > (3, vector<double>(new_nx*new_ny, 0));

        //return new_image;


        if(enter_1){ // If grow in x
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if(lower_index >= ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        upper_index = lower_index + 1;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else if(enter_2) {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if(lower_index >= ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        upper_index = lower_index + 1;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = data[raster][new_nx*lower_index + x];
                        double upper_val = data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        
        return newer_image;
    }

    // Shrink in y
    if(new_ny < ny){
        Image newer_image;
        newer_image.nx = new_nx;
        newer_image.ny = new_ny;
        newer_image.data = vector < vector <double> > (3, vector<double>(new_nx*new_ny, 0));

        //return new_image;


        if(enter_1){ // If grow in x
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else if(enter_2) {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = new_image.data[raster][new_nx*lower_index + x];
                        double upper_val = new_image.data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        else {
            double ratio = (double)new_ny / (double)ny;
            double corresponding_old;
            //Go through new_image data and fill accroding to where it should be in old image
            for(int raster = 0; raster < 3; raster++){
                for(int y = 0; y < new_ny; y++){
                    corresponding_old = (double)y / ratio;
                    int lower_index = corresponding_old;
                    int upper_index;
                    if((double)(y + 1)/ ratio > ny - 1){
                        upper_index = ny - 1;
                    }
                    else {
                        double upper_loc = (double)(y+1) / ratio;
                        upper_index = upper_loc;
                    }

                    //cout << lower_index << ' ' << corresponding_old << ' ' << upper_index << endl;

                    for(int x = 0; x < new_nx; x++){
                        double lower_val = data[raster][new_nx*lower_index + x];
                        double upper_val = data[raster][new_nx*upper_index + x];
                        double slope = upper_val - lower_val; 
                        double delta = corresponding_old - lower_index;
                        double new_val = slope*delta + lower_val;
                        newer_image.data[raster][new_nx*y + x] = new_val;
                       //cout << lower_val << ' ' << new_image.data[raster][new_nx*y + x] << ' ' << upper_val << endl;
                    }
                }
            }
        }
        
        return newer_image;
    }


    return new_image;

}*/

//Skew Function
// Given: a real number of pixels to shift per row (assume integer)
// create a new image that's big enough
// shift each row into the new image
// Assume positive shift is right and negative shift is left
Image Image::skew(vector<int> &shift){
    //Find largest shift value to set image width
    int max = *(std::max_element(shift.begin(), shift.end()));
    int new_nx = nx + max;
    Image new_img;
    new_img.nx = new_nx;
    new_img.ny = ny;
    new_img.data = vector < vector <double> > (this->ChannelCount(), vector<double>(new_nx*ny, 0));
    
    for(int raster = 0; raster < this->ChannelCount(); raster++){
        for(int y = 0; y < ny; y++){
            int shift0 = shift[y];
            for(int x = 0; x < new_nx; x++){
                if(shift0 > 0){
                    new_img.data[raster][new_nx*y+x] = 0;
                    --shift0;
                }
                else if( x - shift[y] < nx){
                    new_img.data[raster][new_nx*y+x] = data[raster][nx*y + x - shift[y] ];
                }
                else {
                    new_img.data[raster][new_nx*y+x] = 0;
                }
            }
        }
    }

    return new_img;
}
