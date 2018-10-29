from flask import Flask,render_template,request 
import simplejson as json 
import datetime 
import shutil 
import os 
app=Flask(__name__) 
fh=open("filename.txt","a") 
obj=0 
id=30
#entry_type="in"
ni=open("nameid.txt","w")
def create_json():
	global obj
        shutil.copyfile("filename.txt","buffer.txt")
        buf=open("buffer.txt","a")
        buf.write(']')
        buf.flush()
        buf.close()
        buf=open("buffer.txt","r")
        str1=buf.readline()
        obj=json.loads(str1)
        buf.close()
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

def add_id(name):
	global id
	global ni
	if os.stat('nameid.txt').st_size == 0:
		ni.write('[')
	else:
		ni.write(',')
	ni.write('{"name":"'+name+'","id":"'+str(id)+'"}')
        print name
	ni.flush()
def logging(val):
        var1=val
        var2=datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
        if os.stat('filename.txt').st_size==0:
		fh.write('[')
        else:
        	fh.write(',')
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
    	fh.write('{"time":"'+var2+'","id":"'+var1+'","type":"'+entry_type+'"}')
	fh.flush()
@app.route('/')
def hello():
	return render_template('login.htm')
@app.route('/en')
def hello2():
	return render_template('enroll.htm')
@app.route('/search.htm')
def show_log():
	return render_template('view_log.htm')
@app.route('/search',methods=['POST','GET'])
def fetch_id():
	search_id=request.form['id']
    	global obj
    	create_json()
    	i=0
        while True:
           	 if i<100:
                	if obj[i][unicode('id')]==str(search_id):
                    		return obj[i][unicode('time')]
                   		break
                 	else:
                    		i=i+1
@app.route('/enroll', methods=['POST', 'GET'])
def enroll():
	global id
	if valid_login(request.form['username'],request.form['password']):
		add_id(unicode(request.form['guestname']))
		comf=open('webcommand.txt','w')
		comf.write('$')
		comf.flush()
		comf.close()
		id=id+1;
		return render_template('confirmed.htm')
	else:
		return render_tempate('enroll.htm')
@app.route('/login', methods=['POST', 'GET'])
def login():
    	error = None
    	if request.method == 'POST':
        	if valid_login(request.form['username'],
                       request.form['password']):
                   	logging(unicode(request.form['guestname']))
			comf=open('webcommand.txt','w')
                   	comf.write('@')
                   	comf.flush()
                   	comf.close()
            		return render_template('confirmed.htm')
        	else:
            		error = 'Invalid username/password'
    			return render_template('login.htm', error=error)
@app.route('/view_log.htm')
def view_log():
    	global obj
    	create_json()
        templateData={ 'people':obj }
        return render_template('logs.htm',**templateData)
@app.route('/images.htm')
def images():
	return render_template('images.htm')
                   
def valid_login(username,password):
        return True
app.run(host='0.0.0.0',port=80)
        

