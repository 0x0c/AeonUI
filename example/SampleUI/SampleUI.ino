#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <string>
#include <vector>
#include <iterator>
#include "U8glib.h"
#include <AeonFoundation.h>
#include <AeonUI.h>

using namespace AeonUI;

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

int global_id_counter = 0;
bool need_to_draw = true;

AeonUI::Page page;

void draw()
{
	page.draw();
}

void setup(void)
{
	// flip screen, if required
	// u8g.setRot90();
	// u8g.setRot180();
	u8g.setFont(u8g_font_unifont);
	u8g.setCursorFont(u8g_font_cursor);
	Serial.begin(9600);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	// set REST HIGH
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	
	page.context = &u8g;

	List *list = new List();
	page.listner.add(new Event(list, EventTypeKeyLeft));
	page.listner.add(new Event(list, EventTypeKeyRight));
	page.add(list);

	Button *c = new Button(Point(0, 0), Point(21, 21));
	page.listner.add(new Event(c, EventTypeKeySelect));
	c->roundRect = true;
	list->add(c);
	// page.add(c);
	Button *c2 = new Button(Point(0, 0), Point(21, 21));
	page.listner.add(new Event(c2, EventTypeKeySelect));
	c2->roundRect = true;
	list->add(c2);
	// page.add(c2);
	Button *c3 = new Button(Point(0, 0), Point(21, 21));
	page.listner.add(new Event(c3, EventTypeKeySelect));
	c3->roundRect = true;
	list->add(c3);
	// page.add(c3);
	// Button *c4 = new Button(Point(21, 0), Point(21, 21));
	// page.listner.add(new Event(c4, EventTypeKeySelect));
	// c4->roundRect = true;
	// c4->text = "4";
	// list->add(c4);

	// Switch *s = new Switch(Point(21, 0), Point(15, 15));
	// Switch *s = new Switch(Point(21, 0), Point(32, 32));
	// page.listner.add(new Event(s, EventTypeKeySelect));
	// s->roundRect = true;
	// list->add(s);

	Image *i = new Image(Point(1, 3), Point(3, 28));
	list->add(i);

	Label *l = new Label(Point(3, 30), "Hello");
	// list->add(l);
	page.add(l);
}

void loop(void)
{
	if (page.needToRefresh()) {
		u8g.firstPage();
		do {
			draw();
		} while(u8g.nextPage());
	}
}
