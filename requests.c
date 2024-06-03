#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char *cookies, int cookies_count, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(BUFLEN, sizeof(char));
 
    // Step 1: write the method name, URL, request params (if any) and protocol type
       
  
    sprintf(line,"GET %s HTTP/1.1\r\n", url);

      if(strcmp(query_params, "delete_book")==0){
        sprintf(line, "DELETE %s HTTP/1.1\r\n", url);
    }

    sprintf(line + strlen(line), "Host: %s:8080\r\n", host);
    strcat(line, "Connection: keep-alive\r\n");


    if(strcmp(query_params, "enter_library")==0 || strcmp(query_params, "logout") == 0){
        strcat(line, "Cookie: ");
        if (strcmp(cookies, "") != 0) {
                strcat(line, cookies);
                sprintf(line + strlen(line), ";\r\n");
        }
    }

    if(strcmp(query_params, "get_books")==0 || strcmp(query_params, "get_book")==0 || strcmp(query_params, "delete_book") == 0 ) {
        // add Bearer
        strcat(line, "Authorization: Bearer ");
        strcat(line, token);
        strcat(line, "\r\n");
    }

    // free line
    strcat(line, "\r\n");
   // printf("%s\n", line);
 
    return line;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count, char *keys[], char* token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *line1 = calloc(LINELEN, sizeof(char));
    char *line2 = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    // post ip:port //path
    sprintf(line, "POST %s HTTP/1.1\r\n", url);
    sprintf(line1, "Host: %s:8080\r\n", host);
    strcat(line, line1);
    strcat(line2, "Content-Type: application/json\r\n");
    strcat(line, line2);
    // add connecton keep alive
    strcat(line, "Connection: keep-alive\r\n");
    // add authorization is token is not null
    if (token != NULL) {
        // add Bearer
        strcat(line, "Authorization: Bearer ");
        strcat(line, token);
        strcat(line, "\r\n");
        }
    
   
    
    // turn body_data into a json
    char *json_data = calloc(LINELEN, sizeof(char));

    // make it general for any body data
    create_json_data(json_data, (const char **)keys, (const char **)body_data, body_data_fields_count);
   // sprintf(json_data, "{\"username\":\"%s\",\"password\":\"%s\"}", body_data[0], body_data[1]);

    // add the json data to the message
    sprintf(line + strlen(line), "Content-Length: %zu\r\n", strlen(json_data));
    strcat(line, "\r\n");
    strcat(line, json_data);
    strcat(line, "\r\n");
    //print line
   // printf("%s\n", line);

    return line;
}
