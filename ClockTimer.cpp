#include "ClockTimer.h"
#include <QTimer>
#include <QTime>

ClockTimer::ClockTimer(QWidget *parent) :
	QLCDNumber(parent),
	timer(new QTimer()),
	time(new QTime),
	pauseMilliseconds(0),
	bStarted(false)
{
	setSegmentStyle(Filled);
	setNumDigits(8);

	// We need a time that looks like it's being update every ms
	// but without lagging the system by being to fast, 10ms is too often
	// 42 also sometimes creates delays up to 200ms
	// I tested this by hitting the space bar as rapidly twice as possible
	timer->setInterval(61);
	connect(timer,SIGNAL(timeout()),this,SLOT(incrementClock()));
	time->start();
	reset();
}
void ClockTimer::incrementClock(){
	// since we are playing with millisecond timers we want to stop them
	// while we are doing these calcs so we don't queue up a bunch that would
	// then increase our time
	// the debate here is over whether or not we should grab the ms first or stop the
	// timer first, I decided stoping the timer was more critical because pulling
	// the elapsed time may cause it to shoot off one more time, thus increasing
	// the time that we put out
	bool timerState = timer->isActive();
	timer->stop();
	// Calc the time from milliseconds
	milliseconds = time->elapsed() + pauseMilliseconds;

	int min = 0;
	int sec = 0;
	int mil = 0;

	min = milliseconds/60000;
	milliseconds %= 60000;
	sec = milliseconds/1000;
	milliseconds %= 1000;
	mil = milliseconds;
	setDisplay(min,sec,mil);
	if(timerState)
		timer->start();
}
void ClockTimer::start(){
	timer->start();
	time->restart();
	bStarted=true;
}
void ClockTimer::stop(){
	timer->stop();
	incrementClock();
	// Reset afterwards because this is used in incrementClock();
	pauseMilliseconds=0;
	bStarted=false;
}
void ClockTimer::reset(){
	stop();
	setDisplay(0,0,0);
}
void ClockTimer::setDisplay(int min, int sec, int mil){
	QString displayString;
	displayString = QString("%1:%2:%3").arg(min,2,10,QChar('0'))
									   .arg(sec,2,10,QChar('0'))
									   .arg(mil,3,10,QChar('0'));
	this->display(displayString);
}
void ClockTimer::pause(){
	// We can't just call stop beceause that resets pauseMilliseconds
	timer->stop();
	incrementClock();
	// Needs to be caused afterwards because this is used in incrementClock();
	pauseMilliseconds += time->elapsed();
}
int ClockTimer::currentTime(){
	return time->elapsed()+pauseMilliseconds;
}
