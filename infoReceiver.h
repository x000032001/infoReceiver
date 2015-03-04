#ifndef infoReceiver_H
#define infoReceiver_H

#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>
#include <string>
#include <deque>
#include <stdexcept>
#include <map>

#define OK 1
#define ERR 0
#define NOT_FOUND -1

using namespace std;

class infoReceiver
{
public:
	infoReceiver();
	int updateInfo(string);
	void registerItem(deque<string>*,int col,int max);
	void sync();

	vector<string> getColumns();

// debug help func
	void printColumns();

private:
	bool hasHeader;
	string buffer;
	string host;
	deque<string> lines;
	vector<string> columnName;
	vector< deque<string> > infos;

	map<int, int> maxSize;
	map<int, deque<string>* > monitorVec;
	map<int, deque<string> > tempData;

	void updateVector(int);
	int tryResolveHeader();
	int tryResolveInfo();
	int parseOneLine(string);
	string retrieveOneLineFromQueue();
	string tryCutString(string&, char) throw(int);
};


#endif
