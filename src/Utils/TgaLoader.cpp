#include "include/Utils/TgaLoader.h"
#include "include/Utils/Utils.h"

#include "include/TextureManager/SalmonTexture.h"
//Thanks to NeHe

namespace SE
{


	
 unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
 unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header




//bool LoadTGA(Texture * texture, char * filename)				// Load a TGA file
bool LoadTGA(const std::string& filename, TTextureData& texture)				// Load a TGA file
{
	
	size_t byteCount;

	boost::shared_array<char> fTGA = CreateMemFromFile<char>(filename, byteCount);

	TGAHeader tgaheader;

	tgaheader = *(reinterpret_cast<TGAHeader*>(&fTGA[0]));

	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		return LoadUncompressedTGA(texture, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		return LoadCompressedTGA(texture, fTGA);							// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
		return false;																// Exit function
	}
														// All went well, continue on
}


//bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
bool LoadUncompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
{																			// TGA Loading code nehe.gamedev.net)
	TGA tga;
	
	int shift = sizeof(TGAHeader);

	memcpy(tga.header, &fTGA[shift], CONST_TGA_INNER_HEADER_SIZE);

	shift += CONST_TGA_INNER_HEADER_SIZE;

	
	texture.Width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture.Height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	
	int bitsPerPixel = tga.header[4];
	
	if (bitsPerPixel == 24)
	{
		strcpy(texture.Format, "bmp24");
	}
	else if (bitsPerPixel == 32)
	{
		strcpy(texture.Format, "bmp32");
	}
	else
	{
		return false;
	}

	//texture->bpp	= tga.header[4];										// Determine the bits per pixel
	
	
	tga.Width = texture.Width;										// Copy width into local structure						
	tga.Height = texture.Height;										// Copy height into local structure
	tga.Bpp = bitsPerPixel;											// Copy BPP into local structure

	if((texture.Width <= 0) || (texture.Height <= 0))	// Make sure all information is valid
	{
		return false;														// Return failed
	}


	tga.bytesPerPixel = (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	
	texture.DataSize = tga.imageSize;
	texture.Data = boost::shared_array<char>(new char [texture.DataSize]);

	memcpy(&texture.Data[0], &fTGA[shift], texture.DataSize);


	// Byte Swapping Optimized By Steve Thomas
	for(unsigned int cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		texture.Data[cswap] ^= texture.Data[cswap+2] ^=
		texture.Data[cswap] ^= texture.Data[cswap+2];
	}

	return true;
}





//bool LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA)		// Load COMPRESSED TGAs
bool LoadCompressedTGA(TTextureData& texture, boost::shared_array<char> fTGA)		// Load COMPRESSED TGAs
{ 

	TGA tga;
	
	int shift = sizeof(TGAHeader);

	memcpy(tga.header, &fTGA[shift], CONST_TGA_INNER_HEADER_SIZE);

	shift += CONST_TGA_INNER_HEADER_SIZE;


	
	texture.Width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture.Height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	
	int bitsPerPixel = tga.header[4];
	
	if (bitsPerPixel == 24)
	{
		strcpy(texture.Format, "bmp24");
	}
	else if (bitsPerPixel == 32)
	{
		strcpy(texture.Format, "bmp32");
	}
	else
	{
		return false;
	}


	tga.Width = texture.Width;										// Copy width into local structure						
	tga.Height = texture.Height;										// Copy height into local structure
	tga.Bpp = bitsPerPixel;											// Copy BPP into local structure




	if((texture.Width <= 0) || (texture.Height <= 0))	//Make sure all texture info is ok
	{
		return false;														// Return failed
	}

	tga.bytesPerPixel = (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	
	
	texture.DataSize = tga.imageSize;
	texture.Data = boost::shared_array<char>(new char [texture.DataSize]);

	unsigned int pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	unsigned int currentpixel	= 0;												// Current pixel being read
	unsigned int currentbyte	= 0;												// Current byte 
	
	//GLubyte* colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	boost::shared_array<unsigned char> colorbuffer(new unsigned char[tga.bytesPerPixel]);

	do
	{
		unsigned char chunkheader = 0;											// Storage for "chunk" header


		chunkheader = static_cast<unsigned char>(fTGA[shift]);
		shift += sizeof(unsigned char);

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{

				memcpy(&colorbuffer[0], &fTGA[shift], tga.bytesPerPixel);
				shift += tga.bytesPerPixel;

				texture.Data[currentbyte] = static_cast<char>(colorbuffer[2]);
				texture.Data[currentbyte + 1] = static_cast<char>(colorbuffer[1]);
				texture.Data[currentbyte + 2] = static_cast<char>(colorbuffer[0]);

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					texture.Data[currentbyte + 3] = static_cast<char>(colorbuffer[3]);
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
					return false;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;	// Subteact 127 to get rid of the ID bit


			memcpy(&colorbuffer[0], &fTGA[shift], tga.bytesPerPixel);
			shift += tga.bytesPerPixel;


			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				
				texture.Data[currentbyte] = static_cast<char>(colorbuffer[2]);
				texture.Data[currentbyte + 1] = static_cast<char>(colorbuffer[1]);
				texture.Data[currentbyte + 2] = static_cast<char>(colorbuffer[0]);

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					texture.Data[currentbyte + 3] = static_cast<char>(colorbuffer[3]);
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
					return false;														// Return failed
				}
			}
		}

	} while(currentpixel < pixelcount);													// Loop while there are still pixels left
	
	return true;																		// return success
}



} //namespace SE