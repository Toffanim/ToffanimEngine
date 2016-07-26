#include "sprite.h"

namespace TE
{
	namespace Renderer
	{		
		sprite::sprite(Core::texture2D& Texture) : _Texture(Texture), _Quad(nullptr)
		{
		}

		sprite::sprite(const std::string FilePath)
			: _Texture(Core::texture2D(FilePath)),
			_Quad(nullptr)
		{
		}

		sprite::~sprite()
		{
		}

		void sprite::SetSize(Math::vec2f NewSize)
		{
			_Size = NewSize;
		}

		Math::vec2f sprite::GetSize() const
		{
			return (_Size);
		}

		void sprite::Render( Math::mat4f Projection, Math::mat4f View )
		{		
			if (_Owner)
			{
				Math::vec3f SpriteCenter = _Owner->GetPosition();
				std::vector<Core::vertex> Vertices;
				Vertices.reserve(4);
				Math::vec3f NewVertex;
				NewVertex = { SpriteCenter.x - _Size.x, SpriteCenter.y - _Size.y, SpriteCenter.z };
				Vertices.push_back({ NewVertex,
				{ 0.f, 0.f, 1.f },
				{ 0.f, 0.f } });
				NewVertex = { SpriteCenter.x + _Size.x, SpriteCenter.y - _Size.y, SpriteCenter.z };
				Vertices.push_back({ NewVertex,
				{ 0.f, 0.f, 1.f },
				{ 0.f, 0.f } });
				NewVertex = { SpriteCenter.x - _Size.x, SpriteCenter.y + _Size.y, SpriteCenter.z };
				Vertices.push_back({ NewVertex,
				{ 0.f, 0.f, 1.f },
				{ 0.f, 0.f } });
				NewVertex = { SpriteCenter.x + _Size.x, SpriteCenter.y + _Size.y, SpriteCenter.z };
				Vertices.push_back({ NewVertex,
				{ 0.f, 0.f, 1.f },
				{ 0.f, 0.f } });

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
				_Texture.Bind();
				_Quad->Render();
			}
		}
	}
}

