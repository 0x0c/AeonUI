#include "Event.h"

namespace AeonUI
{
	Event::Event(Control *c, EventType type, void (*f)(EventType type, Control *c)) {
		this->attachedControl = c;
		this->type = type;
		this->f = f;
	}

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
			if (this->left == false) {
				for (int i = 0; i < this->events[EventTypeKeyLeft].size(); i++) {
					Event *e = this->events[EventTypeKeyLeft].at(i);
					e->attachedControl->eventCall(e);
				}
				this->left = true;
				result = true;
			}
		}
		else {
			this->left = false;
		}

		return result;
	}
}