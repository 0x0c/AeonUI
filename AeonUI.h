#pragma once

#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include "U8glib.h"
#include <Arduino.h>

namespace AeonUI
{
	class Util
	{
	public:
		static std::vector<std::string> split(const std::string &str, char delim)
		{
			std::vector<std::string> res;
			size_t current = 0, found;
			while((found = str.find_first_of(delim, current)) != std::string::npos){
				res.push_back(std::string(str, current, found - current));
				current = found + 1;
			}
			res.push_back(std::string(str, current, str.size() - current));

			return res;
		}
	};
	// prototype
	class Control;
	class Notification;

	class Point
	{
	public:
		Point() {
			this->x = 0;
			this->y = 0;
		};
		Point(int x, int y) {
			this->x = x;
			this->y = y;
		};
		~Point(){
		};
		
		/* data */
		int x;
		int y;
	};

	typedef enum {
		EventTypeKeySelect = 0,
		EventTypeKeyDeselect = 1,
		EventTypeKeyUp = 2,
		EventTypeKeyDown = 3,
		EventTypeKeyLeft = 4,
		EventTypeKeyRight = 5,
		EventTypeKeyHome = 6
	} EventType;
	class Event
	{
	public:
		Control *attachedControl;
		EventType type;
		void (*f)(EventType type, Control *c);
		Event() {
		}
		Event(Control *c, EventType type, void (*f)(EventType type, Control *c)) {
			this->attachedControl = c;
			this->type = type;
			this->f = f;
		};
		~Event() {
		};
	};

	class EventListner
	{
	private:
		std::vector<Event *>events[7];

		bool selected;
		bool lefted;
	public:
		EventListner() {
		};
		~EventListner() {
		};
		bool listen();
		void add(Event *e);
	};

	class Page
	{
	private:
		int globalIdentifierCounter;
		std::vector<Control *> hieralchy;
	public:
		bool showNotification;
		int selectedControl;
		Point origin;
		Point size;
		bool refresh;
		EventListner listner;
		U8GLIB *context;
		Notification *notification;

		Page() {
			this->refresh = true;
		};
		~Page() {
		};
		void selectControl();
		void moveToNextControl();
		void moveToPreviousControl();
		bool needToRefresh() {
			return refresh || listner.listen();
		};
		void add(Control *c);
		void remove();
		void draw();
		void postNotification(Notification *n);
		void revokeNotification() {
			this->showNotification = false;
		}
	};

	class Control
	{
	public:
		/* data */
		int identifier;
		bool selected;
		bool highlighted;
		Point origin;
		Point size;
		Page *parentPage;
		std::vector<Control *> items;
		U8GLIB *context;
		Control() {
			this->origin = Point(0, 0);
			this->size = Point(0, 0);
		}
		Control(Point origin, Point size) {
			this->origin = origin;
			this->size = size;
		}
		~Control() {
		}
		virtual void eventCall(Event *e) {
			if (e->f != NULL) {
				e->f(e->type, this);
			}
		}
		void needToDraw() {
			this->parentPage->refresh = true;
		}
		virtual void highlight() {
			this->highlighted = true;
			this->needToDraw();
		}
		virtual void unhighlight() {
			this->highlighted = false;
			this->needToDraw();
		}
		virtual void select() {
		}
		virtual void draw() {
		}
		void remove();
	};
	
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
	
	class Switch : public Button
	{
	public:
		Switch(Point origin, Point size) : Button (origin, size){
		}
		~Switch() {

		}
		virtual void eventCall(Event *e) {
			if (this->highlighted) {
				switch (e->type) {
					case EventTypeKeySelect: {
						this->select();
					}
						break;
					default:
					break;
				}
				Control::eventCall(e);
			}
		}
		virtual void select() {
			this->selected = !this->selected;
			this->needToDraw();
		}
		virtual void deselect() {}
		void draw();
	};
	
	class Label : public Control
	{
	public:
		std::string text;
		Point position;
		Label(Point position, std::string text) {
			this->text = text;
			this->position = position;
		}
		~Label() {
		}
		void draw() {
			this->context->setDefaultForegroundColor();
		    std::vector<std::string> result = Util::split(this->text, '\n');
		    for (int i = 0; i < result.size(); i++) {
				std::string s = result.at(i);
				this->context->drawStr(this->position.x, this->position.y + (i * this->context->getFontLineSpacing()), s.c_str());
			}
		}
	};
	
	class Notification : public Control
	{
		std::string title;
		std::string body;
	public:
		Notification(std::string title, std::string body) {
			this->title = title;
			this->body = body;
		}
		~Notification() {
		}
		void draw() {
			this->context->drawStr(0, 0, this->title.c_str());
			std::vector<std::string> result = Util::split(this->body, '\n');
			this->context->drawLine(0, this->context->getFontLineSpacing(), 128, this->context->getFontLineSpacing());
			for (int i = 0; i < result.size(); i++) {
				std::string s = result.at(i);
				this->context->drawStr(0, this->context->getFontLineSpacing() + 1 + (i * this->context->getFontLineSpacing()), s.c_str());
			}
		}
	};

	class List : public Control
	{
	private:
		int scrollPosition;
	public:
		std::vector<Control *> items;
		int selectedIndex;
		List() {
			this->origin = Point(0, 0);
			this->size = Point(128, 32);
			this->selectedIndex = 0;
		}
		void eventCall(Event *e);
		void add(Control *c) {
			c->context = this->context;
			int width = 32;
			c->origin = Point((this->items.size() * width) + (width - c->size.x) / 2, (width - c->size.y) / 2);
			this->items.push_back(c);
		}
		void next() {
			this->selectedIndex = (this->selectedIndex + 1) % this->items.size();
			this->needToDraw();
		}
		void prev() {
			this->selectedIndex = (this->selectedIndex - 1) % this->items.size();
			this->needToDraw();
		}
		void select() {
			Control *child = this->items.at(this->selectedIndex);
			this->select();
		}
		void draw();
	};

	class Image : public Control
	{
	public:
		uint8_t *image;
		Image(Point origin, Point size) : Control(origin, size){
		}
		~Image() {
		}

		void draw() {
			if (this->highlighted) {
				this->context->setDefaultBackgroundColor();
			}
			else {
				this->context->setDefaultForegroundColor();
			}

			this->context->drawXBMP(this->origin.x, this->origin.y, this->size.x, this->size.y, this->image);
		}
	};
};