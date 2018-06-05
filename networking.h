#ifndef NETWORKING
#define NETWORKING

/**
	Crawler: Send a GET request through socket fp for file in url
**/
int send_request(int fp,char* url);

/**
	Server: Read GET request in socket fp for file in root_dir.
	Returns complete relevant path to the file in buffer.
**/
int parse_request(int fp,char* buffer,char* root_dir);

/**
	Server: Send response to buffer: Full for status 403 and 404, only headers for 200
**/
int send_response(int fp,int response_status);

/**
	Server: Send text file fp through socket sock_fp
**/
void send_file(int sock_fp,FILE* fp);

/**
	Client: Read server response and save the file if necessary, return response status
**/
int parse_response(int fp,char* filename);

#endif