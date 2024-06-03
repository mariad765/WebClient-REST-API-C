#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "buffer.h"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}

char *basic_extract_json_response(char *str)
{
    return strstr(str, "{\"");
}

void create_json_data(char *json_data, const char *keys[], const char *values[], int num_entries) {
 
    sprintf(json_data, "{");
    for (int i = 0; i < num_entries; i++) {
        strcat(json_data, "\"");
        strcat(json_data, keys[i]);
        strcat(json_data, "\":\"");
        strcat(json_data, values[i]);
        strcat(json_data, "\"");
        if (i < num_entries - 1) {
            strcat(json_data, ",");
        }
    }
    strcat(json_data, "}");
}

void get_cookies(char *response, char *cookies) {
    char *cookie = strstr(response, "Set-Cookie: ");
    if (cookie == NULL) {
        return;
    }
    cookie += strlen("Set-Cookie: ");
    char *end = strstr(cookie, ";");
    if (end == NULL) {
        return;
    }
    strncpy(cookies, cookie, end - cookie);
}

void get_token(char *json_str, char *token) {
    const char *token_field = "\"token\":\"";
    char *start = strstr(json_str, token_field);

    if (start == NULL) {
        printf("Token field not found\n");
        return;
    }

    start += strlen(token_field);
    char *end = strchr(start, '\"');

    if (end == NULL) {
        printf("End of token field not found\n");
        return;
    }

    strncpy(token, start, end - start);
    token[end - start] = '\0';
}

// Function to check if the response indicates an error
int is_error_response(const char *response, const char *error_string) {
  // split error string into a vector each element being untill newline
   // Make a writable copy of error_string
    char *error_string_copy = strdup(error_string);

    char *error = strtok((char *)error_string_copy, "\n");
    while(error != NULL) {
        if (strstr(response, error) != NULL) {
            return 1;
        }
        error = strtok(NULL, "\n");
    }

return 0;
   
}

char* extract_error_code_and_message(char *response, const char *error_string) {
  // split error string into a vector each element being untill newline
   // Make a writable copy of error_string
    char *error_string_copy = strdup(error_string);

    char *error = strtok((char *)error_string_copy, "\n");
    while(error != NULL) {
        if (strstr(response, error) != NULL) {
            break;
        }
        error = strtok(NULL, "\n");
    }
    // add to the error message the error code in the json
    char *json = basic_extract_json_response(response);
    // get rid of {}
    json++;
    json[strlen(json) - 1] = '\0';
    strcat(error, json);
    return error;
   
   
}
