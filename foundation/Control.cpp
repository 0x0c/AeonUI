#include "Control.h"

namespace AeonUI
{
	Control::Control() {
		this->origin = Point(0, 0);
		this->size = Point(0, 0);
	}

	Control::Control(Point origin, Point size) {
		this->origin = origin;
		this->size = size;
	}

	virtual void Control::eventCall(Event *e) {
		if (e->f != NULL) {
			e->f(e->type, this);
		}
	}

	void Control::needToDraw() {
		this->parentPage->refresh = true;
	}
	
	virtual void Control::highlight() {
		this->highlighted = true;
		this->needToDraw();
	}
	
	virtual void Control::unhighlight() {
		this->highlighted = false;
		this->needToDraw();
	}
}