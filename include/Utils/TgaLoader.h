#ifndef TGA_LOADER_H_INCLUDED
#define TGA_LOADER_H_INCLUDED

#include "include/Utils/Utils.h"

//Thanks to NeHe

namespace SE
{


const int CONST_TGA_OUTER_HEADER_SIZE = 12;

typedef struct
{
	GLubyte Header[CONST_TGA_OUTER_HEADER_SIZE];		// TGA File Header
} TGAHeader;

const int CONST_TGA_INNER_HEADER_SIZE = 6;

typedef struct
{
	GLubyte		header[CONST_TGA_INNER_HEADER_SIZE];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


//TGAHeader tgaheader;									// TGA header
//TGA tga;												// TGA image data



extern GLubyte uTGAcompare[12];	// Uncompressed TGA Header
extern GLubyte cTGAcompare[12];	// Compressed TGA Header


struct TTextureData; // Find it in SalmonTexture.h 


bool LoadTGA(const std::string& filename, TTextureData& texture);

bool LoadUncompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA);	// Load an Uncompressed file
bool LoadCompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA);		// Load a Compressed file


} //namespace SE


#endif //TGA_LOADER_H_INCLUDED