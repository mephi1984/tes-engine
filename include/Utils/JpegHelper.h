#ifndef JPEG_HELPER_H_INCLUDED
#define JPEG_HELPER_H_INCLUDED

#include <string>

namespace SE
{

struct TTextureData; // Find it in SalmonTexture.h 

bool LoadJpg(const std::string& filename, TTextureData& texData);

} //namespace SE


#endif //JPEG_HELPER_H_INCLUDED