#pragma once

#include "Control.h"

namespace AeonUI
{
	class List : public Control
	{
	private:
		int scrollPosition;
	public:
		List();
		~List() {};
		void eventCall(Event *e);
		void add(Control *c):
		void next();
		void prev();
		void select();
		void draw();

		std::vector<Control *> items;
		int selectedIndex;
	};
}