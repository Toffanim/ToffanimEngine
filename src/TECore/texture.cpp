#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

namespace TE
{
	namespace Core
	{

		texture2D::texture2D(std::string Path, std::string Name, 
			base_internal_format BaseInternalFormat, 
			sized_internal_format SizedInternalFormat,
			data_type DataType) : 
			_Path(Path),
			_Name(Name),
			_DataType(DataType),
			_SizedInternalFormat(SizedInternalFormat),
			_BaseInternalFormat(BaseInternalFormat),
			_RawData(nullptr)
		{
			int comp;
			_RawData = stbi_load(_Path.c_str(), &_Width, &_Height, &comp, 3);
			if (_RawData)
			{
				glGenTextures(1, &_ID);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _ID);
				//IMPORTANT(Marc) : Ajout du param pour choisir le format interne
				CreateTexture(_ID, _SizedInternalFormat, _BaseInternalFormat, _DataType, _Width, _Height, _RawData);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				std::cerr << "Error loading texture file" << std::endl;
			}
		}

		texture2D::texture2D(unsigned int Width, unsigned int Height,
			base_internal_format BaseInternalFormat,
			sized_internal_format SizedInternalFormat,
			data_type DataType) :
			_Path(std::string()),
			_Name(std::string()),
			_DataType(DataType),
			_SizedInternalFormat(SizedInternalFormat),
			_BaseInternalFormat(BaseInternalFormat),
			_Width(Width),
			_Height(Height),
			_RawData(nullptr)
		{
			glGenTextures(1, &_ID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _ID);
			//IMPORTANT(Marc) : Ajout du param pour choisir le format interne
			CreateTexture(_ID, _SizedInternalFormat, _BaseInternalFormat, _DataType, _Width, _Height);
			glBindTexture(GL_TEXTURE_2D, 0);
		}


		texture2D::~texture2D()
		{
			delete[] _RawData;
			glDeleteTextures(1, &_ID);
		}

		void texture2D::GenerateMipmap()
		{
			glBindTexture(GL_TEXTURE_2D, _ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void texture2D::Bind()
		{
			glBindTexture(GL_TEXTURE_2D, _ID);
		}
	}
}

