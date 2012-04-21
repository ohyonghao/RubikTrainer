#include "CustomWidgetListWidget.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSize>
#include <QScrollBar>

CustomWidgetListWidget::CustomWidgetListWidget(QWidget *parent) :
	QWidget(parent),
	sa(0),
	layout(0),
	size(0,0)
{
	sa = new QScrollArea(this);
	sa->setWidget(new QWidget);
	sa->setWidgetResizable(true);
}
void CustomWidgetListWidget::addWidget(QWidget *widget){
	QVBoxLayout *vlayout;
	if(layout == 0){
		vlayout = new QVBoxLayout;
		layout = vlayout;
		this->sa->widget()->setLayout(layout);
	}
	vlayout = static_cast<QVBoxLayout *>(layout);
	vlayout->addWidget(widget);
	widgetList.append(widget);
	widget->show();
	sa->show();
	// calculate size
	QSize widgetSize = widget->sizeHint();
	if(widgetSize.width()>size.width()){
		size.setWidth(widgetSize.width()+sa->verticalScrollBar()->sizeHint().width()*3);
	}
	if(widgetSize.height()*2>size.height()){
		size.setHeight(widgetSize.height()*2);
	}
	sa->resize(size);
	sa->setMinimumSize(size);

}
QSize CustomWidgetListWidget::sizeHint() const{

	return sa->size();
}
