#pragma once

#include "Control.h"

namespace AeonUI
{
	class Image : public Control
	{
	public:
		Image(Point origin, Point size) : Control(origin, size){}
		~Image() {}
		void draw();

		uint8_t *image;
	};
}