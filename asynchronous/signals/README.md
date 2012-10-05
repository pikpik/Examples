Asynchronous Signals
====================

This example tries to illustrate how Unix signals can be used for "critical sections" and asynchronous events.


To Compile
----------

	cc -o example example.c


To try
------

In one terminal, type the following and press enter.

	./example

In another terminal, get the Process ID (PID) of the running program:

	ps

Then send signals to the program:

	kill -USR1 PID

(It may seem absurd to use "kill" for this, but that's the standard utility for sending signals.)

Watch the first terminal to see when the signals are received and what number of signal was seen.
