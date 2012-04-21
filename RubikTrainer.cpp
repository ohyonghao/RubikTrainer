#include "RubikTrainer.h"
#include "ClockTimer.h"
#include "CustomWidgetListWidget.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QxtLogger>
#include <QxtCsvModel>
#include <QtSql/QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include "Algorithm.h"

RubikTrainer::RubikTrainer(QWidget *parent) :
	QMainWindow(parent),
	algModel(0),
	activeAlgModel(0),
	pllModel(0),
	timeFile(new QFile),
	pllFile(new QFile),
	algFile(new QFile),
	algWidget(0),
	algLayout(0),
	currentScramble(0),
	hlayout(0),
	vlayout(0),
	pbStart(0),
	ctClock(0)
{
	pllModel = new QSqlQueryModel(this);
	activeAlgModel = new QSortFilterProxyModel(this);
	activeAlgModel->setSourceModel(pllModel);
	activeAlgModel->setFilterKeyColumn(4);
	activeAlgModel->setFilterFixedString("TRUE");
	algModel = new QxtCsvModel(this);
	connect(algModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(algDataChanged()));
	setupUi();
	loadSettings();
	srand(QTime().msec());
	qxtLog->debug(QString("Active Alg Model Rows: %1 ").arg(activeAlgModel->rowCount()));

	currentScramble->setText(randomScramble());
	connect(pbStart,SIGNAL(clicked()),this,SLOT(startStop()),Qt::DirectConnection);
	QSize size;
	size.setHeight(cwListWidget->sizeHint().height()+20);
	size.setWidth(currentScramble->sizeHint().width()+cwListWidget->sizeHint().width()+30);
	this->setMinimumSize(size);
	this->resize(size);
}
void RubikTrainer::setupUi(){
	currentScramble = new QLabel("");
	currentScramble->setFrameStyle(QFrame::Panel|QFrame::Raised);
	pbStart = new QPushButton(tr("Start"));
	ctClock = new ClockTimer;
	vlayout = new QVBoxLayout;
	vlayout->addWidget(currentScramble);
	vlayout->addWidget(ctClock);
	vlayout->addWidget(pbStart);
	cwListWidget = new CustomWidgetListWidget;
	hlayout = new QHBoxLayout;
	hlayout->addLayout(vlayout);
	hlayout->addWidget(cwListWidget,1);
	if(this->centralWidget() == 0){
		QWidget *widget = new QWidget(this);
		widget->setLayout(hlayout);
		this->setCentralWidget(widget);
	}else{
	this->centralWidget()->setLayout(hlayout);
	}
	QSize size = cwListWidget->sizeHint();
	qxtLog->debug(cwListWidget->sizeHint().width());
	qxtLog->debug(cwListWidget->sizeHint().height());
	this->centralWidget()->setMinimumWidth(hlayout->sizeHint().width()+cwListWidget->sizeHint().width());
}

RubikTrainer::~RubikTrainer()
{
	delete timeFile;
	delete pllFile;
}
void RubikTrainer::loadSettings(){
	// The idea is to first find our directory wherever the OS stores
	// Application data by default
	// Check the settings if there was a last user
	// Check if that file exists
	// if it exists open it and load
	// if it doesn't exist then ask if they want to create a new session
	QSettings *s = new QSettings;
	s->beginGroup("SessionInfo");
	QString sessionName = s->value( "lastsession","").toString();
	if( sessionName == QVariant().toString()){
		sessionName = "newsession";
		QMessageBox::information(this,"Rubik's Cube Trainer", "No previous session available.");
	}
	QDir dir;
	if(!dir.exists(QDir::homePath()+"/Rubiks/"+sessionName)){
		qxtLog->info("Session Directory does not xist, will attempt to make.");
		if(!dir.mkpath(QDir::homePath()+"/Rubiks/"+sessionName)){
			qxtLog->warning("Could not create session dir");
			return;
		}
		qxtLog->info("Session Directory successfully created.");
	}
	dir.setCurrent(QDir::homePath()+"/Rubiks/"+sessionName);
	if( !QFile::exists( sessionName + ".csv" ) ){
		createSession(sessionName);
	}
		loadSession(sessionName);

	// Remove our qsettings
	delete s;

}
void RubikTrainer::createSession(const QString &sessionName){
	// Let's create this session now
	// We want to full the pll.csv over
	// This file shouldn't change much, unless they want to change what the scramble is
	pllFile->setFileName("pll.csv");
	if(pllFile->exists()){
		// What do we do if the file we want to use exists?
		// We can just load it I suppose
		qxtLog->info("Pll file already exists, trying to load anyway");
	}else{
		qxtLog->info("Copying pll.csv file");
		pllFile->copy(QDir::homePath()+"/Rubiks/pll.csv","pll.csv");
	}
	// We need a file to hold all the algorithms for solving
	// pll_id, alg_id, alg, notes
	algFile->setFileName("alg.csv");
	if(algFile->exists()){
		qxtLog->info("Algorithm file already exists, trying to load anyway");
	}else{
		qxtLog->info("Copying alg.csv file");
		algFile->copy(QDir::homePath()+"/Rubiks/alg.csv", "alg.csv");
	}
	// Next we want a way to keep track of individual times for each algorithm
	// This is where things get to be much more difficult
	// We can either go with the multiple files, one per algorithm, or with
	// repeating the same information in the file.  Since it is all text it isn't
	// really that big of a deal so a single file should do
	// We'll want pll_id, alg_id, time
	timeFile->setFileName(sessionName+".csv");
	if(timeFile->exists()){
		qxtLog->info("Time file already exists, trying to load anyway");
	}else{
		qxtLog->info("Creating "+timeFile->fileName()+" file");
		timeFile->open(QIODevice::ReadWrite);
		timeFile->write("");
		timeFile->close();
	}

}
void RubikTrainer::insertAlgs(){
	// we might think of how to store these files, probably in our install dir
}
void RubikTrainer::loadSession(const QString& sessionName ){
	// Close old sessions

	// change to new dir
	QDir dir;
	if(!dir.exists(QDir::homePath()+"/Rubiks/"+sessionName)){
		// Maybe ask to create session?
		return;
	}
	dir.setCurrent(QDir::homePath()+"/Rubiks/"+sessionName);
	// Load files
	pllFile->setFileName("pll.csv");
	if(!pllFile->open(QIODevice::ReadWrite)){
		qxtLog->warning("pll File did not open.");
		return;
	}

	pllModel->setSource(pllFile, true, ',');

	algFile->setFileName("alg.csv");
	if(!algFile->open(QIODevice::ReadWrite)){
		qxtLog->warning("alg File did not open");
		return;
	}

	algModel->setSource(algFile, true, ',');
	loadAlgorithms();
}
void RubikTrainer::startStop(){
	if(ctClock->started()){
		ctClock->stop();
		currentScramble->setText(randomScramble());
		// record time
	}else{
		ctClock->start();
	}
}
void RubikTrainer::loadAlgorithms(){
	if(pllModel->rowCount()==0){
		qxtLog->warning("No Rows");
		return;
	}
// before we load the scrambles we need to make sure that we have their algs loaded also
	int row = pllModel->rowCount();
	for( int i=0;i<row;i++){
		Algorithm *alg = new Algorithm(this);
		connect(alg,SIGNAL(toggled(QString,bool)),this,SLOT(algToggled(QString,bool)));
		alg->setScramble(pllModel->data(pllModel->index(i,2)).toString());
		QPixmap image(QDir::currentPath()+'/'+pllModel->data(pllModel->index(i,1)).toString(),"gif");
		if(image.isNull()){
			qxtLog->warning("Image not loaded");
			qxtLog->warning(QDir::currentPath()+'/'+pllModel->data(pllModel->index(i,1)).toString());
		}
		alg->setImage(image);
		alg->setAlgEnable(pllModel->data(pllModel->index(i,4)).toString()=="TRUE");
		alg->setPllId(pllModel->data(pllModel->index(i,0)).toString());
		alg->setAlgModel(algModel);
		cwListWidget->addWidget(alg);
		qxtLog->info(QString("Added Algorithm %1").arg(pllModel->data(pllModel->index(i,0)).toString()));
	}
	cwListWidget->show();
	return;
}

QString RubikTrainer::randomScramble(){
	int rowCount = activeAlgModel->rowCount();
	if(rowCount){
		return activeAlgModel->data(activeAlgModel->index(rand()%rowCount,2)).toString();
	}else{
		return tr("No Currently active Permutations.");
	}
}
void RubikTrainer::algToggled( QString pll_id, bool checked ){
	QModelIndexList list = pllModel->match(pllModel->index(0,0),Qt::DisplayRole,pll_id,1,Qt::MatchExactly);
	if(list.isEmpty()){
		qxtLog->warning("PLL ID not Found");
		return;
	}
	pllModel->setData(pllModel->index(list.first().row(),4), checked ? "TRUE":"FALSE");
	pllModel->toCSV(pllFile,true );
}
void RubikTrainer::algDataChanged(){
	qxtLog->info("Writing Alg File to disk");
	algModel->toCSV(algFile,true);
}
