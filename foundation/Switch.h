#pragma once

#include "Button.h"

namespace AeonUI
{
	class Switch : public Button
	{
	public:
		Switch(Point origin, Point size) : Button (origin, size){}
		~Switch() {}
		virtual void eventCall(Event *e);
		virtual void select();
		virtual void deselect() {}
		void draw();
	};
}