#include <stdio.h>
#include <signal.h>

volatile sig_atomic_t eventHappened = 0;

volatile sig_atomic_t waiting = 1;


void responseForSignal ( void (* action) (int), int signal ) {

	struct sigaction signalResponse;


	// Have the responder ignore all signals.

	sigset_t ignoredSignals;

	sigfillset ( & ignoredSignals );


	// Get ready.

	// The responder should use the function,
	// ignore all signals,
	// and then start listening again.

	signalResponse.sa_handler = action;

	signalResponse.sa_mask = ignoredSignals;

	signalResponse.sa_flags = SA_RESTART;


	// Listen for the signal.

	sigaction ( signal, & signalResponse, NULL );

}


void respondToSIGUSR1 ( int signal ) {

	eventHappened = signal;

}


void respondToSIGUSR2 ( int signal ) {

	eventHappened = signal;

}


void runLoop ( void ) {

	sigset_t listeningSignals, allSignals;


	// Say which signals we're listening for.

	sigemptyset ( & listeningSignals );

	sigaddset ( & listeningSignals, SIGUSR1 );

	sigaddset ( & listeningSignals, SIGUSR2 );


	// Begin the "critical section" by ignoring our signals until we're suspended.

	sigprocmask ( SIG_BLOCK, & listeningSignals, & allSignals );


	// Listen for our signals once we uncover our ears.

	responseForSignal ( respondToSIGUSR1, SIGUSR1 );

	responseForSignal ( respondToSIGUSR2, SIGUSR2 );


	while ( waiting ) {

		printf ( "Pre: %d\n", eventHappened );


		// Ignore all other signals while we wait.
		// The previously blocked signals is unblocked.

		sigsuspend ( & allSignals );


		printf ( "Post: %d\n", eventHappened );

		// Let's go again!

		eventHappened = 0;

	}


	// This ends the "critical section" by listening to signals normally again.

	sigprocmask ( SIG_UNBLOCK, & listeningSignals, NULL );

}


int main ( void ) {

	runLoop ();

	return 0;

}
