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
	vector<string> getColumns();
	vector< vector<string> > getInfos();
	vector<string> getInfoByCol(size_t);

// debug help func
	void printColumns();
	void printInfos();
	void printInfoByCol(size_t);

private:
	bool hasHeader;
	string buffer;
	string host;
	deque<string> lines;
	vector<string> columnName;
	vector< vector<string> > infos;
	map<int, vector<string> > monitorVectors;

	void updateVector(int);
	int tryResolveHeader();
	int tryResolveInfo();
	int parseOneLine(string);
	string retrieveOneLineFromQueue();
	string tryCutString(string&, char) throw(int);
};


#endif
