#ifndef PNG_HELPER_H_INCLUDED
#define PNG_HELPER_H_INCLUDED

extern "C"
{
#define PNG_DEBUG 3
#include "png.h"
}

#include "boost/shared_array.hpp"
#include "include/Utils/DataTypes/DataTypes.h"

namespace SE
{

struct TPngDataStruct
{
	int Width;
	int Height;
	png_byte ColorType;
	png_byte BitDepth;

	png_structp PngPtr;
	png_infop InfoPtr;
	int NumberOfPasses;
	png_bytep* RowPointers;
};



TPngDataStruct read_png_file(const boost::shared_array<char>& fileArr, cardinal fileSize);


} //namespace SE


#endif