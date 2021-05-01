/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
//IKT104 Group 16
//Wifi credentials in mbed_app.json
//The last 20% of this assignment took 80% of the time. Developer is stable, but still recovering.

     //Requirements:
 
    //10% Connect to your network. If you are at the lab you can use the student-FoU network. The password is stud2018
    // Print a log message saying if the connection was successful or not.
    //20% Connect to the following server: www.mocky.io (Lenker til en ekstern side.)
    // Print a log message saying if the connection was successful or not.
    //20% Send a HTTP GET request for the following resource: /v2/5e37e64b3100004c00d37d03
    // Note: Use www.mocky.io as Host in the request. Host is the FQDN only, protocol (http) and resource /v2/..) is not part of the host.
    //20% Print the response from the server while reading it
    //10% Store the received data in a character array instead of printing it directly, then after all is received, print this array.
    //20% The response is actually JSON formatted. Use a JSON library to parse the response, then: Print out first name, last name and age from the parsed data.
    //Note: This requirement is more difficult than the rest and should be attempted last. Even without this step you will get 80% for the rest.

//Libraries

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "mbed.h"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <Json.h>
#include <MbedJSONValue.h>

//Jsonparse func and json string declaration

void jsonparse(){
}
 
char json_str[1000] = { 0 };
 

//TCPrequest function
nsapi_size_or_error_t send_request(TCPSocket &socket, const char *request) {
    // The request might not be fully sent in one go, so keep track of how much we have sent
    nsapi_size_t bytes_to_send = strlen(request);
    nsapi_size_or_error_t bytes_sent = 0;

    printf("Requesting resource --> %s", request);

    // Loop as long as there are more data to send
    while (bytes_to_send) {
        // Try to send the remaining data. send() returns how many bytes were actually sent
        bytes_sent = socket.send(request + bytes_sent, bytes_to_send);

        // Negative return values from send() are errors
        if (bytes_sent < 0) {
            return bytes_sent;
        } else {
            printf("sent %d bytes\n", bytes_sent);
        }

        bytes_to_send -= bytes_sent;
    }

    printf("Complete message sent\n");

    return 0;
}

//TCP response reader, store response in char array
nsapi_size_or_error_t read_response(TCPSocket &socket) {
    // Storing response in memory
    char http_data[1000];
    int remaining_bytes = 1000;
    int received_bytes = 0;

    // Loop while data remains
    while (remaining_bytes > 0) {
        nsapi_size_or_error_t result = socket.recv(http_data + received_bytes, remaining_bytes);

        // Break when response is read
        if (result == 0) {
            break;
        }

        // Negative return values from recv() are errors
        if (result < 0) {
            return result;
        }

        received_bytes += result;
        remaining_bytes -= result;

        
    }


//remove header from site_data (try to anyway)
char site_data[1000];
strcpy(site_data, strstr(http_data, "{"));


    //Print recieved data from character array.

    printf("received %d bytes.\n__________________________________________________________________________________\nRaw data retrieved from site:\n\n%s\n__________________________________________________________________________________\n", received_bytes, site_data);



//PRINT DA JSON, MY FINAL MESSAGE     GOODBYE

//maybe we need to escape the json? idk
//strcpy(json_esc, site_data);

//new json suturingu desu 
int i = 0;
    while(site_data[i] != 123)
        {
            i++;
        }
    int z = strlen(site_data)-i;
    for(int y = 0; y< z;y++, i++)
    {
        json_str[y] = site_data[i];
    }

//finished successfully
    return 0;
}

int main(){


//Info print
printf("Mbed http reader with .json parser, IKT104 Group 16 Assignment 6\n");


//Wifi module interface
NetworkInterface *network = NetworkInterface::get_default_instance();
if(!network){
    printf("Failed to communicate with default interface, press RESET to retry\n");
    while(1);  //Hold if connectn't
}

//Network connection
printf("\nEstablishing network connetion...");
nsapi_size_or_error_t result = network->connect();

if (result != 0){
    printf("Error! connect() returned: %d\n press RESET to retry\n", result);
    while(1); //Hold if connection pizdets
}
printf("Connected successfully.\n");


// Create and allocate resources for socket
    TCPSocket socket;
    socket.open(network);

    // Create destination address
    SocketAddress address;

    // Get IP address of host by name
    result = network->gethostbyname("www.mocky.io", &address);

    // Check result
    if (result != 0) {
        printf("Failed to get IP address of host: %d\n press RESET to retry\n", result);
        while (1);
    }

    printf("Host IP aquired.\n");

    // Set server port number
    address.set_port(80);

    // Connect to server at the given address
    result = socket.connect(address);

    // Check result
    if (result != 0) {
        printf("Server connection failed: %d\n press RESET to retry\n", result);
        while (1);
    }

    printf("Connected to host server.\n");

    // HTTP request
    const char request[] = "GET /v2/5e37e64b3100004c00d37d03 HTTP/1.1\r\n"
                           "Host: www.mocky.io\r\n"
                           "Connection: close\r\n"
                           "\r\n";

    // Send request
    result = send_request(socket, request);

    // Check result
    if (result != 0) {
        printf("Failed to send request: %d, press RESET to retry\n", result);
        while (1);
    }



    // Read response
    result = read_response(socket);

// Check result
    if (result != 0) {
        printf("Failed to read response: %d, press RESET to retry\n", result);
        while (1);
    }

//Ayyo json time let's fvcking gooooooo
printf("\nDerived values from JSON:\n");
MbedJSONValue json_;
 
    parse(json_, json_str);
 
    std::string firstname;
    std::string lastname;
    int age;
 
    firstname = json_["first name"].get<std::string>();
    lastname = json_["last name"].get<std::string>();
    age = json_["age"].get<int>();
 
    printf("First name: %s\n\r", firstname.c_str());
    printf("Last name: %s\n\r", lastname.c_str());
    printf("Age: %i\n", age);
 





printf("\n~~End of program~~\n");


while(true){
ThisThread::sleep_for(1s);
}

}
