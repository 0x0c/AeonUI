#pragma once

#include "Point.h"
#include "U8glib.h"

namespace AeonUI
{
	class Control
	{
	protected:
		bool selected;
		bool highlighted;
		U8GLIB *context;
	public:
		Control();
		Control(Point origin, Point size);
		~Control() {}
		virtual void eventCall(Event *e);
		void needToDraw();
		virtual void highlight();
		virtual void unhighlight();
		virtual void select() {}
		virtual void draw() {}
		void remove();

		/* data */
		int identifier;
		Point origin;
		Point size;
		Page *parentPage;
		std::vector<Control *> items;
	};
}