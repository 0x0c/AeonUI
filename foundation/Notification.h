#pragma once

namespace AeonUI
{
	class Notification : public Control
	{
	protected:
		std::string title;
		std::string body;
	public:
		Notification(std::string title, std::string body);
		~Notification() {}
		void draw();
	};
}