#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include "networking.h"
#define BUFFSIZE 1024

int main(int argc,char** argv){
	int serving_port,command_port,num_threads,serving_sock,command_sock,csock,readsocks;
	int i,found = 0,pages_served = 0,bytes_served = 0,index,mins_running,response_status;
	double seconds_running;
	char* root_dir;
	struct sockaddr_in server,command;
	struct sockaddr *serverptr,*commandptr;
	unsigned int serverlen, commandlen;
	FILE *sock_fp,*fp;
	char command_name[128];
	char buffer[BUFFSIZE];
	struct timeval  start, current;
	fd_set socks;
	int highsock = -1;
	
	/**
		Checking command line arguments
	**/
	if(argc == 9){
		for(i=1;i<(argc-1);i++){
			if(!strcmp(argv[i],"-p")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				serving_port = atoi(argv[i+1]);
				break;
			}
		}
		if(!found){
			printf("-p flag and its value are required.\n");
			return 1;
		}
		found = 0;
		for(i=1;i<(argc-1);i++){
			if(!strcmp(argv[i],"-c")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				command_port = atoi(argv[i+1]);
				break;
			}
		}
		if(!found){
			printf("-c flag and its value are required.\n");
			return 1;
		}
		found = 0;
		for(i=1;i<(argc-1);i++){
			if(!strcmp(argv[i],"-t")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				num_threads = atoi(argv[i+1]);
				break;
			}
		}
		if(!found){
			printf("-t flag and its value are required.\n");
			return 1;
		}
		found = 0;
		for(i=1;i<(argc-1);i++){
			if(!strcmp(argv[i],"-d")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				root_dir = argv[i+1];
				break;
			}
		}
		if(!found){
			printf("-d flag and its value are required.\n");
			return 1;
		}
	}
	else{
		printf("Wrong number of arguments.\n");
		return 1;
	}
	//Get start time
	gettimeofday(&start, NULL);

	/**
		Preparing sockets for serving client and receiving commands
	**/
	if ((serving_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("server socket");
		exit(-1);
	}
	if ((command_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("command socket");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(serving_port);
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof server;
	command.sin_family = AF_INET;
	command.sin_addr.s_addr = htonl(INADDR_ANY);
	command.sin_port = htons(command_port);
	commandptr = (struct sockaddr *) &command;
	commandlen = sizeof command;
	
	if (bind(serving_sock, serverptr, serverlen) < 0){
		perror("server bind"); 
		exit(1);
	}
	if (bind(command_sock, commandptr, commandlen) < 0){
		perror("command bind"); 
		exit(1);
	}
	
	if ( listen(command_sock, 5) != 0 ){
		perror( "listen" );
		exit(1);
	}
	if ( listen(serving_sock, 5) != 0 ){
		perror( "listen" );
		exit(1);
	}
	
	
	while(1){
		/**
			select() to get requests from both sockets
		**/
		highsock = -1;
		FD_ZERO(&socks);
		FD_SET(serving_sock, &socks);
		if(serving_sock > highsock)
			highsock = serving_sock;
		FD_SET(command_sock, &socks);
		if(command_sock > highsock)
			highsock = command_sock;
		
		readsocks = select(highsock+1, &socks, NULL, NULL, NULL);
		if (readsocks < 0) {
			perror("select");
			return 1;
		}
		else{
			/**
				Client socket
			**/
			if(FD_ISSET(serving_sock, &socks)){
				puts("serving sock");
				if ( (csock = accept(serving_sock, NULL, NULL)) < 0 ){
					perror("accept");
					return 1;
				}
				
				if(parse_request(csock,buffer,root_dir) < 0){
					puts("Could not process request");
					continue;
				}
				if((fp = fopen(buffer,"r")) == NULL){
					if(errno == ENOENT)
						response_status = 404;
					else if(errno == EACCES)
						response_status = 403;
					else
						response_status = 404;
				}
				else
					response_status = 200;
				if(send_response(csock,response_status) == 0 && response_status == 200){
					send_file(csock,fp);
					fclose(fp);
				}
				
			}
			/**
				Command socket
			**/
			if(FD_ISSET(command_sock,&socks)){
				if ( (csock = accept(command_sock, NULL, NULL)) < 0 ){
					perror("accept");
					return 1;
				}
				if ((sock_fp = fdopen(csock,"r+")) == NULL){
					perror("fdopen");
					return 1;
				}
				if (fgets(command_name, BUFFSIZE, sock_fp) == NULL){
					perror("reading command");
					return 1;
				}
				//Removing newlines from command
				index = strlen(command_name) - 1;
				while(index > 0){
					if((command_name[index] == '\n') || (command_name[index] == '\r')){
						command_name[index] = '\0';
						index--;
					}
					else 
						break;
				}
				if(!strcmp(command_name,"SHUTDOWN")){
					fclose(sock_fp);
					break;
				}
				else if(!strcmp(command_name,"STATS")){
					gettimeofday(&current, NULL);
					seconds_running = (double) (current.tv_usec - start.tv_usec) / 1000000 + (double) (current.tv_sec - start.tv_sec);
					mins_running = seconds_running / 60;
					seconds_running -= mins_running * 60;
					if(mins_running >= 10)
						fprintf(sock_fp,"Server up for %d:", mins_running);
					else
						fprintf(sock_fp,"Server up for 0%d:", mins_running);
					if(seconds_running >= 10)
						fprintf(sock_fp,"%f, served %d pages, %d bytes\n",seconds_running,pages_served,bytes_served);
					else
						fprintf(sock_fp,"0%f, served %d pages, %d bytes\n",seconds_running,pages_served,bytes_served);
				}
				else
					printf("Unknown command\n");
				fclose(sock_fp);
			}
		}
	}
	close(command_sock);
	close(serving_sock);
	
	return 0;
}
