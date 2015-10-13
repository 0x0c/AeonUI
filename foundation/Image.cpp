#include "Image.h"

namespace AeonUI
{
	void Image::draw() {
		if (this->highlighted) {
			this->context->setDefaultBackgroundColor();
		}
		else {
			this->context->setDefaultForegroundColor();
		}

		this->context->drawXBMP(this->origin.x, this->origin.y, this->size.x, this->size.y, this->image);
	}
}