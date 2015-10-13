#pragma once

#include "Control.h"

namespace AeonUI
{
	class Button : public Control
	{
	public:
		bool roundRect;
		std::string text;
		Button(Point origin, Point size) : Control(origin, size){
			this->selected = false;
			this->highlighted = false;
		}
		Button() : Control() {
			this->selected = false;
		}
		~Button() {
			
		}
		virtual void eventCall(Event *e);
		virtual void select() {
			this->selected = true;
			this->needToDraw();
		}
		virtual void deselect() {
			this->selected = false;
			this->needToDraw();
		}
		void draw();
	};
}