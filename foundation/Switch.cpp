#include "Switch.h"

namespace AeonUI
{
	virtual void Switch::eventCall(Event *e) {
		if (this->highlighted) {
			switch (e->type) {
				case EventTypeKeySelect: {
					this->select();
				}
					break;
				default:
				break;
			}
			Control::eventCall(e);
		}
	}
	
	virtual void Switch::select() {
		this->selected = !this->selected;
		this->needToDraw();
	}

	void Switch::draw() {
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
		// inner border line
		if (this->roundRect) {
			this->context->drawRFrame(x + 2, y + 2, width - 4, height - 4, 4);
		}
		else {
			this->context->drawFrame(x + 2, y + 2, width - 4, height - 4);
		}
		
		if (this->highlighted) {
			this->context->setDefaultBackgroundColor();
		}
		this->context->drawCircle(x + width / 2, y + height / 2, 7);

		// inner content
		if (this->selected) {
			this->context->setDefaultForegroundColor();
			if (this->highlighted) {
				this->context->setDefaultBackgroundColor();
			}
			this->context->drawDisc(x + width / 2, y + height / 2, 5);
		}
	}
}