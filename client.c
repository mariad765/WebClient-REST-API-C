#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    // imput
    char input[50];

    // port 
    int port = 8080;
    // ip
    char ip[50] = "34.246.184.49";
    // cookie
    char cookies[BUFLEN];
    memset(cookies, 0, sizeof(cookies));
    strcpy(cookies, "");
    // token
    char token[BUFLEN];
    memset(token, 0, sizeof(token));
    strcpy(token, "");
   
    // error string codes
   char *error_string = "400 Bad Request\n"
                     "401 Unauthorized\n"
                     "402 Payment Required\n"
                     "403 Forbidden\n"
                     "404 Not Found\n"
                     "405 Method Not Allowed\n"
                     "406 Not Acceptable\n"
                     "407 Proxy Authentication Required\n"
                     "408 Request Timeout\n"
                     "409 Conflict\n"
                     "410 Gone\n"
                     "411 Length Required\n"
                     "412 Precondition Failed\n"
                     "413 Payload Too Large\n"
                     "414 URI Too Long\n"
                     "415 Unsupported Media Type\n"
                     "416 Range Not Satisfiable\n"
                     "417 Expectation Failed\n"
                     "418 I'm a Teapot\n"
                     "421 Misdirected Request\n"
                     "422 Unprocessable Entity\n"
                     "423 Locked\n"
                     "424 Failed Dependency\n"
                     "425 Too Early\n"
                     "426 Upgrade Required\n"
                     "428 Precondition Required\n"
                     "429 Too Many Requests\n"
                     "431 Request Header Fields Too Large\n"
                     "451 Unavailable For Legal Reasons\n";


    // variables for commands

    // register
    char *register_command = "register";
    char *register_way = "/api/v1/tema/auth/register";

    // login
    char *login_command = "login";
    char *login_way = "/api/v1/tema/auth/login";

    //enter_library
    char *enter_library_command = "enter_library";
    char *enter_library_way = "/api/v1/tema/library/access";

    // get_books
    char *get_books_command = "get_books";
    char *get_books_way = "/api/v1/tema/library/books";

    // get_book
    char *get_book_command = "get_book";
    char *get_book_way = "/api/v1/tema/library/books/";
    int id = -1;

    // add_book
    char *add_book_command = "add_book";
    char *add_book_way = "/api/v1/tema/library/books";

    // delete_book
    char *delete_book_command = "delete_book";
    char *delete_book_way = "/api/v1/tema/library/books/";

    // logout
    char *logout_command = "logout";
    char *logout_way = "/api/v1/tema/auth/logout";

    // exit
    char *exit_command = "exit";

 

    // Comanda acceptata este register
   printf("Comanda acceptata este:\n"
       "register - efectuează înregistrarea\n"
       "login - efectuează autentificarea\n"
       "enter_library - cere acces în bibliotecă\n"
       "get_books - cere toate cărţile de pe server\n"
       "get_book - cere informaţie despre o carte\n"
       "add_book - adaugă o carte\n"
       "delete_book - șterge o carte\n"
       "logout - efectueaza delogarea utilizatorului\n"
       "exit - efectueaza ieșirea din program\n");

    while(1){
        // clean buffer
       memset(input, 0, sizeof(input));
     
        // read input
       scanf("%s", input);
       // get over newline
         getchar();

        if(strcmp(input, register_command)&& strcmp(input, login_command) && 
        strcmp(input, enter_library_command) && strcmp(input, get_books_command) && strcmp(input, get_book_command) 
        && strcmp(input, add_book_command) && strcmp(input, delete_book_command) && strcmp(input, logout_command) && 
        strcmp(input, exit_command)){
            printf("Comanda invalida ERROR\n");
            continue;
        }
        if(strcmp(input, register_command) == 0){

            int invalid_reg = 0;
            // check if client is already logged with another account
            if(strcmp(cookies, "") != 0){
                printf("Trebuie sa te deloghezi pt a inregistra alt utilizator ERROR\n");
                continue;
            }
            // register
            // read username
            char username[50] = {0};
            // wait for a username
            printf("username= ");
            fgets(username, sizeof(username), stdin);  // Read line including spaces
            username[strcspn(username, "\n")] = 0;     // Remove the newline character
            if(strlen(username)==0){
                invalid_reg = 1;
            }
            // check for spaces
            if (strpbrk(username, " ") != NULL) {
                invalid_reg = 1;
            }
             if (strpbrk(username, ",.\":;/][\\{})!@#$^&*()'+=<>?|]") != NULL) {
                invalid_reg = 1;
            }
            // read password
            char password[50] = {0};
            // wait for a password
            printf("password= ");
            fgets(password, sizeof(password), stdin);  // Read line including spaces
            password[strcspn(password, "\n")] = 0;     // Remove the newline character
            if(strlen(password)==0){
                invalid_reg = 1;
            }
            // check for spaces
            if (strpbrk(password, " ") != NULL) {
                invalid_reg = 1;
            }

            if(invalid_reg == 1){
                // print the relevant error message
                printf("Date invalide  username sau password ce contin caractere interzise - spatii; ERROR\n");
                continue;
            }
            // number of entries
            int num_entries = 2;

            // create json 
            char *body_data[2];
            body_data[0] = username;
            body_data[1] = password;

            // keys
            const char *keys[num_entries];
            keys[0] = "username";
            keys[1] = "password";

            // create message
            message = compute_post_request(ip, register_way, "application/json", body_data, num_entries, NULL, NULL, keys, NULL);

            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
            

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            // print response
             //  printf("%s\n", response);

            // check if register was successful
            if(strstr(response, "201 Created") != NULL){
                printf("Register SUCCESS.\n");
            }

          

            // check for error
            if(is_error_response(response, error_string) == 1){
                // get error message
                char *error_message = extract_error_code_and_message(response, error_string);
                printf("%s; Register ERROR.\n", error_message);
            }
            

            // close connection
            close_connection(sockfd);

        } else if(strcmp(input, login_command) == 0){
            if(strcmp(cookies, "") != 0){
                printf("Sunteti deja logat ERROR\n");
                continue;
            }
            // login
            // read username
            char username[50];
            int invalid_log = 0;
            // wait for a username
            printf("username= ");
            fgets(username, sizeof(username), stdin);  // Read line including spaces
            username[strcspn(username, "\n")] = 0;     // Remove the newline character
            if(strlen(username)==0){
                invalid_log = 1;
            }
            if (strpbrk(username, ",.\":;/][\\{})!@#$^&*()_'+=<>?|]") != NULL) {
                invalid_log = 1;
            }
            if (strpbrk(username, " ") != NULL) {
                invalid_log = 1;
            }
            

           // read password
            char password[50] = {0};
            // wait for a password
            printf("password= ");
            fgets(password, sizeof(password), stdin);  // Read line including spaces
            password[strcspn(password, "\n")] = 0;     // Remove the newline character
            if(strlen(password)==0){
                invalid_log = 1;
            }
            // check for spaces
            if (strpbrk(password, " ") != NULL) {
                invalid_log = 1;
            }

            if(invalid_log == 1){
                printf("Date invalide login  username sau password ce contin caractere interzise - spatii; ERROR\n");
                continue;
            }

            // number of entries
            int num_entries = 2;

            // create json 
            char *body_data[2];
            body_data[0] = username;
            body_data[1] = password;

            // keys
            const char *keys[num_entries];
            keys[0] = "username";
            keys[1] = "password";

            // create message
            message = compute_post_request(ip, login_way, "application/json", body_data, num_entries, NULL, NULL, keys, NULL);

            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            //get cookies
            get_cookies(response, cookies);
            // print cookie
           // printf("Cookie: %s\n", cookies);

            // print response
          //  printf("%s\n", response);

            // check if login was successful
            if(strstr(response, "200 OK") != NULL){
                printf("Login SUCCESS.\n");
            }

            // check for error
            if(is_error_response(response, error_string) == 1){
                // get error message
                char *error_message = extract_error_code_and_message(response, error_string);
                printf("%s; Login ERROR.\n", error_message);
              
            }

            // close connection
            close_connection(sockfd);

        } 
        else if(strcmp(input, enter_library_command) == 0){

            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // must be logged
            if(strcmp(cookies, "") == 0 ){
                printf("Trebuie sa fiti logat ERROR\n");
                continue;
            }
            // check if you already are in library
            if(strcmp(token, "") != 0){
                printf("Sunteti deja in biblioteca ERROR\n");
                continue;
            }
    
            // get request
            message = compute_get_request(ip, enter_library_way, "enter_library", cookies, 1, token);

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            // get the token
            get_token(response, token);
            
            // print response
           // printf("%s\n", response);

            close_connection(sockfd);

            // check if enter_library was successful
            if(strstr(response, "200 OK") != NULL){
                printf("Enter_library SUCCESS.\n");
            }

            // check for error
            if(is_error_response(response, error_string) == 1){
                printf("Enter_library ERROR.\n");
            }

            // error if token is empty
            if(strcmp(token, "") == 0){
                printf("Token ERROR.\n");
            }

        } else if(strcmp(get_books_command,input) == 0) {

            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // must be logged
            if(strcmp(cookies, "") == 0 ){
                printf("Trebuie sa fiti logat ERROR\n");
                continue;
            }

            // must be in library
            if(strcmp(token, "") == 0 ){
                printf("Trebuie sa fiti in biblioteca ERROR\n");
                continue;
            }

            // get request
            message = compute_get_request(ip, get_books_way, "get_books", cookies, 1, token);

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            // print response
            //printf("%s\n", response);

            // check if get_books was successful
            if(strstr(response, "200 OK") != NULL){
                // extract json
                char *json = basic_extract_json_response(response);
                // print json
                if(json == NULL)
                printf("[]\n");
                if(json != NULL)
                printf("[%s]\n", json);
                printf("get_books SUCCES.\n");
            }

            // check for error
            if(is_error_response(response, error_string) == 1){
                // get error
                char *error_message = extract_error_code_and_message(response, error_string);
                printf("%s; get_books ERROR.\n", error_message);
            }

            close_connection(sockfd);

        } if(strcmp(get_book_command, input) == 0){
            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // must be logged
            if(strcmp(cookies, "") == 0 ){
                printf("Trebuie sa fiti logat ERROR\n");
                continue;
            }

            // must be in library
            if(strcmp(token, "") == 0 ){
                printf("Trebuie sa fiti in biblioteca ERROR\n");
                continue;
            }

            // get id
            char id_string_aux[50] = {0};
            memset(id_string_aux, 0, sizeof(id_string_aux));
            printf("id= ");
            fgets(id_string_aux, 50, stdin);
            
            // check if the id is valid
            if(strlen(id_string_aux) == 1){
                printf("Id invalid ERROR\n");
                continue;
            }
            
            // convert id to int
            id = atoi(id_string_aux);
            if(id <= 0){
                printf("Id invalid nu e numar ERROR\n");
                continue;
            }


            // add id to the url
            char *get_book_way_id = (char *)malloc(50);
            memset(get_book_way_id, 0, sizeof(get_book_way_id));
            strcpy(get_book_way_id, get_book_way);
            // turn int id into string
            char *id_string = (char *)malloc(50);
            memset(id_string, 0, sizeof(id_string));
            sprintf(id_string, "%d", id);
            strcat(get_book_way_id, id_string);

            message = compute_get_request(ip, get_book_way_id, "get_book", cookies, 1, token);

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            // print response
           // printf("%s\n", response);

            // check if get_book was successful
            if(strstr(response, "200 OK") != NULL){
                // extract json
                char *json = basic_extract_json_response(response);
                // print json
                if(json == NULL)
                printf("[]\n");
                if(json != NULL)
                printf("[%s]; ", json);
                printf("get_book SUCCES.\n");
            }

            // check for error
            if(is_error_response(response, error_string) == 1){
                // get error
                char *error_message = extract_error_code_and_message(response, error_string);
                // if it is 404 not found
                if(strstr(error_message, "404 Not Found") != NULL)
                printf("Cartea nu exista INDEX INVALID; ERROR. ");

                printf("%s; get_book ERROR.\n", error_message);

              //  printf("get_book ERROR.\n");
            }


            close_connection(sockfd);
       

        } else if(strcmp(input, add_book_command) == 0){
            // open connection
            sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

            // must be logged
            if(strcmp(cookies, "") == 0 ){
                printf("Trebuie sa fiti logat ERROR\n");
                continue;
            }

            // must be in library
            if(strcmp(token, "") == 0 ){
                printf("Trebuie sa fiti in biblioteca ERROR\n");
                continue;
            }

            // Invalid imput
            int invalid = 0;

            // read title
            char title[60] = {0};
            // wait for a title
   
          printf("title= ");
          fgets(title, 60, stdin);

          if(strlen(title)==1){
                invalid = 1;
          }

            // read author
            char author[60] = {0};
            // wait for a author
            printf("author= ");
           fgets(author, 60, stdin);
            if(strlen(author)==1){
                invalid = 1;
            }
        

            // read genre
            char genre[50] = {0};
            // wait for a genre
            printf("genre= ");
            fgets(genre, 50, stdin);
            if(strlen(genre)==1){
                invalid = 1;
            }
            
            // read page_count
            char page_count[50] = {0};
            int result;
            // wait for a page_count
            printf("page_count= ");
           fgets(page_count, 50, stdin);
           if(strlen(page_count)==1){
                invalid = 1;
            }

            // publisher
            char publisher[50] = {0};
            // wait for a publisher
            printf("publisher= ");
            fgets(publisher, 50, stdin);
            if(strlen(publisher)==1){
                invalid = 1;
            }
            
           // check if it can be converted to int
           if(invalid == 0){
            result = atoi(page_count);
            if(result <= 0){
                invalid = 1;
                printf("Page Count care nu e numar; ERROR\n");
                continue;
            }
            }

            // number of entries
            int num_entries = 5;

            // create json
            char *body_data[5];
            body_data[0] = title;
            body_data[1] = author;
            body_data[2] = genre;
            body_data[3] = page_count;
            body_data[4] = publisher;

            // make sure all fields have something
            if(strcmp(title, "") == 0 || strcmp(author, "") == 0 || strcmp(genre, "") == 0 || strcmp(page_count, "") == 0 || strcmp(publisher, "") == 0){
                printf("Toate campurile trebuie completate ERROR\n");
                continue;
            }
            if(invalid == 1){
                printf("Date invalide ERROR\n");
                continue;
            }else{
                // remove newline from every body data filed
                for(int i = 0; i < num_entries; i++){
                    body_data[i][strlen(body_data[i]) - 1] = '\0';
                }
            }

            // keys
            const char *keys[num_entries];
            keys[0] = "title";
            keys[1] = "author";
            keys[2] = "genre";
            keys[3] = "page_count";
            keys[4] = "publisher";

            // create message
            message = compute_post_request(ip, add_book_way, "application/json", body_data, num_entries, cookies, token, keys, token);

            // send message
            send_to_server(sockfd, message);

            // receive response
            response = receive_from_server(sockfd);

            // print response
            //  printf("%s\n", response);

            // check if add_book was successful
            if(strstr(response, "200 OK") != NULL){
                printf("add_book SUCCES.\n");
            }

            // check for error
            if(is_error_response(response, error_string) == 1){
                // get error
                char *error_message = extract_error_code_and_message(response, error_string);
                printf("%s; add_book ERROR.\n", error_message);
               // printf("add_book ERROR.\n");
            }

            close_connection(sockfd);

    }  else if(strcmp(input,delete_book_command)==0){
        // open connection
        sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

        // must be logged
        if(strcmp(cookies, "") == 0 ){
            printf("Trebuie sa fiti logat ERROR\n");
            continue;
        }

        // must be in library
        if(strcmp(token, "") == 0 ){
            printf("Trebuie sa fiti in biblioteca ERROR\n");
            continue;
        }

        // get id
        printf("id= ");
        char id_string_aux[50] = {0};
        fgets(id_string_aux, 50, stdin);
        // check if the id is valid
        if(strlen(id_string_aux) == 1){
            printf("Id invalid ERROR\n");
            continue;
        }
        // convert id to int
        id = atoi(id_string_aux);
        if(id <= 0){
            printf("Id invalid nu e numar ERROR\n");
            continue;
        }


        // add id to the url
        char *delete_book_way_id = (char *)malloc(50);
        memset(delete_book_way_id, 0, sizeof(delete_book_way_id));
        strcpy(delete_book_way_id, delete_book_way);
        // turn int id into string
        char *id_string = (char *)malloc(50);
        memset(id_string, 0, sizeof(id_string));
        sprintf(id_string, "%d", id);
        strcat(delete_book_way_id, id_string);

        // check if the id is valid 
        if(id == -1){
            printf("Id invalid ERRORR.\n");
            continue;
        }

        message = compute_get_request(ip, delete_book_way_id, "delete_book", cookies, 1, token);

        // send message
        send_to_server(sockfd, message);

        // receive response
        response = receive_from_server(sockfd);

        // print response
       // printf("%s\n", response);

        // check if delete_book was successful
        if(strstr(response, "200 OK") != NULL){
            printf("delete_book SUCCES.\n");
        }

        // check for error
        if(is_error_response(response, error_string) == 1){
            // get error
            char *error_message = extract_error_code_and_message(response, error_string);
            // if it is 404 not found 
            if(strstr(error_message, "404 Not Found") != NULL)
            printf("Cartea nu exista INDEX INVALID; ERROR.\n");
          
            printf("%s; delete_book ERROR.\n", error_message);
        }

        // close connection
        close_connection(sockfd);


    } else if(strcmp(input, logout_command) == 0){
        // open connection
        sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

        // must be logged
        if(strcmp(cookies, "") == 0 ){
            printf("Trebuie sa fiti logat ERROR\n");
            continue;
        }

        // get request
        message = compute_get_request(ip, logout_way, "logout", cookies, 1, token);

        // send message
        send_to_server(sockfd, message);

        // receive response
        response = receive_from_server(sockfd);

        // print response
       // printf("%s\n", response);

        // check if logout was successful
        if(strstr(response, "200 OK") != NULL){
            // coockie is nempty
            memset(cookies, 0, sizeof(cookies));
            strcpy(cookies, "");
            // token is empty
            memset(token, 0, sizeof(token));
            strcpy(token, "");
            printf("Logout SUCCESS.\n");
        }

        // check for error
        if(is_error_response(response, error_string) == 1){
            // get error
            char *error_message = extract_error_code_and_message(response, error_string);
            printf("%s; Logout ERROR.\n", error_message);
        }

        // close connection
        close_connection(sockfd);


        } else if(strcmp(input,exit_command) == 0){
            //printf("Inchidere program\n");
            return;

        }

    }

    return 0;
}
