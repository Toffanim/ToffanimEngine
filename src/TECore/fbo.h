#if !defined(FBO_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define FBO_H
#include "core.h"
#include <stdlib.h>
#include <vector>
#include <memory>

namespace TE
{
	namespace Core
	{
		class fbo
		{


		public:
			enum attachment
			{
				COLOR0 = GL_COLOR_ATTACHMENT0,
				COLOR1 = GL_COLOR_ATTACHMENT0 + 1,
				COLOR2 = GL_COLOR_ATTACHMENT0 + 2,
				COLOR3 = GL_COLOR_ATTACHMENT0 + 3,
				COLOR4 = GL_COLOR_ATTACHMENT0 + 4,
				DEPTH = GL_DEPTH_COMPONENT
			};

			fbo(unsigned int Width, unsigned int Height);
			~fbo();

			void AddDrawBuffer(attachment Attachment, texture2D::base_internal_format BaseInternalFormat,
				texture2D::sized_internal_format SizedInternalFormat,
				texture2D::data_type DataType);

			void fbo::AddDepthBuffer(texture2D::sized_internal_format SizedInternalFormat);

			void Bind();

		private:
			std::vector < texture2D > _ColorAttachments;
			std::unique_ptr<texture2D> _DepthAttachment;
			unsigned int _DrawBuffersSize;
			unsigned int _ColorAttachmentsSize;
			unsigned int _Width;
			unsigned int _Height;
			GLuint _ID;
		};
	}
}
#endif