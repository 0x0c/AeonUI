#pragma once

#include <string>
#include "Point.h"
#include "Control.h"

namespace AeonUI
{
	class Label : public Control
	{
	public:
		std::string text;
		Label(std::string text);
		~Label() {}
		void draw();
	};
}