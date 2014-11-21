#include "AeonUI.h"

namespace AeonUI
{
	void EventListner::add(Event *e) {
		this->events[e->type == 1 ? 0 : e->type].push_back(e);
	}
	bool EventListner::listen() {
		bool result = false;
		if (PIND & _BV(7)) {
			if (this->selected == false) {
				for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
					Event *e = this->events[EventTypeKeySelect].at(i);
					e->type = EventTypeKeySelect;
					e->attachedControl->eventCall(e);
				}
				this->selected = true;
				result = true;
			}
		}
		else if (this->selected == true){
			this->selected = false;
			for (int i = 0; i < this->events[EventTypeKeySelect].size(); i++) {
				Event *e = this->events[EventTypeKeySelect].at(i);
				e->type = EventTypeKeyDeselect;
				e->attachedControl->eventCall(e);
			}
			result = true;
		}

		if (PIND & _BV(6)) {
			if (this->lefted == false) {
				for (int i = 0; i < this->events[EventTypeKeyLeft].size(); i++) {
					Event *e = this->events[EventTypeKeyLeft].at(i);
					e->attachedControl->eventCall(e);
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
		this->context->setDefaultForegroundColor();
		if (this->highlighted) {
			this->context->setDefaultBackgroundColor();
		}
		int x = this->origin.x;
		int y = this->origin.y;
		int width = this->size.x;
		int height = this->size.y;

		if (this->selected) {
			this->context->drawRBox(x, y, width, height, 4);
		}
		// border line
		if (this->roundRect) {
			this->context->drawRFrame(x, y, width, height, 4);
		}
		else {
			this->context->drawFrame(x, y, width, height);
		}

		this->context->setDefaultBackgroundColor();
		if (this->highlighted) {
			this->context->setDefaultForegroundColor();
		}

		this->context->drawBox(x + 3, y + 3, width - 6, height - 6);
		this->context->setDefaultForegroundColor();
		if (this->highlighted) {
			this->context->setDefaultBackgroundColor();
			this->context->drawHLine(x + 4, y + height - 5, width - 8);
		}
		this->context->drawStr(x + 4, y + height / 2, this->text.c_str());
	}

	void Switch::draw() {
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
		
		// this->context->drawBox(x + 3, y + 3, width - 6, height - 6);
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

	void List::eventCall(Event *e) {
		switch (e->type) {
			case EventTypeKeyRight: {
				this->prev();
			}
				break;
			case EventTypeKeyLeft: {
				this->next();
			}
				break;
			default:
			break;
		}
		Control::eventCall(e);
	}
	void List::draw() {
		this->context->drawFrame(this->origin.x, this->origin.y, this->size.x, this->size.y);
		uint8_t height = 32;
		uint8_t width = 32;
		int itemCount = this->items.size();
		int count = 4 > itemCount ? itemCount : 4;
		for(int i = 0; i < count; i++) {
			Control *child = this->items.at(i);
			if (i == this->selectedIndex) {
				this->context->setDefaultForegroundColor();
				this->context->drawBox(i * width, 0, width, height);
				child->highlight();
			}
			else {
				this->context->setDefaultForegroundColor();
				this->context->drawFrame(i * width, 0, width, height);
				this->context->setDefaultBackgroundColor();
				child->unhighlight();
			}
			child->draw();
		}
	}
};
