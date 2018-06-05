#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include "networking.h"
#define BUFFSIZE 1024

int main(int argc,char** argv){
	char* host_or_ip;
	char* save_dir;
	char* starting_URL;
	int port,command_port,num_threads,sock,response_status;
	int i, found;
	unsigned int serverlen;
	struct sockaddr_in server;
	struct sockaddr *serverptr;
	struct hostent *rem;
	int length;
	char temp;
	char buffer[BUFFSIZE];
	
	/**
		Checking command line arguments
	**/
	if(argc == 12){
		for(i=1;i<(argc-2);i++){
			if(!strcmp(argv[i],"-h")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				host_or_ip = argv[i+1];
				break;
			}
		}
		if(!found){
			printf("-h flag and its value are required.\n");
			return 1;
		}
		found = 0;
		for(i=1;i<(argc-2);i++){
			if(!strcmp(argv[i],"-p")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				port = atoi(argv[i+1]);
				break;
			}
		}
		if(!found){
			printf("-p flag and its value are required.\n");
			return 1;
		}
		found = 0;
		for(i=1;i<(argc-2);i++){
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
		for(i=1;i<(argc-2);i++){
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
		for(i=1;i<(argc-2);i++){
			if(!strcmp(argv[i],"-d")){
				found = 1;
				if(argv[i+1][0] == '-'){
					printf("All flags must have a value.\n");
					return 1;
				}
				save_dir = argv[i+1];
				break;
			}
		}
		if(!found){
			printf("-d flag and its value are required.\n");
			return 1;
		}
		starting_URL = argv[11];
	}
	else{
		printf("Wrong number of arguments.\n");
		return 1;
	}
	
	/**
		Sending request for file
	**/
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return 1;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof server;
	if ((rem = gethostbyname(host_or_ip)) == NULL){
		perror("gethostbyname");
		return 1;
	}
	bcopy((char *) rem -> h_addr, (char *) &server.sin_addr,rem -> h_length);
	server.sin_port = htons(port);
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof server;
	if (connect(sock, serverptr, serverlen) < 0){
		perror("connect");
		return 1;
	}
	strcpy(buffer,save_dir);
	strcat(buffer,starting_URL);
	if(send_request(sock,starting_URL) < 0)
		puts("Failed to get file");
	if((response_status = parse_response(sock,buffer)) < 0){
		puts("Failed to read response");
		return 1;
	}
	if(response_status == 403)
		puts("Forbidden");
	else if(response_status == 404)
		puts("Not Found");
	else
		puts("OK");
	
	return 0;
}