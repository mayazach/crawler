#ifndef NETWORKING
#define NETWORKING

int send_request(int fp,char* url);

int parse_request(int fp,char* buffer,char* root_dir);

int send_response(int fp,int response_status);

void send_file(int sock_fp,FILE* fp);

int parse_response(int fp,char* filename);

#endif