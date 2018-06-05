#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#define BUFFSIZE 1024

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

int parse_request(int fp,char* buffer,char* root_dir){
	char* word;
	char temp;
	char url[BUFFSIZE];
	int length;
	
	/**
		Check request format
	**/
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
	if(strcmp(word,"GET")){
		puts("Not a get request");
		return -1;
	}
	if(!(word = strtok(NULL," "))){
		puts("URL missing");
		return -1;
	}
	else{
		//Save file path
		strcpy(url,root_dir);
		strcat(url,word);
	}
	if(!(word = strtok(NULL," ")) || strcmp(word,"HTTP/1.1")){
		puts("Wrong request format");
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
	if(strcmp(word,"User-Agent:")){
		puts("Wrong request format");
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
	if(strcmp(word,"Host:")){
		puts("Wrong request format");
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
	if(strcmp(word,"Accept-Language:")){
		puts("Wrong request format");
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
	if(strcmp(word,"Accept-Encoding:")){
		puts("Wrong request format");
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
		puts("Wrong request format");
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
	if(strcmp(buffer,"\n")){
		puts("Wrong request format");
		return -1;
	}
	
	strcpy(buffer,url);
	return 0;
}

int send_response(int fp,int response_status){
	char buffer[BUFFSIZE];
	int length;
	char temp;
	char status[4];
	time_t rawtime;
	struct tm * timeinfo;
	
	stpcpy(buffer,"HTTP/1.1 ");
	sprintf(status, "%d", response_status);
	strcat(buffer,status);
	if(response_status == 200)
		strcat(buffer," OK");
	else if(response_status == 403)
		strcat(buffer," Forbidden");
	else if(response_status == 404)
		strcat(buffer," Not Found");
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
	strcpy(buffer, "Date: ");
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strcat(buffer,asctime(timeinfo));
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
	strcpy(buffer,"Server: myhttpd/1.0.0 (Ubuntu64)");
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
	strcpy(buffer,"Content-Length: 124");
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
	strcpy(buffer,"Content-Type: text/html");
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
	strcpy(buffer,"Connection: Closed");
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
	strcpy(buffer,"\n");
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
	
	if(response_status == 403){
		strcpy(buffer,"<html>Trying to access this file but I don't think I can make it.</html>");
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
	}
	else if(response_status == 404){
		strcpy(buffer,"<html>Sorry dude, couldn't find this file.</html>");
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
	}
	return 0;
}

void send_file(int sock_fp,FILE* fp){
	struct stat file_stat;
	char buffer[BUFFSIZE];
	char sizebuf[10];
	int bytes_to_transfer,length;
	char temp;
	
	//Find file size in bytes
	if (fstat(fileno(fp), &file_stat) < 0){
        perror("fstat");
		exit(EXIT_FAILURE);
    }
	bytes_to_transfer = file_stat.st_size;
	//Send number of bytes to be transferred to client
	printf("Transferring: %d\n",(int) bytes_to_transfer);
	sprintf(buffer,"%d",(int) bytes_to_transfer);
	length=strlen(buffer) + 1;
	temp = length;
	if(write(sock_fp,&temp,1) < 0){
		perror("write");
		exit(EXIT_FAILURE);
	}
	if(write(sock_fp,buffer,length) < 0){
		perror("write");
		exit(EXIT_FAILURE);
	}
	//Send file
	while(bytes_to_transfer > 0){
		if(bytes_to_transfer > BUFFSIZE-1){
			fgets(buffer,BUFFSIZE-1,fp);
			bytes_to_transfer-=(BUFFSIZE - 1);
			length = BUFFSIZE - 1;
		}
		else{
			fgets(buffer,bytes_to_transfer,fp);
			length = bytes_to_transfer;
			bytes_to_transfer = 0;
		}
		sprintf(sizebuf,"%d",length);
		length=strlen(sizebuf) + 1;
		temp = length;
		if(write(sock_fp,&temp,1) < 0){
			perror("write");
			exit(EXIT_FAILURE);
		}
		if(write(sock_fp,sizebuf,length) < 0){
			perror("write");
			exit(EXIT_FAILURE);
		}
		length = atoi(sizebuf);
		if(write(sock_fp,buffer,length) < 0){
			perror("write");
			exit(EXIT_FAILURE);
		}
		buffer[length] = '\0';
	}
}

int parse_response(int fp,char* filename){
	char* word;
	char temp;
	char buffer[BUFFSIZE];
	int length,status,i,bytes_to_read;
	FILE* save_fp;
	struct stat sb;
	
	//Check response headers
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
	printf("%s\n",buffer);
	word = strtok(buffer," ");
	if(strcmp(word,"Connection:")){
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
	if(status == 200){
		//Create directories to save file if they don't exist
		printf("%s\n",filename);
		length = strlen(filename);
		for(i=0;i<length;i++){
			if(filename[i] == '/'){
				strncpy(buffer,filename,i);
				buffer[i] = '\0';
				if (stat(buffer, &sb) != 0 || !S_ISDIR(sb.st_mode))
					mkdir(buffer, 0777);
			}
		}
		if((save_fp = fopen(filename,"w")) == NULL ){
			return -1;
		}
		//Read and save file
		if(read(fp,&temp,1) < 0){
			puts("failed to read");
			return -1;
		}
		length = temp;
		if(read(fp,buffer,length) < 0){
			puts("failed to read");
			return -1;
		}
		bytes_to_read = atoi(buffer);
		printf("Reading: %d\n",bytes_to_read);
		while(bytes_to_read > 0){
			if(read(fp,&temp,1) < 0){
				puts("failed to read");
				return -1;
			}
			length = temp;
			if(read(fp,buffer,length) < 0){
				puts("failed to read");
				return -1;
			}
			length = atoi(buffer);
			if(read(fp,buffer,length) < 0){
				puts("failed to read");
				return -1;
			}
			buffer[length] = '\0';
			fprintf(save_fp,"%s", buffer);
			bytes_to_read -= length;
		}
		fclose(save_fp);
		return status;
	}
	else{
		if(read(fp,&temp,1) < 0){
			puts("failed to read");
			return -1;
		}
		length = temp;
		if(read(fp,buffer,length) < 0){
			puts("failed to read");
			return -1;
		}
		return status;
	}
}