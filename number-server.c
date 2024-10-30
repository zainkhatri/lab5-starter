#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "http-server.h"

int num = 0;  // Global variable to store the current number

void handle_response(char *path, int client_sock) {
    if (strcmp(path, "/shownum") == 0) {
        // Respond with the current number value
        char response[128];
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nYour number is %d", num);
        write(client_sock, response, strlen(response));
        return;
    }

    if (strcmp(path, "/increment") == 0) {
        // Increment the number by 1
        num += 1;
        char response[128];
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nYour number is %d", num);
        write(client_sock, response, strlen(response));
        return;
    }

    if (strstr(path, "/add?value=") == path) {
        // Parse the value from the URL and add it to the number
        int value_to_add = 0;
        sscanf(path, "/add?value=%d", &value_to_add);
        num += value_to_add;

        char response[128];
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nYour number is %d", num);
        write(client_sock, response, strlen(response));
        return;
    }

    // Handle 404 for unsupported paths
    char response[128];
    snprintf(response, sizeof(response), "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nPath not found: %s", path);
    write(client_sock, response, strlen(response));
}

int main() {
    // Start the server on a random available port
    start_server(handle_response, 0);
    return 0;
}
