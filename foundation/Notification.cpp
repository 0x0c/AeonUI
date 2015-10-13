#include "Notification.h"

namespace AeonUI
{
	Notification::Notification(std::string title, std::string body) {
		this->title = title;
		this->body = body;
	}

	void Notification::draw() {
		this->context->drawStr(0, 0, this->title.c_str());
		std::vector<std::string> result = Util::split(this->body, '\n');
		this->context->drawLine(0, this->context->getFontLineSpacing(), 128, this->context->getFontLineSpacing());
		for (int i = 0; i < result.size(); i++) {
			std::string s = result.at(i);
			this->context->drawStr(0, this->context->getFontLineSpacing() + 1 + (i * this->context->getFontLineSpacing()), s.c_str());
		}
	}
}