#include "Button.h"

namespace AeonUI
{
	void Button::eventCall(Event *e) {
		if (this->highlighted) {
			switch (e->type) {
				case EventTypeKeySelect:
					this->select();
					break;
				case EventTypeKeyDeselect:
					this->deselect();
					break;
				default:
				break;
			}
			Control::eventCall(e);
		}
	}
	
	void Button::draw() {
		int x = this->origin.x;
		int y = this->origin.y;
		int width = this->size.x;
		int height = this->size.y;

		this->context->setDefaultForegroundColor();
		if (this->highlighted) {
			this->context->drawBox(x, y, width, height);
			this->context->setDefaultBackgroundColor();
		}

		// border line
		if (this->roundRect) {
			this->context->drawRFrame(x, y, width, height, 4);
		}
		else {
			this->context->drawFrame(x, y, width, height);
		}

		if (this->selected) {
			this->context->setDefaultBackgroundColor();
			this->context->drawRBox(x, y, width, height, 4);
			this->context->setDefaultForegroundColor();
			this->context->drawRFrame(x + 1, y + 1, width - 2, height - 2, 4);
		}

		this->context->setDefaultBackgroundColor();
		if (this->selected) {
			this->context->setDefaultForegroundColor();
		}
		if (this->highlighted) {
			this->context->drawHLine(x + 4, y + height - 5, width - 8);
		}
		else {
			this->context->setDefaultForegroundColor();
		}
		this->context->drawStr(x + 4, y + height / 2, this->text.c_str());
	}
}