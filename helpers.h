#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

// make the json
void create_json_data(char *json_data, const char *keys[], const char *values[], int num_entries);

// get cookies from response
void get_cookies(char *response, char *cookies);

//get token from response
void get_token(char *response, char *token);

// check error message
int is_error_response(const char *response, const char *error_string);

// extract error
char* extract_error_code_and_message(char *response, const char *error_string);


#endif