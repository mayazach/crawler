#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#define BUFFSIZE 1024

int send_request(int fp,char* url);

int parse_response(int fp);

int main(int argc,char** argv){
	char* host_or_ip;
	char* save_dir;
	char* starting_URL;
	int port,command_port,num_threads,sock;
	int i, found;
	unsigned int serverlen;
	struct sockaddr_in server;
	struct sockaddr *serverptr;
	struct hostent *rem;
	int length;
	char temp;
	char buffer[BUFFSIZE];
	
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
	if(send_request(sock,starting_URL) < 0)
		puts("Failed to get file");
	if(parse_response(sock) < 0)
		puts("Failed to read response");
	
	return 0;
}

int send_request(int fp,char* url){
	char buffer[BUFFSIZE];
	int length;
	char temp;
	
	stpcpy(buffer,"GET ");
	strcat(buffer,url);
	strcat(buffer," HTTP/1.1");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "Host: linux00.di.uoa.gr");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "Accept-Language: en-us");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "Accept-Encoding: gzip, deflate");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "Connection: Close");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	strcpy(buffer, "\n");
	length=strlen(buffer) + 1;
	temp = length;
	if(write(fp,&temp,1) < 0){
		perror("write");
		return -1;
	}
	if(write(fp,buffer,length) < 0){
		perror("write");
		return -1;
	}
	
	return 0;
}

int parse_response(int fp){
	char* word;
	char temp;
	char buffer[BUFFSIZE];
	int length,status;
	
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"HTTP/1.1")){
		puts("Could not read response");
		return -1;
	}
	if(!(word = strtok(NULL," "))){
		puts("Response status missing");
		return -1;
	}
	else
		status = atoi(word);
	if(!(word = strtok(NULL," "))){
		puts("Could not read response");
		return -1;
	}
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"Date:")){
		puts("Could not read response");
		return -1;
	}
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"Server:")){
		puts("Could not read response");
		return -1;
	}
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"Content-Length:")){
		puts("Could not read response");
		return -1;
	}
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"Content-Type:")){
		puts("Could not read response");
		return -1;
	}
	if(read(fp,&temp,1) < 0){
		puts("failed to read");
		return -1;
	}
	length = temp;
	if(read(fp,buffer,length) < 0){
		puts("failed to read");
		return -1;
	}
	word = strtok(buffer," ");
	if(strcmp(word,"Connection:")){
		puts("Could not read response");
		return -1;
	}
	
	return 0;
}