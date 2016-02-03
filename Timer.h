#ifndef _TIMER_H

#define _TIMER_H

#include <ctime>
#include <string>

class Timer
{
public:
	virtual int Init() = 0;
	virtual void Run() = 0;

private:
	struct StandrandTime
	{
		short int hour;		
		short int min;			//minutes
		short int sec;			//seconds

		//÷ÿ‘ÿ∏≥÷µ∫≈
		const StandrandTime &operator=(int settime)
		{
			sec = settime % 100;
			settime /= 100;
			min = settime % 100;
			settime /= 100;
			hour = settime % 100;
			
			return *this;
		}
	};

protected:
	struct Info
	{
		bool isTimer;			//True represents timer and false represents shutdown.
		int week;				//hex int type.
		time_t lefttime;		//Represent the left time by now.
		StandrandTime settime;	//hhmmss
		std::string name;

		//used by sorting
		bool operator<(const Info &b) const
		{
			return lefttime < b.lefttime;
		}
	};
};

#endif
