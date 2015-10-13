#include "Label.h"

namespace AeonUI
{
	Label::Label(std::string text) {
		this->text = text;
	}

	void Label::draw() {
		this->context->setDefaultForegroundColor();
		std::vector<std::string> result = Util::split(this->text, '\n');
		for (int i = 0; i < result.size(); i++) {
			std::string s = result.at(i);
			this->context->drawStr(this->origin.x, this->origin.y + (i * this->context->getFontLineSpacing()), s.c_str());
		}
	}
}