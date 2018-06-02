#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char** argv){
	char* host_or_ip;
	char* save_dir;
	char* starting_URL;
	int port,command_port,num_threads;
	int i, found;
	
	
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
	
	return 0;
}
