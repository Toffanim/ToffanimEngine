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
#include <GL/glew.h>
#include "stb_image.h"

namespace TE
{
	namespace Core
	{
		static void CreateTexture(GLuint Texture, 
			int SizedInternalType, 
			int BaseInternalFormat, 
			int ComponentType, 
			int Width, 
			int Height,
			unsigned char* RawData)
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

		class texture2D
		{
		public:
			texture2D( std::string Path, std::string Name = "default" );
			~texture2D();

			void GenerateMipmap();
			void Bind();

		private:
			std::string _Name;
			std::string _Path;
			GLuint _ID;
			int _Width;
			int _Height;
			unsigned char * _RawData;
		};
	}
}
#endif

