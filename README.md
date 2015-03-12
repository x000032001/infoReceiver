collectl info receiver
======================

This tool can help you receive message from collectl.
It can run with google test but I'm not using unit test actually.


Installation
------------

first get collectl and install it

	http://collectl.sourceforge.net/
	
then compile by yourself

	g++ -o main *.cpp -std=c++11 -pthread


Usage
-----

	1. get your ip address
	2. open our program, $ ./receiver
	3. run program in the system you want to monitor, like: 		
		$ collectl -A <ip you just got>:4000 -scm -P
	   don't forget -P option because we use plot format to commuicate.
	4. now you can see some message show in console

useless ha
