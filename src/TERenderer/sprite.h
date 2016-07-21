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

namespace TE
{
	namespace Renderer
	{
		class sprite : public Core::component
		{
		public:
			sprite(Core::texture2D& Texture);
			sprite(const std::string FilePath);
			~sprite();

			void Tick(float DeltaTime) override {};
			void Render() override;

		private:
			Core::texture2D& _Texture;
			unsigned int _Quad;
		};
	}
}

#endif

