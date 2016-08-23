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