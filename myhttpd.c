#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc,char** argv){
	int serving_port,command_port,num_threads,serving_sock,command_sock;
	int i,found = 0;
	char* root_dir;
	struct sockaddr_in server,command;
	struct sockaddr *serverptr,*commandptr;
	unsigned int serverlen, commandlen;
	
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
		perror("server bind"); exit(-1);
	}
	if (bind(command_sock, commandptr, commandlen) < 0){
		perror("command bind"); exit(-1);
	}
	
	return 0;
}
