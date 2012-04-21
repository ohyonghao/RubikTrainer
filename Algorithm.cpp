#include "Algorithm.h"
#include <QxtLogger>
#include <QString>
#include <QSortFilterProxyModel>

Algorithm::Algorithm(QWidget *parent) :
	QWidget(parent),
	pll_id(""),
	algModel(0)
{
	algModel = new QSortFilterProxyModel(this);
	setupUi(this);
	cbAlgorithms->setModel(algModel);
	cbAlgorithms->setModelColumn(1);
	connect(cbAlgorithms,SIGNAL(editTextChanged(QString)),this,SLOT(cbAlgFinishedEditing()));
	connect(cbAlgorithms,SIGNAL(currentIndexChanged(QString)),lCurrent,SLOT(setText(QString)));
	setMinimumSize(this->gbAlg->size());
	connect(this->gbAlg,SIGNAL(toggled(bool)),this,SLOT(algChecked(bool)));
	connect(pbNew,SIGNAL(clicked()),this,SLOT(pbNewPressed()));
	connect(teNotes,SIGNAL(textChanged()),this,SLOT(teNotesFinishedEditing()));
}
void Algorithm::setScramble(const QString &scramble){
	gbAlg->setTitle(scramble);
}
void Algorithm::setImage(QPixmap &image){
	lImage->setPixmap(image);
}
void Algorithm::setAlgEnable(bool enable){
	gbAlg->setChecked(enable);
}
void Algorithm::algChecked(bool checked){
	emit toggled(pll_id,checked);
}
void Algorithm::setPllId(QString _pll_id){
	pll_id = _pll_id;
	algModel->setFilterFixedString(pll_id);
}
void Algorithm::setAlgModel(QAbstractItemModel *model){
	qxtLog->trace("setAlgModel");
	algModel->setSourceModel(model);
	algModel->setFilterKeyColumn(0);
	algModel->setFilterFixedString(pll_id);
	cbAlgorithms->setModel(algModel);
	cbAlgorithms->setModelColumn(1);
	qxtLog->debug(QString("algModel Rows: %1, Columns: %2").arg(algModel->rowCount()).arg(algModel->columnCount()));
}
void Algorithm::cbAlgFinishedEditing(){
	QModelIndex index = algModel->index(cbAlgorithms->currentIndex(),1);
	algModel->setData(index,cbAlgorithms->currentText());
}
void Algorithm::pbNewPressed(){
	qxtLog->info(QString( "Insert New Algorithm for Pll %1" ).arg( pll_id ) );
	QAbstractItemModel *model = algModel->sourceModel();
	int row = model->rowCount();
	model->insertRow(row);
	QModelIndex index = model->index(row,0);
	if(!model->setData(index,pll_id)){
		qxtLog->warning(QString("New Algorithm not inserted for Pll %1").arg(pll_id));
	}
	index = model->index(row,1);
	model->setData(index,"New Algorithm");
	cbAlgorithms->setCurrentIndex(cbAlgorithms->count());
}
void Algorithm::teNotesFinishedEditing(){
	QModelIndex index = algModel->index(cbAlgorithms->currentIndex(),3);
	algModel->setData(index,teNotes->toPlainText());
}
