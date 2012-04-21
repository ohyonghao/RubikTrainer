#ifndef CLOCKTIMER_H
#define CLOCKTIMER_H

#include <QLCDNumber>
class QTimer;
class QTime;
class ClockTimer : public QLCDNumber
{
    Q_OBJECT
public:
    explicit ClockTimer(QWidget *parent = 0);
	bool started(){return bStarted;}
	int lastTime(){return milliseconds; }
	int currentTime();

signals:
	void clockChanged();

public slots:
	void start();
	void stop();
	void reset();
	void pause();
private slots:
	void incrementClock();
private:
	int milliseconds;

	void setDisplay(int,int,int);
	QTimer *timer;
	QTime *time;
	int pauseMilliseconds;
	bool bStarted;
};

#endif // CLOCKTIMER_H
