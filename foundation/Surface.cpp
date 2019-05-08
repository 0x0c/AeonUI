#include "Surface.h"

namespace AeonUI
{
	Surface::Surface() {
		this->refresh = true;
	};
	void Surface::moveToNextControl() {
		Control *c = this->hieralchy.at(this->selectedControl);
		c->unhighlight();
		this->selectedControl = (this->selectedControl + 1) % this->hieralchy.size();;
		c = this->hieralchy.at(this->selectedControl);
		c->highlight();
		this->refresh = true;
	}

	void Surface::moveToPreviousControl() {
		Control *c = this->hieralchy.at(this->selectedControl);
		c->unhighlight();
		this->selectedControl = (this->selectedControl - 1) % this->hieralchy.size();
		c = this->hieralchy.at(this->selectedControl);
		c->highlight();
		this->refresh = true;
	}

	void Surface::draw() {
		if (notification == false) {
			for (int i = 0; i < this->hieralchy.size(); i++) {
				Control *c = this->hieralchy.at(i);
				c->draw();
			}
		}
		else {
			this->notification->draw();
		}
		
		this->context->setDefaultForegroundColor();
		this->refresh = false;
	}

	void Surface::add(Control *c) {
		c->identifier = this->globalIdentifierCounter++;
		c->context = this->context;
		this->hieralchy.push_back(c);
		this->refresh = true;
	}

	void Surface::postNotification(Notification *n) {
		this->notification = n;
		this->notification->context = this->context;
		this->showNotification = true;
	}

	Surface::bool needToRefresh() {
		return refresh || listener.listen();


	Surface::void revokeNotification() {
		this->showNotification = false;
	}
}