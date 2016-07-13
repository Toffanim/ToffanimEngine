#if !defined(FBO_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define FBO_H
#include "texture.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

namespace TE
{
	namespace Core
	{
		class fbo
		{


		public:

			static void BindDefaultFBO()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			static void ClearDefaultFBO()
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			enum attachment : unsigned int
			{
				COLOR0 = GL_COLOR_ATTACHMENT0,
				COLOR1 = GL_COLOR_ATTACHMENT0 + 1,
				COLOR2 = GL_COLOR_ATTACHMENT0 + 2,
				COLOR3 = GL_COLOR_ATTACHMENT0 + 3,
				COLOR4 = GL_COLOR_ATTACHMENT0 + 4,
				DEPTH = GL_DEPTH_ATTACHMENT
			};

			enum clear_type
			{
				CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
				CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
				CLEAR_STENCIL = GL_STENCIL_BUFFER_BIT,
				CLEAR_ALL = (CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL),
			};

			fbo(unsigned int Width, unsigned int Height);
			~fbo();

			void AddDrawBuffer(std::string Name, attachment Attachment, texture2D::base_internal_format BaseInternalFormat,
				texture2D::sized_internal_format SizedInternalFormat,
				texture2D::data_type DataType);

			void fbo::AddDepthBuffer(texture2D::sized_internal_format SizedInternalFormat);

			void Bind();

			void Clear(clear_type Type = CLEAR_ALL);

			//void BindTexture(attachment Attachment);
			void BindTexture(std::string Name);

		private:
			std::map < std::string, std::unique_ptr<texture2D> > _ColorAttachments;
			std::unique_ptr<texture2D> _DepthAttachment;
			std::vector< unsigned int > _DrawBuffers;
			unsigned int _DrawBuffersSize;
			unsigned int _ColorAttachmentsSize;
			unsigned int _Width;
			unsigned int _Height;
			GLuint _ID;

			bool _CheckCompletness;
		};
	}
}
#endif