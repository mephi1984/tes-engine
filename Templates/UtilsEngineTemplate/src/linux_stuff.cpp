#ifndef _WIN32

#include "linux_stuff.h"
#include <iostream>

void daemonize()
{
	/* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0)
		{
			std::cout<<"error, pid < 0"<<std::endl;
                exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
			std::cout<<"pid > 0 exit process"<<std::endl;
                exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
			std::cout<<"sid < 0 exit process"<<std::endl;
                exit(EXIT_FAILURE);
        }
        

        
        /* Change the current working directory */
        if ((chdir("/")) < 0) {
                /* Log the failure */
			std::cout<<"(chdir(\"/\")) < 0 exit process"<<std::endl;
                exit(EXIT_FAILURE);
        }
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
}





#endif
