//================================================
// Image.cpp
//
// by Tejas Jha, Feb 1, 2018
// Adapted from HW03 written by Leland Pierce
//
// Implementation of the Image class.
//================================================
    
   
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdexcept>
	
#include "Image2.h"
#include "Exceptions.hpp"
 
using namespace std;


#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>

#include "H5Cpp.h"
using namespace H5;

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
    void Image2::filter(string filter_name, double param) 
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

//-----------------------------------------------------------------
    // read a single-channel image file
    // infile is the name of the intput image file
    // if the image is more than 1 channel, it just uses the first.
    // for now, it creates a float channel.
    // returns the newly-created Image
//-----------------------------------------------------------------

    void Image2::read_file(const std::string &infile){
    // 1. open the data file
    // 2. figure out data characteristics
    // 3. create empty raster
    // 4. fill raster.
    FileOpenErrorException FileOpenError;

#ifdef DEBUG
    cout << "read_file: at 1"<<endl;
#endif
    // 1. open the data file
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


    

    }// end: Image::read_file


//-----------------------------------------------------------------
// Write out the image data as a 1-channel jpeg.
//-----------------------------------------------------------------
    void Image2::write_file(const std::string &outfile){
    
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



      /* Once we're done, properly close the dataset */
      GDALClose( (GDALDatasetH) poDstDS );


    }// end: Image::write_file


/////////////////////////////////////////////////////////////
// Image: create_empty function
// Input Parameters:
//    1) nx (width of image)
//    2) ny (height of image)
// Overview: Generates an image with input size parameters
// with all 0 values. Returns an Image.
////////////////////////////////////////////////////////////

void Image2::create_empty(unsigned int inx, unsigned int iny, unsigned int channels){
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
// Image: set function
// Input Parameter: double value
// Overview: Sets all values in image to input value (all channels)
/////////////////////////////////////////////////////////////
void Image2::set(double val){
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
bool Image2::valid_slice(Slice &slc){
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
Slice Image2::slice(unsigned int inx0, unsigned int iny0, unsigned int indx, unsigned int indy){
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
void Image2::set_slice(Slice &slc, double val){
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
void Image2::recode(std::vector<int> &vec){
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

void Image2::recode_slice(Slice &slc, std::vector<int> &vec){
    //Ensure slice is of even length
    if(vec.size() & 2 != 0){
        std::cerr << "Inputted vector in recode function not of even length" << std::endl;
        throw std::bad_exception();
    }
    //Ensure that slice is valid for current image
    if(!Image2::valid_slice(slc)){
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

//////////////////////////////////////////////////////////////////////
// Image: open_hdf5
// Open HDF5 image
///////////////////////////////////////////////////////////////////////
void HDF5File::open_hdf5(){
    //H5File * file = 0;
    H5File *file = new H5File( "example_hdf5_file.h5", H5F_ACC_RDWR );
    //delete file;
    //H5File * file = new H5File(filename.c_str(), H5F_ACC_RDWR);
    //return file;
}

/*
//////////////////////////////////////////////////////////////////////
// Image: read_hdf5
// Read HDF5 image
///////////////////////////////////////////////////////////////////////
void Image::read_hdf5();

//////////////////////////////////////////////////////////////////////
// Image: read_hdf5_line
// Read HDF5 image line
///////////////////////////////////////////////////////////////////////
void Image::read_hdf5_line();

//////////////////////////////////////////////////////////////////////
// Image: write_hdf5_line
// Write HDF5 line
///////////////////////////////////////////////////////////////////////
void Image::write_hdf5_line();

//////////////////////////////////////////////////////////////////////
// Image: close_hdf5
// Close HDF5 image
///////////////////////////////////////////////////////////////////////
void Image::close_hdf5();

//////////////////////////////////////////////////////////////////////
// Image: thresh_hdf5
// Perform thresh on HDF5 image
///////////////////////////////////////////////////////////////////////
void Image::thresh_hdf5();

*/
