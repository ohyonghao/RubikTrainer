#include <QtGui/QApplication>
#include <QxtBasicSTDLoggerEngine>
#include <QxtLogger>
#include "RubikTrainer.h"

void startLogger();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setApplicationName( QObject::tr("Rubik's Cube Trainer") );
	a.setApplicationVersion("0.1");
	a.setOrganizationName("Rain Drop LLC");

	// Startup logging code
	startLogger();
    RubikTrainer w;
    w.show();

    return a.exec();
}
void startLogger(){
	// Without this the logger doesn't work
	// With it it prints double - weird
	qxtLog->addLoggerEngine("Rubik's",new QxtBasicSTDLoggerEngine);
}
