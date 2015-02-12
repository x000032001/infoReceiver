#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "infoReceiver.h"

using namespace std;

infoReceiver r;
bool isRun;

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void* run(void* p)
{
	int sockfd, newsockfd, portno;
	puts("set port:4000");
	portno = 4000;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	clilen = sizeof(cli_addr);


	puts("binding");
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	puts("listening");
	listen(sockfd,5);

	newsockfd = accept(sockfd, 
			(struct sockaddr *) &cli_addr, 
			&clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");

	int n;
	while( isRun ) 
	{
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		string tmp( buffer , buffer+n );
		r.updateInfo(tmp);
	}

	close(newsockfd);
	close(sockfd);

	pthread_exit(NULL);
	return NULL;
}

int main()
{
	pthread_t t;
	while(1)
	{
		printf( "1. create server\n2. exit\n3. print all columns\n4. print all infos\n5. print selected column\n");
		int opt;
		scanf(" %d",&opt);
		switch(opt)
		{
			case 1:{
				long long int f = 0;
				isRun = true;
				int rrr = pthread_create(&t,NULL,run,NULL);
				if(rrr){
					error("create thread");
				}
				break;}
			case 2:{
				isRun = false;
				pthread_join(t,NULL);
				return 0;
				break;}
			case 3:{
				r.printColumns();
				break;}
			case 4:{
				r.printInfos();
				break;}
			case 5:{
				size_t col;
				scanf(" %llu",&col);
				try {
				r.printInfoByCol(col);
				} catch(...) {;}
				break;}
		}
	}

	return 0;
}
