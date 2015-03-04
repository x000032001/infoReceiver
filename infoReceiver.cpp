#include "infoReceiver.h"

infoReceiver::infoReceiver()
{
	hasHeader = false;	
}

int infoReceiver::updateInfo( string str )
{
	if( str.empty() ) return 1;

	buffer += str;

	while(1)
	{
		try { 
			string res = tryCutString(buffer, '\n');
			lines.push_back(res);
		} catch ( ... ) { break; }
	}

	if( !hasHeader && tryResolveHeader() == OK )
		hasHeader = true;
	else
		tryResolveInfo();

	return 1;
}

void infoReceiver :: registerItem( deque<string> *que ,int col ,int max )
{
	maxSize[col] = max;
	monitorVec[col] = que;
}

void infoReceiver :: sync()
{
	for( auto it : monitorVec )
	{
		int col = it.first;
		it.second->insert( it.second->begin()+it.second->size() , tempData[col].begin() , tempData[col].end() );
		while( it.second->size() > maxSize[col] )
			it.second->pop_front();
	}
}

vector<string> infoReceiver::getColumns()
{
	return columnName;
}

void infoReceiver::printColumns()
{
	vector<string> col = getColumns();
	for( size_t i = 0 ; i < col.size() ; ++i )
		cout << i << ":" << col[i] << "\n";
	cout << endl;
}


int infoReceiver::tryResolveHeader()
{
	if( lines.empty() ) return ERR;

	string info = retrieveOneLineFromQueue();

	host = "";
	try { 
		host = tryCutString(info, '#');
	} catch ( ... ) { return ERR; }

	stringstream ss(info);
	columnName.clear();
	columnName.push_back(host);
	string tmp;
	while( ss >> tmp )
		columnName.push_back(tmp);
	
	if( columnName.empty() )
		throw runtime_error("get hostname but not have any column.");

	return OK;
}

int infoReceiver::tryResolveInfo()
{
	while( !lines.empty() )
	{
		string line = retrieveOneLineFromQueue();
		if( ERR == parseOneLine(line) )
		{
			puts("warning: illegal line.");	
		}
	}

	return OK;
}

int infoReceiver::parseOneLine( string line )
{
	stringstream ss(line);
	vector<string> tmp;
	string s;
	while( ss >> s )	
		tmp.push_back(s);

	if( tmp.size() != columnName.size() )
		return ERR;
	
	for( auto it : monitorVec )
	{
		int col = it.first;
		tempData[col].push_back( tmp[col] );
	}

	return OK;
}

string infoReceiver::retrieveOneLineFromQueue()
{
	if( lines.empty() )
		throw runtime_error("queue is empty.");
	string info = lines.front();
	lines.pop_front();

	return info;
}

string infoReceiver::tryCutString(string& str, char delim) throw(int)
{
	size_t pos = str.find(delim);	
	if( pos == string::npos )
		throw NOT_FOUND;

	string ret = str.substr( 0, pos );
	str.erase( 0, pos+1 );

	return ret;
}

