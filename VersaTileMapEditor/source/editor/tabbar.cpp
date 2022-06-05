#include <editor/tabbar.hpp>

#include <editor/closebutton.hpp>
#include <editor/tabwidget.hpp>

#include <QEvent>

namespace vt::editor {

TabBar::TabBar(QWidget* parent)
    : QTabBar{ parent }
{
	installEventFilter(this);
	setMovable(true);
}



TabBar::~TabBar() {
	removeEventFilter(this);
}



void TabBar::updateCloseButtons(QPoint mouse_pos) {	
	for (qint32 i = 0; i < count(); ++i) {
		auto close_button = static_cast<editor::CloseButton*>(tabButton(i, QTabBar::RightSide));
		if (close_button == nullptr)
			continue;
		
		if (i == currentIndex())
			close_button->show();
		else {
			if (static_cast<bool>(close_button->state & ButtonState::Pressed))
				close_button->show();
			else {
				if (tabRect(i).contains(mouse_pos) && visibleRegion().contains(mouse_pos))
					close_button->show();
				else
					close_button->hide();
			}
		}
	}
}



void TabBar::setCloseButton(qint32 index) {
	QTabBar::setTabButton(index, QTabBar::RightSide, new editor::CloseButton);
	editor::CloseButton* close_button = static_cast<editor::CloseButton*>(tabButton(index, QTabBar::RightSide));

	connect(close_button, &editor::CloseButton::clicked, this, &TabBar::closeTab);
}



void TabBar::closeTab() {
	QPoint cursor_pos = mapFromGlobal(QCursor::pos());
	for (qint32 i = 0; i < count(); ++i) {
		auto close_button = static_cast<editor::CloseButton*>(tabButton(i, QTabBar::RightSide));
		if (close_button == nullptr)
			continue;

		if (tabRect(i).contains(cursor_pos)) {
			static_cast<editor::TabWidget*>(parentWidget())->widget(i)->deleteLater();
			break;
		}
	}
}



bool TabBar::eventFilter(QObject* object [[maybe_unused]], QEvent* event) {
	if (event->type() == QEvent::Paint || event->type() == QEvent::HoverLeave)
		updateCloseButtons(mapFromGlobal(QCursor::pos()));

	return false;
}

}
