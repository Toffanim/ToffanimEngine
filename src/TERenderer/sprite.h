#if !defined(SPRITE_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define SPRITE_H

#include "TECore\component.h"
#include "TECore\texture.h"
#include "TERenderer\shader.h"
#include "TECore\frame_buffer.h"
#include "TECore\globals.h"
#include "TECore\math.h"
#include "TECore\vertex_array.h"
#include "TECore\actor.h"

namespace TE
{
	namespace Renderer
	{
		class sprite : public Core::component_CRTP<sprite>
		{
		public:
			sprite(Core::texture2D& Texture);
			sprite(const std::string FilePath);
			~sprite();

			void SetSize(Math::vec2f NewSize);
			Math::vec2f GetSize() const;
			void Tick(float DeltaTime) override {};
			void Render(Math::mat4f Projection, Math::mat4f View) override;
			void SetupAttachement(component* Parent) override;

		private:
			Core::texture2D* _Texture;
			Core::vertex_array* _Quad;
			Math::vec2f _Size;
		};
	}
}

#endif

