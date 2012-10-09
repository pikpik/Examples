/*            Based on Beej's Guide to Network Programming
  http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#syscalls
                                                                       */

// We want to talk on the terminal the C++ way.
#include <iostream>

// We'll need these for sockets.
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main ( void ) {

  int connection;


  char server[] = "google.com";

  char port[] = "80";


  struct addrinfo hints;

  struct addrinfo *servinfo;


  // Let's chat with Google.com.

  std::cout
	<< std::endl
	<< "Connecting to " << server << " on port " << port << "." << std::endl
	<< std::endl
	<< std::endl;

  memset ( &hints, 0, sizeof hints );

  hints.ai_family = AF_UNSPEC;

  hints.ai_socktype = SOCK_STREAM;


  // What IP address is Google.com at anyway?

  int status;

  if (
    (
      status = getaddrinfo (
        server,
        port,
        &hints,
        &servinfo
      )
    ) != 0
  ) {

    std::cout << "Error: getaddrinfo says, \"" << gai_strerror ( status ) << "\"" << std::endl;

    return 1;

  }


  // Let's connect to it.

  struct addrinfo *somePort;

  int yes = 1;


  // Loop through the list of ports, and choose the first available one.

  for (
    somePort = servinfo;
    somePort != NULL;
    somePort = somePort->ai_next
  ) {


    // Can we use this port?

    if (
      (
        connection = socket (
          somePort->ai_family,
          somePort->ai_socktype,
          somePort->ai_protocol
        )
      ) == -1
    ) {

      // If it didn't work, restart the loop with the next port.

      std::cout << "Error: problem with socket; trying again..." << std::endl;

      continue;

    }


    // If the port has a socket attached to it but isn't in use,
    // then let's try to use it.

    if (
      setsockopt (
        connection,
        SOL_SOCKET,
        SO_REUSEADDR,
        &yes,
        sizeof ( int )
      ) == -1
    ) {

      // Let's not deal with bizarre things. If this doesn't work, then just quit.

      std::cout << "Error: setsockopt behaved unexpectedly." << std::endl;

      return 1;

    }


    // Can we connect?

    if (
      connect (
        connection,
        somePort->ai_addr,
        somePort->ai_addrlen
      ) == -1
    ) {

      // If we couldn't connect, then close this connection
      // and try again with the next port.

      std::cout << "Error: couldn't connect; trying again..." << std::endl;

      close ( connection );

      continue;

    }


    // If we get this point in the loop,
    // then don't go again.

    // This means we've got what we need.

    break;

  }


  // Did we somehow get a non-existent port?

  if ( somePort == NULL ) {

    // We have no idea what's going on, so quit.

    std::cout << "Error: failed to connect." << std::endl;

    return 1;

  }


  // We're done with this, so let's save some space.

  freeaddrinfo ( servinfo );


  // Let's send a message to Google.com.

  char message[] = "GET /\n\n";

  std::cout
	<< "Sending:" << std::endl
	<< std::endl
	<< "-------->8---------->8----------" << std::endl
	<< message << std::endl
	<< "--------8<----------8<----------" << std::endl
	<< std::endl
	<< std::endl;

  send ( connection, message, sizeof message, 0 );


  // Listen for Google's response.

  int response;

  char inbox [ 1024 ];

  int receiving = 1;

  std::cout
	<< "Here's " << server << "'s response:" << std::endl
	<< std::endl
	<< "-------->8---------->8----------" << std::endl;

  while ( receiving ) {

    // Empty the receiving area first.
    // Otherwise, we'll get the new message plus parts of the previous one.

    memset ( &inbox, 0, sizeof inbox );

    response = recv ( connection, &inbox, sizeof inbox, 0 );


    // Show the response or hang up if Google already did.

    if ( response > 0 )

      std::cout << inbox;

    else

      receiving = 0;

  }


  // The closing.

  close ( connection );

  std::cout
	<< std::endl
	<< "--------8<----------8<----------" << std::endl
	<< std::endl
	<< std::endl
	<< "Connection closed." << std::endl
	<< std::endl;


  // Was there a problem?

  if ( response == -1 )

    std::cout
	<< "(There were errors.)" << std::endl
	<< std::endl;;


  // Exeunt stage right.

  return 0;

}