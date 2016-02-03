#python3

__author__ = 'Alrash'

import sys,os
import configparser
import re

def getData():
	conf = configparser.ConfigParser()
	conf.read("timer.conf")
	sections = conf.sections()
	ss = ""
	for options in sections:
		tmp = options + "," + conf.get(options, "timer") + "," + conf.get(options, "week") + "," + conf.get(options, "settime") + ";"
		if (int(conf.get(options, "week"), 16) & 0x80 == False) and re.match(r'^(.+),(0|1),(0x[0-9A-Fa-f][0-9A-Fa-f]),(0\d{1}|1\d{1}|2[0-3])([0-5]\d{1})([0-5]\d{1});$',tmp):
               		ss += tmp
	return ss
