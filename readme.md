Author: Megh Bhavsar
Date: Febraury 12, 2021
Program: myShell.c
Files:
    myShell.c
    .CIS3110_profile
    .CIS3110_history
    makefile

Functions Not Implemented - should implement:
 - Export function not implemented
 - Does not print done when & command finishes running, also does not kill zombie process
 - input / output redirection at the same time.

Program Functionality:
 - Implemented a UNIX shell program
 - Able to run all basic shell commands
 - Able to redirect output/input to/from file
 - Piping implemented
 - Able to change directories

 Compilation and Execution:
 - make
 - ./myShell

 Resources used:
 - https://linuxhint.com/pipe_system_call_c/, helped with piping
 - https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/#:~:text=The%20chdir%20command%20is%20a,the%20directory%20specified%20in%20path. - helped with cd 
 - other geeksforgeeks links for knwoledge with forking and sigaction. waitpid
 - https://www.geeksforgeeks.org/signals-c-language/ - signals
 - https://www.geeksforgeeks.org/fork-system-call/ - fork info
 
 Other Info
 - Handed in the .CIS3110_history and .CIS3110_profile