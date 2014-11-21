#pragma once

#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <functional>
#ifdef DEBUG
#include <iostream>
#else
#include "U8glib.h"
#include <Arduino.h>
#endif

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
		Event(Control *c, EventType type) {
			this->attachedControl = c;
			this->type = type;
		};
		~Event();
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
		bool highlighted;
		Point origin;
		Point size;
		Page *parentPage;
		std::vector<Control *> items;
		U8GLIB *context;

		Control() {
		}
		~Control() {
		}
		virtual void eventCall(EventType type) {
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
	protected:
		bool selected;
	public:
		bool roundRect;
		std::string text;
		Button(Point origin, Point size) {
			this->origin = origin;
			this->size = size;
			this->selected = false;
		}
		Button() {
			
		}
		~Button() {
			
		}
		virtual void eventCall(EventType type);
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
		virtual void eventCall(EventType type) {
			if (this->highlighted) {
				switch (type) {
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
			}
		}
		virtual void select() {
		}
		virtual void deselect() {
		}
		virtual void on() {
			Serial.println("on");
			this->text = "v";
			this->selected = true;
			this->needToDraw();
		}
		virtual void off() {
			Serial.println("off");
			this->text = "";
			this->selected = false;
			this->needToDraw();
		}
		void draw() {
			if (this->highlighted) {
				this->context->setDefaultBackgroundColor();
			}
			else {
				this->context->setDefaultForegroundColor();
			}
			int x = this->origin.x;
			int y = this->origin.y;
			int width = this->size.x;
			int height = this->size.y;

			// border line
			if (this->roundRect) {
				this->context->drawRFrame(x, y, width, height, 4);
			}
			else {
				this->context->drawFrame(x, y, width, height);
			}
			// inner border line
			if (this->roundRect) {
				this->context->drawRFrame(x + 2, y + 2, width - 4, height - 4, 4);
			}
			else {
				this->context->drawFrame(x + 2, y + 2, width - 4, height - 4);
			}
			
			this->context->drawBox(x + 3, y + 3, width - 6, height - 6);
			if (this->highlighted) {
				this->context->setDefaultForegroundColor();
				this->context->drawCircle(x + width / 2, y + height / 2, 6);
			}
			else {
				this->context->setDefaultBackgroundColor();
				this->context->drawDisc(x + width / 2, y + height / 2, 5);
				// this->context->drawCircle(x + width / 2, y + height / 2, 4);
			}
			// inner content
			if (this->selected) {
				this->context->setDefaultForegroundColor();
				this->context->drawDisc(x + width / 2, y + height / 2, 4);
			}
		}
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
		void eventCall(EventType type);
		void add(Control *c) {
			c->context = this->context;
			int width = 32;
			c->origin = Point(this->items.size() * width + 1, 0);
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
};