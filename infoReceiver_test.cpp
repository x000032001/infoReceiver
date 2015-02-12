#include "infoReceiver.h"
#include <fstream>
#include <iostream>
using namespace std;

void testHelper( fstream &fin )
{
	string line;
	infoReceiver r;
	while( getline( fin , line) )
	{
		// getline eat '\n' from raw data
		line += '\n';
		r.updateInfo(line);
	}

	r.printColumns();
	r.printInfos();
	r.printInfoByCol(3);
}

TEST( infoReceiverTest , test1 )
{
	fstream fin( "test3" , ios::in );
	testHelper(fin);
}

