#include <algorithm>
#include <ctime>
#include <Python.h>
#include "SimpleTimer.h"

#ifdef linux
#include <unistd.h>				//sleep函数
#endif

#ifdef _WIN32
#include <windows.h>			//Sleep函数
#define sleep(a) Sleep((a)*1000)//Sleep(millionseconds);
#endif

#ifdef __TEST__
#include <iostream>				//test
#define __FORTEST__
#endif

SimlpeTimer::SimlpeTimer()
{
}

SimlpeTimer::~SimlpeTimer()
{
	info.clear();
}

/*
 * 调用GetData.py脚本，读取timer.conf配置文件
 */
string SimlpeTimer::GetData()
{
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyObject *pModule = NULL, *pFunc = NULL;
	char *str;
	pModule = PyImport_ImportModule("GetData");				//调用的文件名
	pFunc = PyObject_GetAttrString(pModule, "getData");			//调用的函数名
	PyArg_Parse(PyEval_CallObject(pFunc, NULL), "s", &str);			//获取返回值
	Py_Finalize();
	return str;
}

void SimlpeTimer::Windows(const string & name, const string & settime, bool isTimer) const
{
	string str;

	if (isTimer)
		str = "Alarms  ";
	else
		str = "ShutDown  ";
	str += name + "    Time:" + settime;

	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyObject *pModule = NULL, *pFunc = NULL;
	PyObject *pArgs;
	pModule = PyImport_ImportModule("Warnning");				//调用的文件名
	pFunc = PyObject_GetAttrString(pModule, "warnning");		//调用的函数名

	pArgs = PyTuple_New(1);
	PyTuple_SetItem(pArgs, 0, PyBytes_FromString(str.c_str()));
	PyObject_CallObject(pFunc, pArgs);

	Py_Finalize();
}

/*
 * 从文件中获得数据的格式：name1,isTimer,week,settime;name2,isTimer,week,settime;name3...settime;
 */
int SimlpeTimer::Init()
{
	string str;
	int pos = 0, npos = std::string::npos, tpos;
	Info tmp;

#ifdef __FORTEST__
	str = "love,1,0x1f,152420; abc,0,0x11,000100;ajcs,1,0x11,220332;";		//test
#else
	/*
	* 调用python
	* 读取配置文件数据
	* 数据格式如上
	*/
	str = GetData();
#endif

	while ((npos = str.find(";", pos)) != std::string::npos)
	{
		//get name
		tpos = str.find(",", pos);
		tmp.name = str.substr(pos, tpos - pos);
		
		//find this clock whether is timer
		pos = tpos += 2;
		if (str.at(pos - 1) == '0')
			tmp.isTimer = false;
		else
			tmp.isTimer = true;

		//get week
		tpos = str.find(",", pos + 1);
		tmp.week = std::stoi(str.substr(pos + 1, tpos - pos - 1), nullptr, 16);

		//get setting time
		pos = tpos; tpos = str.find(",", pos + 1);
		tmp.settime = std::stoi(str.substr(pos + 1, tpos - pos - 1));

		//join in array
		info.push_back(tmp);
		pos = npos + 1;
	}

#ifdef __FORTEST__
	//for test
	for (std::deque<Info>::iterator it = info.begin(); it != info.end(); it++)
		std::cout << it->isTimer << " " << it->name << " " << it->week << " "
			<< it->settime.hour << " " << it->settime.min << " " << it->settime.sec << "\n";
#endif

	time_t timer;
	tm now;

	time(&timer);
	now = *localtime(&timer);

	for (std::deque<Info>::iterator item = info.begin(); item != info.end(); item++)
	{
		if (item->isTimer == true)
		{
			//set timer
			now.tm_hour = item->settime.hour;
			now.tm_min = item->settime.min;
			now.tm_sec = item->settime.sec - 10;

			item->lefttime = mktime(&now) - timer;
			if (item->lefttime < 0)
				item->lefttime += 86400;			//3600 * 24 = 86400
		}
		else
		{
			//get showdown time
			item->lefttime = item->settime.hour * 3600 + 60 * item->settime.min + item->settime.sec;

			now = *localtime(&timer);
			item->settime.hour += now.tm_hour;
			item->settime.min += now.tm_min;
			item->settime.sec += now.tm_sec;
		}
	}

	//ascending
	sort(info.begin(), info.end());

#ifdef __FORTEST__
	//for test
	for (std::deque<Info>::iterator it = info.begin(); it != info.end(); it++)
		std::cout << it->isTimer << " " << it->name << " " << it->week << " "
		<< it->settime.hour << " " << it->settime.min << " " << it->settime.sec
		<< " " << it->lefttime << "\n";
#endif

	return 0;
}

/*
 * 运行计时器，调用python弹出提示窗口
 */
void SimlpeTimer::Run()
{
	time_t offset = 0;
	time_t timer, tmp = 0;
	tm now;
	deque<Info>::iterator item;

	while (!info.empty())
	{
		time(&timer);
		now = *localtime(&timer);

		now.tm_hour = info.front().settime.hour;
		now.tm_min = info.front().settime.min;
		now.tm_sec = info.front().settime.sec;

		offset = mktime(&now) - timer - info.front().lefttime;
		//std::cout << info.front().name << " " << mktime(&now) << " " << timer << " " << info.front().lefttime << " " << offset << "\n";;

		if (info.front().lefttime < 2)
		{
			/*
			 * 使用python，弹出窗口
			 * 需要检查week
			 * 窗口提示信息――name, 时间
			 */
			if (!info.front().isTimer ||((0x01 << now.tm_wday) & info.front().week) || info.front().week == 0x00)		//For example, 0x02(0000 0010) & 0x0f(0111 1111) 
			{
				/*
				 * 调用Windows函数，弹出窗口提示
				 */
				Windows(info.front().name, 
					std::to_string((int)info.front().settime.hour) + ":" + 
					std::to_string((int)info.front().settime.min) + ":" + 
					std::to_string((int)info.front().settime.sec), info.front().isTimer);

				/*
				 * 这里无论是计时器，还是倒计时，都不影响
				 */
				info.front().lefttime += 86400;
			}

			/*
			 * 如果是计时器，则加入队尾
			 * 然后队首元素出队
			 */
			if (info.front().isTimer)
				info.push_back(info.front());
			info.pop_front();
		}
		else if (info.front().lefttime <= 40)
			sleep(1), tmp = 1;
		else if (info.front().lefttime <= 60)
			sleep(10 - (int)offset), tmp = 10;
		else if (info.front().lefttime <= 600)
			sleep(60 - (int)offset), tmp = 60;
		else if (info.front().lefttime <= 3600)
			sleep(600 - (int)offset), tmp = 600;
		else
			sleep(3600 - (int)offset), tmp = 3600;

		for (item = info.begin(); item != info.end(); item++)
			item->lefttime -= tmp;
	}
}
