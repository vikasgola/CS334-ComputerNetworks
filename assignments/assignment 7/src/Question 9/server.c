#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include "net.h"
#include "file.h"
#include "mime.h"
//#include "cache.h"

#define PORT "9923"  // the port users will be connecting to

#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

/**
 * Send an HTTP response
 *
 * header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 * content_type: "text/plain", etc.
 * body:         the data to send.
 * 
 * Return the value from the send() function.
 */
int send_response(int fd, char *header, char *content_type, void *body, int content_length)
{
    const int max_response_size = 65536;
    char response[max_response_size];
    // Build HTTP response and store it in response
    time_t ticks;
    ticks = time(NULL);
    snprintf(response, sizeof(response), "%s\nDate: %.24s\r\nConnection: close\nContent-Length: %d\nContent-Type: %s\r\n\r\n%s\r\n\r\n", header , ctime(&ticks), content_length,content_type,(char*)body);

    //printf("%s\n",response);
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    int response_length = strlen(response);

    // Send it all!
    int rv = send(fd, response, response_length, 0);

    if (rv < 0) {
        perror("send");
    }

    return rv;
}


/**
 * Send a 404 response
 */
void resp_404(int fd)
{
    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}


/**
 * Handle HTTP request and send response
 , struct cache *cache
 */
void handle_http_request(int fd)
{
    const int request_buffer_size = 65536; // 64K
    char request[request_buffer_size];
    struct file_data *filedata; 
    char *mime_type;
    char filepath[4096];
    
    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }
    
    char* pch = strtok (request," ");
    while (pch != NULL){
        //printf("%s\n",pch);
        // handling get request
        if(strcmp(pch,"GET") == 0){
            printf("HTTP Request Received\n");
            pch = strtok (NULL, " ");
            if(strcmp(pch,"/") == 0)
                snprintf(filepath, sizeof filepath, "%s/index.html", SERVER_ROOT);
            else
                snprintf(filepath, sizeof filepath, "%s%s", SERVER_ROOT,pch);
            printf("filepath: %s\n",filepath);
            filedata = file_load(filepath);
            if (filedata == NULL) {
                // TODO: make this non-fatal
                fprintf(stderr, "cannot find system index file\n");
                resp_404(fd);
                break;
                //exit(3);
            }
            mime_type = mime_type_get(filepath);
            send_response(fd, "HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);
            //pch = strtok (NULL, " ");
            break;
        }
        pch = strtok (NULL, " ");
    }


    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Read the three components of the first request line

    // If GET, handle the get endpoints

    //    Check if it's /d20 and handle that special case
    //    Otherwise serve the requested file by calling get_file()


    // (Stretch) If POST, handle the post request
}

/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

//    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // forks a handler process to take care of it. The main parent
    // process then goes back to waiting for new connections.
    
    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        
        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.
        // , cache
        handle_http_request(newfd);
        //resp_404(newfd);
        close(newfd);
    }

    // Unreachable code

    return 0;
}

