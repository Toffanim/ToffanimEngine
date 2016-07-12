#include "fbo.h"

namespace TE
{
	namespace Core
	{
		fbo::fbo(unsigned int Width, unsigned int Height) :
			_Width(Width), _Height(Height)
		{
			glGenFramebuffers(1, &_ID);
		}

		fbo::~fbo()
		{}

		void fbo::AddDrawBuffer(attachment Attachment, texture2D::base_internal_format BaseInternalFormat,
			texture2D::sized_internal_format SizedInternalFormat,
			texture2D::data_type DataType)
		{
			_ColorAttachments.push_back(texture2D(_Width, _Height, BaseInternalFormat, SizedInternalFormat, DataType));
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);
			glDrawBuffer(Attachment);
			glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, _ColorAttachments.back().GetID(), 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void fbo::AddDepthBuffer(texture2D::sized_internal_format SizedInternalFormat)
		{
			_DepthAttachment = std::make_unique<texture2D>(_Width, _Height, texture2D::DEPTH_COMPONENT, SizedInternalFormat, texture2D::FLOAT);
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, DEPTH, GL_TEXTURE_2D, _DepthAttachment->GetID(), 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void fbo::Bind()
		{
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cout << "Error binding fbo" << std::endl;
				return;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);
		}
	}
}