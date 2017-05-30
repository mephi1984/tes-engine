#ifndef TGA_LOADER_H_INCLUDED
#define TGA_LOADER_H_INCLUDED

#include "include/Utils/Utils.h"

//Thanks to NeHe

namespace SE
{


const int CONST_TGA_OUTER_HEADER_SIZE = 12;

typedef struct
{
	unsigned char Header[CONST_TGA_OUTER_HEADER_SIZE];		// TGA File Header
} TGAHeader;

const int CONST_TGA_INNER_HEADER_SIZE = 6;

typedef struct
{
	unsigned char		header[CONST_TGA_INNER_HEADER_SIZE];								// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;	
	unsigned int		Height;									//Height of Image
	unsigned int		Width;									//Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
} TGA;


//TGAHeader tgaheader;									// TGA header
//TGA tga;												// TGA image data



extern unsigned char uTGAcompare[12];	// Uncompressed TGA Header
extern unsigned char cTGAcompare[12];	// Compressed TGA Header


struct TTextureData; // Find it in SalmonTexture.h 


bool LoadTGA(const std::string& filename, TTextureData& texture);

bool LoadUncompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA);	// Load an Uncompressed file
bool LoadCompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA);		// Load a Compressed file


} //namespace SE


#endif //TGA_LOADER_H_INCLUDED