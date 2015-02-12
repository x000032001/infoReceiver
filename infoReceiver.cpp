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

vector<string> infoReceiver::getColumns()
{
	return columnName;
}

vector< vector<string> > infoReceiver::getInfos()
{
	return infos;
}

void infoReceiver::printColumns()
{
	vector<string> col = getColumns();
	for( size_t i = 0 ; i < col.size() ; ++i )
		cout << i << ":" << col[i] << "\n";
	cout << endl;
}

void infoReceiver::printInfos()
{
	vector< vector<string> > infos = getInfos();
	for( size_t i = 0 ; i < infos.size() ; ++i )
	{
		for( size_t j = 0 ; j < columnName.size() ; ++j )
			cout << infos[i][j] << "\t";
		cout << endl;
	}
}

void infoReceiver::printInfoByCol( size_t col )
{
	if( col >= columnName.size() )
		throw runtime_error("select column out of range");

	vector<string> val = getInfoByCol(col);
	cout << columnName[col] << endl;

	for( size_t i = 0 ; i < val.size() ; ++i )
		cout << val[i] << '\t';
	cout << endl;
}

vector<string> infoReceiver::getInfoByCol( size_t col )
{
	if( col >= columnName.size() )
		throw runtime_error("select column out of range");
	updateVector(col);

	return monitorVectors[col];
}

void infoReceiver::updateVector( int col )
{
	vector<string>& vec = monitorVectors[col];
	for( size_t i = vec.size() ; i < infos.size() ; ++i )
		vec.push_back( infos[i][col] );	
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
	{
		tmp.push_back(s);
	}

	if( tmp.size() != columnName.size() )
	{
		//printf("tmp.size=%d, col.size=%d, %s\n",tmp.size(),columnName.size(),line.c_str() );
		return ERR;
	}
	
	infos.push_back(tmp);
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

