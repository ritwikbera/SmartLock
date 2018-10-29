import simplejson as json
import datetime
import shutil
import serial
import os
port = "/dev/ttyACM0"
serialArduino = serial.Serial(port,9600)
serialArduino.flushInput()
ah=open('filename.txt','a')
def name_logging(val):
        shutil.copyfile("nameid.txt","buffer.txt")
        buf=open("buffer.txt","a")
        buf.write(']')
        buf.flush()
        buf.close()
        buf=open("buffer.txt","r")
        str1=buf.readline()
        obj=json.loads(str1)
        buf.close()
        i=0
        while True:
                 if i<120:
                        if obj[i][unicode('id')]==str(val):
                                logging(obj[i][unicode('name')])
                                break
			else:
				i=i+1
def logging(val):
    	var1=val
    	var2=datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    	if os.stat('filename.txt').st_size==0:
        	ah.write('[')
    	else:
        	ah.write(',')
	lh=open('lockstatus.txt','r')
	entry_type=lh.readline()
	lh.close()
	lh=open('lockstatus.txt','w')
	if entry_type=='in':
		lh.write('out')
	else:
		lh.write('in')
	lh.flush()
	lh.close()
    	ah.write('{"time":"'+var2+'","id":"'+var1+'","type":"'+entry_type+'"}')
    	ah.flush()
while True:
    	if os.stat('webcommand.txt').st_size==0:
		if serialArduino.inWaiting()>0:        	
			val = str(int(serialArduino.readline()))
        		print val
			if val=='200' or val=='199':
				logging(val)
			else:
				name_logging(val)
    	else:
		if open('webcommand.txt','r').readline()=='@':
			print 'guest'
        		serialArduino.write('5')
		else:
			serialArduino.write('$')
        	comfile=open('webcommand.txt','w')
        	comfile.close()
    
