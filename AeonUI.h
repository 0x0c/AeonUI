#pragma once

#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <functional>
#include "U8glib.h"
#include <Arduino.h>

namespace AeonUI
{
	// prototype
	class Control;

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
		int selectedControl;
		Point origin;
		Point size;
		bool refresh;
		EventListner listner;
		U8GLIB *context;

		Page() {
			this->refresh = true;
		};
		~Page() {
		};
		void selectNextControl();
		void selectPreviousControl();
		bool needToRefresh() {
			return refresh || listner.listen();
		};
		void add(Control *c);
		void remove();
		void draw();
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
		virtual void draw() {
			Serial.println("draw conrtol");
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
						if (this->selected) {
							this->off();
						}
						else {
							this->on();
						}
					}
						break;
					default:
					break;
				}
				Control::eventCall(e);
			}
		}
		virtual void select() {}
		virtual void deselect() {}
		virtual void on() {
			this->selected = true;
			this->needToDraw();
		}
		virtual void off() {
			this->selected = false;
			this->needToDraw();
		}
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
			this->context->drawStr(this->position.x, this->position.y, this->text.c_str());
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