#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QWidget>
#include <QPixmap>
#include "ui_Algorithm.h"
class QSortFilterProxyModel;
class QString;
class QFile;
class Algorithm : public QWidget, private Ui::AlgUi
{
    Q_OBJECT
public:
    explicit Algorithm(QWidget *parent = 0);
	virtual QSize sizeHint() const{ return QSize(this->minimumSize()); }
	// These are the set functions needed for the algorithm widget
	// We are going to use a model for the comboBox and use
	// the right side for editing that model
	void setScramble(const QString&);
	void setImage(QPixmap &);
	void setAlgEnable(bool);
	void setPllId(QString);
	void setAlgModel(QAbstractItemModel*);
signals:
	void toggled(QString,bool);
private slots:
	void algChecked(bool);
	void cbAlgFinishedEditing();
	void pbNewPressed();
	void teNotesFinishedEditing();
private:
	QString pll_id;
	QSortFilterProxyModel *algModel;

};

#endif // ALGORITHM_H
