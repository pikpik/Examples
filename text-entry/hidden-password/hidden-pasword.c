#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portablething.h"

int main ( int argc, char ** argv ) {

	char password [ 8 ];

	int character;

	printf ( "Please Enter your Password: " );

	while ( ( character = getch () ) != '\n' ) {

		printf ( "*" );

		int length = strlen ( password );
		password [ length ] = character;
		password [ length + 1 ] = '\0';

	}

	printf ( "\n" );

	printf ( "Password: %s\n", password );

	return EXIT_SUCCESS;

}
