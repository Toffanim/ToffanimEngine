// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\utils.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#define STB_IMAGE_IMPLEMENTATION
#include "utils.h"

namespace TE
{
	namespace Core
	{
		std::unique_ptr<unsigned char> LoadImage(const char* Path, int* Width, int* Height, int* Comp, unsigned int Channels)
		{
			auto RawDataPtr = stbi_load(Path, Width, Height, Comp, Channels);
			auto Size = (*Width) * (*Height) * Channels;
			auto Result = std::unique_ptr<unsigned char>(new unsigned char[Size]);
			memcpy(Result.get(), RawDataPtr, Size); //Copy to get a malloc for memory size
			stbi_image_free(RawDataPtr);
			return (Result);
		}
	}
}
