#ifndef CUSTOMWIDGETLISTWIDGET_H
#define CUSTOMWIDGETLISTWIDGET_H

#include <QWidget>
class QScrollArea;
class CustomWidgetListWidget : public QWidget
{
    Q_OBJECT
public:
	explicit CustomWidgetListWidget(QWidget *parent = 0);
	void addWidget( QWidget * );
	//void insertWidget( int index, QWidget *);
	virtual QSize sizeHint() const;
signals:

public slots:
private:
	QList<QWidget *> widgetList;
	QScrollArea *sa;
	QLayout *layout;
	QSize size;
};

#endif // CUSTOMWIDGETLISTWIDGET_H
