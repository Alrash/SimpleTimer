//#pragma once

#ifndef _SIMPLETIMER_H

#define _SIMPLETIMER_H

#include <deque>
#include <string>
#include "Timer.h"

using std::string;
using std::deque;

class SimlpeTimer :public Timer
{
private:
	deque<Info> info;

	/*template<class T>
	inline string ReturnString(const T &ss)
	{
		string str;
		str += ss;
		return ss;
	}*/

	string GetData();
	void Windows(const string &name, const string &settime, bool isTimer) const;

public:
	SimlpeTimer();
	~SimlpeTimer();
	
	int Init();
	void Run();
};

#endif