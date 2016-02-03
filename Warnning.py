#from tkinter import *
#import tkinter.messagebox as messagebox

#def showWindow(name, time):
#    messagebox.showinfo('Warnning!', '%s\nTime:%s' % (name, time))

#def showWindow(info):
#	print('%s' % info)
#	messagebox.showinfo('Warnning!', '%s' % info)

#class Application(Frame):
#    def showWindow(self, info = 'Alarm'):
#       Frame.__init__(self, 'Warnning!')
#       self.helloLabel = Label(self, info)
#       self.pack()

#import time
#import winsound
#def play_music():
#   winsound.PlaySound('alert', winsound.SND_ASYNC)
#    time.sleep(3)

# -*- coding: utf-8 -*-
import pyaudio
import wave

def warnning(info):
	n = 0
	for x in range(10):
		print(info)
	chunk = 1024
	wf = wave.open(r"./lily white - 乙姫心で恋宮殿.wav", 'rb')
	p = pyaudio.PyAudio()
	# 打開聲音輸出流
	stream = p.open(format = p.get_format_from_width(wf.getsampwidth()),
	        	channels = wf.getnchannels(),
	        	rate = wf.getframerate(),
	        	output = True)
	# 寫聲音輸出流進行播放
	while True:
		data = wf.readframes(chunk)
		if data == "": break
		stream.write(data)
	stream.close()
	p.terminate()