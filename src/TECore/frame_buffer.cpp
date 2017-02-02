/* ========================================================================
    $File: TECore\frame_buffer.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "frame_buffer.h"

namespace TE
{
	namespace Core
	{
		frame_buffer::frame_buffer(unsigned int Width, unsigned int Height) :
			_Width(Width), _Height(Height), _CheckCompletness(true)
		{
			glGenFramebuffers(1, &_ID);
		}

		frame_buffer::~frame_buffer()
		{
			glDeleteFramebuffers(1, &_ID);
		}

		void frame_buffer::AddDrawBuffer(std::string Name, attachment Attachment, texture2D::base_internal_format BaseInternalFormat,
			texture2D::sized_internal_format SizedInternalFormat,
			texture2D::data_type DataType)
		{
			_ColorAttachments.insert({ Name, std::make_unique<texture2D>(_Width, _Height, BaseInternalFormat, SizedInternalFormat, DataType) });
			_DrawBuffers.push_back(Attachment);
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);

			std::vector<unsigned int>::const_pointer p = _DrawBuffers.data();
			glDrawBuffers(_DrawBuffers.size(), p);

			glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, _ColorAttachments[Name]->GetID(), 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_CheckCompletness = true;
		}

		void frame_buffer::AddDepthBuffer(texture2D::sized_internal_format SizedInternalFormat)
		{
			_DepthAttachment = std::make_unique<texture2D>(_Width, _Height, texture2D::DEPTH_COMPONENT, SizedInternalFormat, texture2D::FLOAT);
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, DEPTH, GL_TEXTURE_2D, _DepthAttachment->GetID(), 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			_CheckCompletness = true;
		}

		void frame_buffer::BindTexture(std::string Name)
		{
			auto Result = _ColorAttachments[Name].get();
			if (Result)
				Result->Bind();
			else
				std::cerr << "Trying to bind unknown FBO texture" << std::endl;
		}

		void frame_buffer::BindDepthTexture()
		{
			auto Result = _DepthAttachment.get();
			if (Result)
				Result->Bind();
			else
				std::cerr << "Trying to bind unknown FBO texture" << std::endl;
		}

		unsigned int frame_buffer::GetDepthTextureID()
		{
			auto Result = _DepthAttachment.get();
			if (Result)
				return (Result->GetID());
			else
				std::cerr << "Trying to bind unknown FBO texture" << std::endl;
			return 0;
		}

		void frame_buffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _ID);
			if (_CheckCompletness)
			{
				_CheckCompletness = false;
				auto Result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if ( Result != GL_FRAMEBUFFER_COMPLETE)
				{
					std::cerr << "Trying to bind incomplete FBO" << std::endl;
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					return;
				}
			}		
		}

		void frame_buffer::Clear(clear_type Type)
		{
			Bind();
			glClear(Type);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		};
	}
}