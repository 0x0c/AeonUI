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
		EventTypeKeySelect,
		EventTypeKeyDeselect,
		EventTypeKeyUp,
		EventTypeKeyDown,
		EventTypeKeyLeft,
		EventTypeKeyRight,
		EventTypeKeyHome
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
		std::vector<Event *>events[8];

		bool deselected;
	public:
		EventListner() {
			this->deselected = true;
		};
		~EventListner() {
		};
		bool listen();
		void add(Event *e) {
			Serial.println("Add event");
			this->events[EventTypeKeySelect].push_back(e);
		}
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
	
	class Image : public Control
	{
	public:
		uint8_t *image;
		void setContext(uint8_t *image) {
			this->image = image;
		}
		void draw() {
			Serial.println("draw image");
			this->context->drawBitmapP(origin.x, origin.y, size.x, size.y, this->image);
		}
	};
	
	class Button : public Control
	{
	public:
		bool roundRect;
		bool selected;
		Button(Point origin, Point size) {
			this->origin = origin;
			this->size = size;
			this->selected = false;
		}
		Button() {
			
		}
		~Button() {
			
		}
		virtual void eventCall(EventType type) {
			switch (type) {
				case EventTypeKeySelect:
					this->select();
					break;
				case EventTypeKeyDeselect:
					this->deselect();
					break;
				default:
				break;
			}
		}
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
		virtual void on() {
			this->selected = true;
		}
		virtual void off() {
			this->selected = true;
		}
		void draw() {
			
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
			this->selectedIndex = 0;
		}
		virtual void eventCall(EventType type) {
			switch (type) {
				case EventTypeKeySelect:
					this->next();
					break;
				case EventTypeKeyDeselect:
					this->prev();
					break;
				default:
				break;
			}
		}
		void add(Control *c) {
			c->context = this->context;
			this->items.push_back(c);
			this->origin = Point(0, 0);
			this->size = Point(128, 32);
		}
		void next() {
			this->selectedIndex++;
			this->needToDraw();
		}
		void prev() {
			this->selectedIndex--;
			this->needToDraw();
		}
		void draw() {
			this->context->drawFrame(this->origin.x, this->origin.y, this->size.x, this->size.y);
			uint8_t height = 32;
			uint8_t width = 32;
			int count = 3 > this->items.size() ? this->items.size() : 3;
			int index = 0;
			for(int i = selectedIndex; i < count; i++) {
				// draw frame
				this->context->setDefaultForegroundColor();
				this->context->drawFrame(i * width + 1, 0, width, height);

				// draw all menu items
				Control *child = this->items.at(index);
				if (i == this->selectedIndex) {
					child->highlight();
					// current selected menu item
					// draw cursor bar
					this->context->drawBox(i * width + 1, 0, width, height);
					this->context->setDefaultBackgroundColor();
				}
				else {
					child->unhighlight();
				}

				// draw child
				child->origin = Point(i * width + 1, 0);
				child->draw();
				index++;
			}
		}
	};
};