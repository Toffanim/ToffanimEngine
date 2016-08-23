#if !defined(TEXTURE_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define TEXTURE_H

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <memory>

#include "utils.h"


namespace TE
{
	namespace Core
	{
		class texture2D
		{
		public:
			NON_COPYABLE_CLASS(texture2D);

			enum sized_internal_format
			{
				SRGB8 = GL_SRGB8,
				RGB8 = GL_RGB8,
				RGB16 = GL_RGB16,
				RGB16F = GL_RGB16F,
				RGB32F = GL_RGB32F,
				RGB8UI = GL_RGB8UI,
				RGB16UI = GL_RGB16UI,
				RGBA8 = GL_RGBA8,
				RGBA16 = GL_RGBA16,
				RGBA32F = GL_RGBA32F,
				DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
				DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
				DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32F,
			};

			enum data_type
			{
				UNSIGNED_INT = GL_UNSIGNED_INT,
				UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
				FLOAT = GL_FLOAT,
			};

			enum base_internal_format
			{
				SRGB = GL_SRGB,
				RGB = GL_RGB,
				RGBA = GL_RGBA,
				ALPHA = GL_ALPHA,
				LUMINANCE = GL_LUMINANCE,
				LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
				INTENSITY = GL_INTENSITY,
				DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
			};

			static void CreateTexture(GLuint Texture,
				sized_internal_format SizedInternalType,
				base_internal_format BaseInternalFormat,
				data_type ComponentType,
				int Width,
				int Height,
				unsigned char* RawData = 0)
			{
				glBindTexture(GL_TEXTURE_2D, Texture);
				glTexImage2D(GL_TEXTURE_2D, 0, SizedInternalType, Width, Height, 0, BaseInternalFormat, ComponentType, RawData);
				//IMPORTANT(Marc): Mettre le parametre pour changer le type de filtre suivant
				//Si la texture est mipmappée ou pas
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			texture2D(std::string Path, std::string Name = "default", base_internal_format BaseInternalFormat = base_internal_format::RGB,
				sized_internal_format SizedInternalFormat = sized_internal_format::RGB32F,
				data_type DataType = data_type::FLOAT );
			
			texture2D(unsigned int Width, unsigned int Height, base_internal_format BaseInternalFormat = base_internal_format::RGB,
				sized_internal_format SizedInternalFormat = sized_internal_format::RGB32F,
				data_type DataType = data_type::FLOAT);
			
			~texture2D();

			void GenerateMipmap();
			void Bind();
			GLuint GetID(){ return(_ID); }

		private:
			std::string _Name;
			std::string _Path;
			GLuint _ID;
			int _Width;
			int _Height;
			base_internal_format _BaseInternalFormat;
			sized_internal_format _SizedInternalFormat;
			data_type _DataType;
			std::unique_ptr<unsigned char> _RawData;
		};
	}
}
#endif

