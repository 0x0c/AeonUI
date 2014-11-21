#include "AeonUI.h"

namespace AeonUI
{
	void EventListner::add(Event *e) {
		this->events[e->type].push_back(e);
	}
	bool EventListner::listen() {
		bool result = false;
		if (PIND & _BV(7)) {
			if (this->selected == false) {
				for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
					Event *e = this->events[EventTypeKeySelect].at(i);
					e->attachedControl->eventCall(EventTypeKeySelect);
				}
				this->selected = true;
				result = true;
			}
		}
		else if (this->selected == true){
			this->selected = false;
			for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
				Event *e = this->events[EventTypeKeySelect].at(i);
				e->attachedControl->eventCall(EventTypeKeyDeselect);
			}
			result = true;
		}

		if (PIND & _BV(6)) {
			if (this->lefted == false) {
				for (int i = 0; i < this->events[EventTypeKeyLeft].size(); i++) {
					Event *e = this->events[EventTypeKeyLeft].at(i);
					e->attachedControl->eventCall(EventTypeKeyLeft);
				}
				this->lefted = true;
				result = true;
			}
		}
		else {
			this->lefted = false;
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
		this->hieralchy.push_back(c);
		this->refresh = true;
	}

	void Control::remove() {

	}

	void Button::eventCall(EventType type) {
		if (this->highlighted) {
			switch (type) {
				case EventTypeKeySelect:
					this->select();
					break;
				case EventTypeKeyDeselect:
					this->deselect();
					break;
				default:
				break;
			}
		}
	}
	void Button::draw() {
		if (this->highlighted) {
			this->context->setDefaultBackgroundColor();
		}
		else {
			this->context->setDefaultForegroundColor();
		}
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
		if (this->selected) {
			this->context->setDefaultBackgroundColor();
		}
		this->context->drawBox(x + 3, y + 3, width - 6, height - 6);
		if (this->highlighted) {
			this->context->setDefaultForegroundColor();
			this->context->drawCircle(x + width / 2, y + height / 2 - 1, 2);
		}
		else {
			this->context->setDefaultBackgroundColor();
		}
	}

	void List::draw() {
		this->context->drawFrame(this->origin.x, this->origin.y, this->size.x, this->size.y);
		uint8_t height = 32;
		uint8_t width = 32;
		int itemCount = this->items.size();
		int count = 3 > itemCount ? itemCount : 3;
		for(int i = 0; i < count; i++) {
			Control *child = this->items.at(i);
			if (i == this->selectedIndex) {
				this->context->setDefaultForegroundColor();
				this->context->drawBox(i * width + 1, 1, width, height);
				child->highlight();
			}
			else {
				this->context->setDefaultForegroundColor();
				this->context->drawFrame(i * width + 1, 0, width, height);
				this->context->setDefaultBackgroundColor();
				child->unhighlight();
			}
			child->draw();
		}
	}
};
