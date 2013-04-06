#ifndef PGETCH
#define PGETCH

/*
Source	StackOverflow
From	http://stackoverflow.com/a/6857444
Author	Daniel Leschkowski - http://stackoverflow.com/users/839180/daniel-leschkowski
Editor	Tim Cooper - http://stackoverflow.com/users/142162/tim-cooper
*/

#if defined(__unix__) || defined(_POSIX_SOURCE) || defined(__APPLE__)
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

int 
getch() 
{
   int cinput;

   if(cbreak(STDIN_FILENO) == -1) {
      fprintf(stderr, "cbreak failure, exiting \n");
      exit(EXIT_FAILURE);
   }
   cinput = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &o_term);

   return cinput;
}

#elif _MSC_VER  || __WIN32__ || __MS_DOS__
  #include <conio.h>
#endif
#endif
