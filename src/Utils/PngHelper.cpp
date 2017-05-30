#include "include/Utils/PngHelper.h"
#include "include/Utils/Utils.h"

namespace SE
{

#ifdef TARGET_ANDROID
typedef PNG_CONST png_byte        FAR * png_const_bytep;
#endif

struct TPngReadStruct
{
	const boost::shared_array<char>& FileArr;
	size_t FileSize;
	int CurrentPointer;
	TPngReadStruct(const boost::shared_array<char>& fileArr, size_t fileSize)
		: FileArr(fileArr)
		, FileSize(fileSize)
		, CurrentPointer(8)
	{
	}

};


void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{


    png_voidp a = png_get_io_ptr(png_ptr);

	TPngReadStruct* pngReadStruct = reinterpret_cast<TPngReadStruct*>(a);


	if (pngReadStruct->CurrentPointer + length >= pngReadStruct->FileSize)
	{
		throw ErrorToLog("PNG pointer gone too far!");
	}

	for (size_t i = 0; i < length; i++)
	{
		data[i] = pngReadStruct->FileArr[pngReadStruct->CurrentPointer + i];
	}

	pngReadStruct->CurrentPointer += length;

}


TPngDataStruct read_png_file(const boost::shared_array<char>& fileArr, size_t fileSize)
{

	TPngDataStruct result;

	TPngReadStruct readStruct(fileArr, fileSize);


	char header[8];    // 8 is the maximum size that can be checked

	for (size_t i=0; i<8; i++)
	{
		header[i] = fileArr[i];
	}


	if (png_sig_cmp(reinterpret_cast<png_bytep>(header), 0, 8))
	{
		throw ErrorToLog("Incorrect PNG file (1)!");
	}

	/* initialize stuff */
	result.PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!result.PngPtr)
	{
		throw ErrorToLog("Incorrect PNG file (2)!");
	}

	result.InfoPtr = png_create_info_struct(result.PngPtr);

	if (!result.InfoPtr)
	{
		throw ErrorToLog("Incorrect PNG file (3)!");
	}

	if (setjmp(png_jmpbuf(result.PngPtr)))
	{
		throw ErrorToLog("Incorrect PNG file (4)!");
	}

	png_set_read_fn(result.PngPtr, (png_voidp)&readStruct, user_read_data);

	png_set_sig_bytes(result.PngPtr, 8);

	png_read_info(result.PngPtr, result.InfoPtr);

	result.Width = png_get_image_width(result.PngPtr, result.InfoPtr);
	result.Height = png_get_image_height(result.PngPtr, result.InfoPtr);
	result.ColorType = png_get_color_type(result.PngPtr, result.InfoPtr);
	result.BitDepth = png_get_bit_depth(result.PngPtr, result.InfoPtr);

	if (result.BitDepth != 8)
	{
		throw ErrorToLog("Incorrect bit depth in png file!");
	}

	if (result.ColorType != PNG_COLOR_TYPE_RGB && result.ColorType != PNG_COLOR_TYPE_RGBA)
	{
		throw ErrorToLog("Incorrect color type in png file!");
	}

	result.NumberOfPasses = png_set_interlace_handling(result.PngPtr);
	png_read_update_info(result.PngPtr, result.InfoPtr);


	if (setjmp(png_jmpbuf(result.PngPtr)))
	{
		throw ErrorToLog("Incorrect PNG file (5)!");
	}


	result.RowPointers = (png_bytep*) malloc(sizeof(png_bytep) * result.Height);
	for (int y=0; y<result.Height; y++)
		result.RowPointers[y] = (png_byte*) malloc(png_get_rowbytes(result.PngPtr, result.InfoPtr));

	png_read_image(result.PngPtr, result.RowPointers);

	return result;

}


} //namespace SE