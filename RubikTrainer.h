#ifndef RUBIKTRAINER_H
#define RUBIKTRAINER_H

#include <QMainWindow>

class QSqlQueryModel;
class QFile;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class Algorithm;
class CustomWidgetListWidget;
class QPushButton;
class ClockTimer;
class QLabel;
class QSortFilterProxyModel;
class RubikTrainer : public QMainWindow
{
    Q_OBJECT

public:
    explicit RubikTrainer(QWidget *parent = 0);
    ~RubikTrainer();
	//virtual bool event(QEvent *event);
private:
	void loadSettings();
	void loadSession(const QString &sessionName);
	void loadAlgorithms();
	void createSession(const QString &sessionName);
	QString scramble(); // Produces a full scramble
	QString scramble( int algorithmId ); // Produces a scramble for algorithm
	QString randomScramble();

	// Csv Table Model
	QSqlQueryModel *algModel;
	QSortFilterProxyModel *activeAlgModel;
	QSqlQueryModel *pllModel;
	void insertAlgs();
	QFile *timeFile;
	QFile *pllFile;
	QFile *algFile;
	QWidget *algWidget;
	QVBoxLayout *algLayout;

	// Basic Layout
	void setupUi();
	QLabel *currentScramble;
	QHBoxLayout *hlayout;
	QVBoxLayout *vlayout;
	QPushButton *pbStart;
	ClockTimer *ctClock;
	CustomWidgetListWidget *cwListWidget;

	//QString stripquotes(QString);
private slots:
	void startStop();
	void algToggled(QString,bool);
	void algDataChanged();
};

#endif // RUBIKTRAINER_H
