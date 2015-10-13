#pragma once

namespace AeonUI
{
	// prototype
	class Control;
	class Notification;

	typedef enum {
		EventTypeKeySelect 		= 0,
		EventTypeKeyDeselect 	= 1,
		EventTypeKeyUp 			= 2,
		EventTypeKeyDown 		= 3,
		EventTypeKeyLeft 		= 4,
		EventTypeKeyRight 		= 5,
		EventTypeKeyHome 		= 6
	} EventType;
	
	class Event
	{
	private:
		Control *attachedControl;
		void (*f)(EventType type, Control *c);

	protected:
		EventType type;

	public:
		Event() {};
		Event(Control *c, EventType type, void (*f)(EventType type, Control *c));
		~Event() {};
	};

	class EventListner
	{
	private:
		std::vector<Event *>events[7];

		bool selected;
		bool lefted;
	public:
		EventListner() {};
		~EventListner() {};

		bool listen();
		void add(Event *e);
	};
}
