#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/fcntl.h>
#include <termios.h>

#include <termios.h>
#include <unistd.h>

static struct termios n_term;
static struct termios o_term;

static int
cbreak(int fd) 
{
   if((tcgetattr(fd, &o_term)) == -1)
      return -1;
   n_term = o_term;
   n_term.c_lflag = n_term.c_lflag & ~(ECHO|ICANON);
   n_term.c_cc[VMIN] = 1;
   n_term.c_cc[VTIME]= 0;
   if((tcsetattr(fd, TCSAFLUSH, &n_term)) == -1)
      return -1;
   return 1;
}

static int uncbreak (int fd) {
   if((tcsetattr(fd, TCSANOW, &o_term)) == -1)
      return -1;
   return 1;
}

int 
getch() 
{
   int cinput;

   cinput = getchar();

   return cinput;
}


int waiting = 1;


void newMessage ( int signal ) {

  char input [ 256 ];

  memset ( & input, 0, 1 );

  if (
    read (
      STDIN_FILENO,
      input,
      1
    ) > -1
  ) {

    printf ( "%s\n", input );

    //fflush ( stdout );

  }
/*
  printf("?");

  int character;
  character = getch ();
  uncbreak(STDIN_FILENO);
  printf ( "%d\n", character );
  cbreak(STDIN_FILENO);
*/

}


void responseForSignal ( void (* action) (int), int signal ) {

  struct sigaction signalResponse;

  sigset_t ignoredSignals;


  sigfillset ( & ignoredSignals );


  signalResponse.sa_handler = action;

  signalResponse.sa_mask = ignoredSignals;

  signalResponse.sa_flags = SA_RESTART;


  sigaction ( signal, & signalResponse, NULL );

}


int main ( void ) {

    /*
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);

    // don't care about writefds and exceptfds:
    select(STDIN+1, &readfds, NULL, NULL, &tv);

    if (FD_ISSET(STDIN, &readfds))
        printf("A key was pressed!\n");
    else
        printf("Timed out.\n");
    */

/*
  fcntl ( STDIN_FILENO, F_SETOWN, ( int ) getpid ( ) );

  //fcntl ( STDIN_FILENO, F_SETFL, FASYNC | O_NONBLOCK );


  // Set the terminal mode to not buffer characters.

  struct termios
	originalTerminalAttributes,
	newTerminalAttributes;

  tcgetattr (
    STDIN_FILENO,
    & originalTerminalAttributes
  );

  memcpy (
    & newTerminalAttributes,
    & originalTerminalAttributes,
    sizeof ( struct termios )
  );

  newTerminalAttributes.c_lflag &= ~ ( ECHO | ICANON );
  newTerminalAttributes.c_cc [ VTIME ] = 0;
  newTerminalAttributes.c_cc [ VMIN ] = 0;

  tcsetattr (
    STDIN_FILENO,
    TCSANOW,
    & newTerminalAttributes
  );
*/

/*   if(cbreak(STDIN_FILENO) == -1) {
      fprintf(stderr, "cbreak failure, exiting \n");
      exit(EXIT_FAILURE);
   }
*/
  // Signals.

  responseForSignal ( newMessage, SIGIO );

  sigset_t listeningSignals, allSignals;

  sigemptyset ( & listeningSignals );

  sigaddset ( & listeningSignals, SIGIO );

  sigaddset ( & listeningSignals, SIGUSR1 );

  sigprocmask ( SIG_BLOCK, & listeningSignals, & allSignals );


  // Wait and respond to events.

  while ( waiting ) {

    sigsuspend ( & allSignals );

  }

  sigprocmask ( SIG_UNBLOCK, & listeningSignals, NULL );


  // Resetting terminal attributes.
/*
  tcsetattr (
    STDIN_FILENO,
    TCSANOW,
    & originalTerminalAttributes
  );
*/
/*
   if(uncbreak(STDIN_FILENO) == -1) {
      fprintf(stderr, "uncbreak failure, exiting \n");
      exit(EXIT_FAILURE);
   }
*/

  return 0;

}
