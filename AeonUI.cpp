#include "AeonUI.h"

namespace AeonUI
{
	bool EventListner::listen() {
		bool result = false;
		if (PIND & _BV(7)) {
			if (this->deselected == true) {
				for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
					Event *e = this->events[EventTypeKeySelect].at(i);
					e->attachedControl->eventCall(EventTypeKeySelect);
				}
				this->deselected = false;
				result = true;
			}
		}
		else if (this->deselected == false){
			this->deselected = true;
			for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
				Event *e = this->events[EventTypeKeySelect].at(i);
				e->attachedControl->eventCall(EventTypeKeyDeselect);
			}
			result = true;
		}

		return result;
	}

	void Page::selectNextControl() {
		Control *c = this->hieralchy.at(this->selectedControl);
		// c->unhighlight();
		this->selectedControl++;
		c = this->hieralchy.at(this->selectedControl);
		// c->highlight();
	};
	void Page::selectPreviousControl() {
		Control *c = this->hieralchy.at(this->selectedControl);
		// c->unhighlight();
		this->selectedControl--;
		c = this->hieralchy.at(this->selectedControl);
		// c->highlight();
	};
	void Page::draw() {
		for (int i = 0; i < this->hieralchy.size(); i++) {
			Control *c = this->hieralchy.at(i);
			c->draw();
		}
		this->refresh = false;
	}
	void Page::add(Control *c) {
		c->identifier = this->globalIdentifierCounter++;
		c->context = this->context;
		this->listner.add(new Event(c, EventTypeKeySelect));
		this->hieralchy.push_back(c);
		this->refresh = true;
	}

	void Control::remove() {

	}

	void Button::draw() {
		// if (this->highlighted) {
		// }
		// else {
		// 	this->context->setDefaultBackgroundColor();
		// 	this->context->setDefaultForegroundColor();
		// }
		int x = this->origin.x;
		int y = this->origin.y;
		int width = this->size.x;
		int height = this->size.y;

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

		// inner content
		if (this->selected == true) {
			this->context->setDefaultBackgroundColor();
		}
		this->context->drawBox(x + 3, y + 3, width - 6, height - 6);

		this->context->setDefaultForegroundColor();
	}
};
