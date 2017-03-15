#include "include/Utils/JpegHelper.h"
#include "include/Utils/Utils.h"
#include "include/TextureManager/SalmonTexture.h"


#include "boost/gil/gil_all.hpp"
#include "boost/gil/extension/numeric/sampler.hpp"
#include "boost/gil/extension/numeric/resample.hpp"

#include "boost/assign.hpp"

#include "jpeglib.h"
#include <setjmp.h>

namespace SE
{

struct my_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_mgr* myerr = (my_error_mgr*) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
	
	throw ErrorToLog("JPEG Error!");
}



bool LoadJpg(const std::string& filename, TTextureData& texData)
{

  jpeg_decompress_struct cinfo;
  
  my_error_mgr jerr;

  JSAMPARRAY buffer;
  int row_stride;


  size_t fileSize;

  boost::shared_array<unsigned char> fileArr = CreateMemFromFile<unsigned char>(filename, fileSize);

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  jpeg_create_decompress(&cinfo);

  jpeg_mem_src(&cinfo, &fileArr[0], fileSize);


  jpeg_read_header(&cinfo, TRUE);
  
  jpeg_start_decompress(&cinfo);
  
  row_stride = cinfo.output_width * cinfo.output_components;
  
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


  if (cinfo.output_components == 3)
  {
	  strcpy(texData.Format, "bmp24");
  }
  else if (cinfo.output_components == 4)
  {
	  strcpy(texData.Format, "bmp32");
  }
  else
  {
	  throw ErrorToLog("cinfo.output_components unknown value!");
  }

  texData.Width = cinfo.output_width;

  texData.Height = cinfo.output_height;

  texData.DataSize = cinfo.output_height * cinfo.output_width * cinfo.output_components;
  texData.Data = boost::shared_array<char>(new char[texData.DataSize]);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		JDIMENSION read_now = jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(&texData.Data[(cinfo.output_scanline - read_now) * cinfo.output_width * cinfo.output_components], buffer[0], row_stride);
	}

  jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  using namespace boost::gil;
  
  boost::shared_array<char> newData(new char[texData.Height * texData.Width * 3]);

  rgb8_view_t oldView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgb8_ptr_t>(texData.Data.get()), texData.DataSize / texData.Height);
 
  rgb8_view_t newView = interleaved_view(texData.Width, texData.Height, reinterpret_cast<const rgb8_ptr_t>(newData.get()), texData.DataSize / texData.Height);

  copy_pixels(flipped_up_down_view(oldView), newView);
  
  texData.Data = newData;

  return true;

}

} //namespace SE
