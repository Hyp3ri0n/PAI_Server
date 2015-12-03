/*
 * client.h
 *
 *  Created on: 3 déc. 2015
 *      Author: ortizlu
 */

#ifndef XML_PARSERS_CLIENT_H_
#define XML_PARSERS_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include "FromXMLToGoogleMapHTTPRequest.h"

int client(int port, char* nomServer);

#endif /* XML_PARSERS_CLIENT_H_ */
