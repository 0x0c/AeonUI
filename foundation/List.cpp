#include "List.h"

namespace AeonUI
{
	List::List() {
		this->origin = Point(0, 0);
		this->size = Point(128, 32);
		this->selectedIndex = 0;
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

	void List::add(Control *c) {
		c->context = this->context;
		int width = 32;
		c->origin = Point((this->items.size() * width) + (width - c->size.x) / 2, (width - c->size.y) / 2);
		this->items.push_back(c);
	}
	void List::next() {
		this->selectedIndex = (this->selectedIndex + 1) % this->items.size();
		this->needToDraw();
	}
	void List::prev() {
		this->selectedIndex = (this->selectedIndex - 1) % this->items.size();
		this->needToDraw();
	}
	void List::select() {
		Control *child = this->items.at(this->selectedIndex);
		this->select();
	}
}