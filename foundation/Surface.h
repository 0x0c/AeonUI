#pragma once

#include "Point.h"
#include "Event.h"
#include "Notification.h"
#include "U8glib.h"

namespace AeonUI
{
	class Surface
	{
	private:
		int globalIdentifierCounter;
		std::vector<Control *> hieralchy;
	public:
		Surface();
		~Surface() {};
		void selectControl();
		void moveToNextControl();
		void moveToPreviousControl();
		bool needToRefresh();
		void add(Control *c);
		void remove();
		void draw();
		void postNotification(Notification *n);
		void revokeNotification();

		bool showNotification;
		int selectedControl;
		Point origin;
		Point size;
		bool refresh;
		EventListner listner;
		U8GLIB *context;
		Notification *notification;
	};
}