# hecho
Simple console HTTP Echo server

Command-line program to open a listening socket at a specified port number and echo to the console anything received on that port.  Designed originally for debugging HTTP sending code.

A meson build script is supplied. Current single source file can be compiled directly with 'gcc -o hecho src/hecho.c'
