#ifndef LINUX_STUFF_H_INCLUDED
#define LINUX_STUFF_H_INCLUDED

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>



void daemonize();



#endif



#endif