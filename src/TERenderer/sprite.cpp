// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: TERenderer\sprite.cpp $
   $Created: 01-01-2017 00h00m00s $
   $Modified: 06-02-2017 09h51m30s $
   $Revision: $
   $Creator : TOFFANIN Marc $
   $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "sprite.h"
#include "TECore\actor.h"

namespace TE
{
	namespace Renderer
	{		
		sprite::sprite(Core::texture2D& Texture) : _Texture(&Texture), _Quad(nullptr)
		{
		}

		sprite::sprite(const std::string FilePath)
			: _Texture( new Core::texture2D(FilePath)),
			_Quad(nullptr)
		{
		}

		sprite::~sprite()
		{
			delete _Quad;
		}

		void sprite::SetSize(Math::vec2f NewSize)
		{
			_Size = NewSize;
		}

		Math::vec2f sprite::GetSize() const
		{
			return (_Size);
		}

		void sprite::SetupAttachement(component* Parent)
		{
			component_CRTP<sprite>::SetupAttachement(Parent);
			Math::axis3 Axis = _Owner->GetAxis();
			Math::vec3f SpriteCenter = _Owner->GetPosition();
			TE::Math::bounding_box BB;
			BB.LowerBound = Math::vec3f(SpriteCenter - (Axis.Right*_Size.x) - (Axis.Up*_Size.y));
			BB.UpperBound = Math::vec3f(SpriteCenter + (Axis.Right*_Size.x) + (Axis.Up*_Size.y));
			Parent->GetOwner()->ExtendBoundingBox(BB);
		}

		void sprite::Render( Math::mat4f Projection, Math::mat4f View )
		{		
			if (_Owner)
			{
				Math::vec3f SpriteCenter = _Owner->GetPosition();
				Math::axis3 Axis = _Owner->GetAxis();
				std::vector<Core::vertex> Vertices;
				Vertices.reserve(4);
				Math::vec3f NewVertex;
				NewVertex = SpriteCenter - (Axis.Right*_Size.x) - (Axis.Up*_Size.y);
				Vertices.push_back({ NewVertex,
						{ 0.f, 0.f, 1.f },
						{ 0.f, 1.f } });
				NewVertex = SpriteCenter + (Axis.Right*_Size.x) - (Axis.Up*_Size.y);
				Vertices.push_back({ NewVertex,
						{ 0.f, 0.f, 1.f },
						{ 1.f, 1.f } });
				NewVertex = SpriteCenter - (Axis.Right*_Size.x) + (Axis.Up*_Size.y);
				Vertices.push_back({ NewVertex,
						{ 0.f, 0.f, 1.f },
						{ 0.f, 0.f } });
				NewVertex = SpriteCenter + (Axis.Right*_Size.x) + (Axis.Up*_Size.y);
				Vertices.push_back({ NewVertex,
						{ 0.f, 0.f, 1.f },
						{ 1.f, 0.f } });

				if (_Quad)
					_Quad->Update(Vertices);
				else
				{
					std::vector<unsigned int> Indices = { 0, 1, 2, 2, 1, 3 };
					_Quad = new Core::vertex_array(Vertices, Indices);
				}

				Math::mat4f MV = Projection * View;
				TE::GBufferFBO->Bind();
				TE::GBufferShader->Bind();
				TE::GBufferShader->SetMatrix4("MVP", MV);
				TE::GBufferShader->SetMatrix4("MV", MV);
				TE::GBufferShader->SetInt("ReverseNormals", 0);
				TE::GBufferShader->SetInt("Material.Texture", 0);
				TE::GBufferShader->SetInt("Material.HasTexture", 1);
				glActiveTexture(GL_TEXTURE0);
				_Texture->Bind();
				_Quad->Render();
			}
		}
	}
}

