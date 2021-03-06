// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\texture.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
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
			_RawData = LoadImage(_Path.c_str(), &_Width, &_Height, &comp, 3);
			if (_RawData)
			{
				glGenTextures(1, &_ID);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _ID);
				//IMPORTANT(Marc) : Ajout du param pour choisir le format interne
				CreateTexture(_ID, _SizedInternalFormat, _BaseInternalFormat, _DataType, _Width, _Height, _RawData.get());
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

