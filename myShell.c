#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      
#include <sys/types.h>   
#include <sys/wait.h>    
#include <errno.h>  

static int counter = 0;
char * directory;
char *home;

char ** argumentSplit(char * arg) {
    char **inputArray = malloc(sizeof(char**) * 1000);
    char *args = strtok(arg, " ");
    int i = 0;
    while (args != NULL)
    {
        inputArray[i++] = args;
        args = strtok(NULL, " ");
    }
    //free(inputArray);
    return inputArray;
}

char * getCurrentWorkingDirectory() {
    char * dir = malloc(sizeof(char) * 100);
    getcwd(dir, 100);
    return dir;
}

int runTheCommand(char ** arguments) {
    int i = 0;
    while (arguments[i] != NULL) {
        arguments[i] = strtok(arguments[i], " \n");
        i++;
    }
    arguments[i++] = NULL;
    execvp(arguments[0], arguments);
    return 1;
}

void sigquit() {
    wait(NULL);
    printf("[%d]+ Done\n", counter);
    counter--;
}

void forking(char ** arguments)  {
    struct sigaction sigact;
    //sigact.sa_flags = 0;
    sigact.sa_handler = SIG_DFL;
    pid_t childpid;  
    int status = 0;   
    int i = 0;
    int flag = 0;
    childpid = fork();
    char * newArr[25];
    if ( childpid >= 0 ) {
        if ( childpid == 0 ) {  
            while (arguments[i] != NULL) {
                if (strcmp(arguments[i], "history\n") == 0 || strcmp(arguments[i], "history") == 0 || strcmp(arguments[i], " history\n") == 0) {
                    int j = 0;
                    while (arguments[j] != NULL) {
                        j++;
                    }
                    if (j == 2) {
                        if (strcmp(arguments[i+1], "-c\n") == 0) {
                            FILE *fp = fopen(".CIS3110_history", "w");
                            fclose(fp);
                        } else {
                            FILE *fp1 = fopen(".CIS3110_history", "r");
                            FILE *fp2 = fopen(".CIS3110_history", "r");
                            int lines = 0;
                            char *input2 = malloc(256);
                            int k = 0;
                            while (fgets(input2, 256, fp1) != NULL) {
                                lines++;
                            }
                            fclose(fp1);
                            for(int l = 0;l<atoi(arguments[1]);l++) {
                                k=0;
                                while (fgets(input2, 256, fp2) != NULL) {
                                    if(k == lines - l - 1) {
                                        printf("%d %s",k + 1, input2);
                                    }
                                    k++;
                                }
                                fp2 = fopen(".CIS3110_history", "r");
                            }
                        }
                    } else if (j == 1) {
                        int k = 1;
                        FILE *fp1 = fopen(".CIS3110_history", "r");
                        char *input = malloc(256);
                        while (fgets(input, 256, fp1) != NULL) {
                            printf("%d %s", k, input);
                            k++;
                        }
                    }
                } else if (strcmp(arguments[i], "cd") == 0) {
                    flag = 3;
                    char * cdArg = malloc(500);
                    cdArg = arguments[i+1];
                    char * dir = malloc(256);
                    cdArg = strtok(cdArg, " \n");
                    cdArg[strlen(cdArg) + 1] = '\0';
                    if (strcmp(arguments[i+1], "~") == 0) {
                        chdir(home);
                        dir = getCurrentWorkingDirectory();
                    } else {
                        chdir(cdArg);
                        dir = getCurrentWorkingDirectory();
                    }
                    directory = dir;
                } else if (strcmp(arguments[i], "&\n") == 0) {
                    flag = 1;
                    arguments[i] = NULL;
                } else if (strcmp(arguments[i], "<") == 0) {
                    printf("hello\n");
                    char * filename;
                    filename = arguments[++i];
                    int j;
                    for (j = 0; j < i - 1; j++) {
                        newArr[j] = arguments[j];
                    }
                    newArr[j++] = NULL;
                    arguments[i] = NULL;
                    FILE *fp1;
                    filename = strtok(filename, "\n");
                    fp1 = freopen(filename, "r", stdin);
                    runTheCommand(newArr);
                    fclose(fp1); 
                } else if (strcmp(arguments[i], ">") == 0) {
                    printf("hello\n");
                    char * filename;
                    filename = arguments[++i];
                    int j;
                    for (j = 0; j < i - 1; j++) {
                        newArr[j] = arguments[j];
                    }
                    printf("%s\n", filename);
                    newArr[j] = NULL;

                    FILE *fp;
                    filename = strtok(filename, "\n");
                    fp = freopen(filename, "w", stdout);
                    runTheCommand(newArr); 
                    fclose(fp);
                }  else if (strcmp(arguments[i], "|") == 0) {
                    int pipeArray[2];
                    char *newArray[20];
                    char *newArray2[20];
                    pid_t pid;
                    int j;
                    pipe(pipeArray);
                    int count = 0;
                    while (arguments[count] != NULL) {
                        count++;
                    }
                    for (j = 0; j < i; j++) {
                        newArray[j] = arguments[j];
                    }
                    newArray[j++] = NULL;
                    j = 0;
                    int k = 0;
                    for (k = 0; k < count - i - 1; k++) {
                        for (j = i + 1; arguments[j] != NULL; j++) {
                            newArray2[k] = arguments[j];
                        }
                    }
                    newArray2[k++] = NULL;
                    arguments[i] = NULL;
                    if (pipe (pipeArray)) {
                        fprintf (stderr, "Pipe failed.\n");
                    }
                    pid = fork();

                    if (pid == 0) {
                        close(STDOUT_FILENO);
                        dup(pipeArray[1]);
                        close(pipeArray[0]);
                        close(pipeArray[1]);
                        runTheCommand(newArray);
                        exit(1);
                    }
                    else {
                        close(STDIN_FILENO);
                        dup(pipeArray[0]);
                        close(pipeArray[1]);
                        close(pipeArray[0]);
                        runTheCommand(newArray2);
                        exit(1);
                    }
                    close(pipeArray[0]);
                    close(pipeArray[1]);
                } else if (strcmp(arguments[i], "$PATH\n") == 0) {
                    flag = 3;
                    char *input = malloc(256);
                    char ** args;
                    char * str = malloc(256);
                    FILE *fp;
                    fp = fopen(".CIS3110_profile", "r");
                    while (fgets(input, 256, fp) != NULL) {
                        args = argumentSplit(input);
                        if (strstr(args[1], "PATH") != NULL) {
                            strcpy(str, args[1]);
                            str += 5;
                        } 
                    }        
                    printf("%s\n", str);
                    fclose(fp);
                }  else if (strcmp(arguments[i], "$HOME\n") == 0) {
                    flag = 3;
                    char *input = malloc(256);
                    char ** args;
                    char * str = malloc(256);
                    FILE *fp;
                    fp = fopen(".CIS3110_profile", "r");
                    while (fgets(input, 256, fp) != NULL) {
                        args = argumentSplit(input);
                        if (strstr(args[1], "HOME") != NULL) {
                            strcpy(str, args[1]);
                            str += 5;
                        } 
                    }   
                    home = str;
                    printf("%s\n", str);
                    fclose(fp);
                }  else if (strcmp(arguments[i], "$HISTFILE\n") == 0) {
                    flag = 3;
                    char *histfile = home;
                    strcat(histfile, "./CIS3110_history");
                    printf("%s\n", histfile);
                }
                i++;
            }   
            //sigact.sa_handler = SIG_DFL;
            if (flag != 3) {
                runTheCommand(arguments);
            }
            exit(status);
        } else {  
            i = 0; 
            while (arguments[i] != NULL) {
                if (strcmp(arguments[i], "cd") == 0) {
                    flag = 3;
                    char * cdArg = malloc(500);
                    cdArg = arguments[i+1];
                    cdArg = strtok(cdArg, " \n");
                    cdArg[strlen(cdArg) + 1] = '\0';
                    char *dir = malloc(256);
                    if (strcmp(arguments[i+1], "~") == 0) {
                        chdir(home);
                        dir = getCurrentWorkingDirectory();
                    } else {
                        chdir(cdArg);
                        dir = getCurrentWorkingDirectory();
                    }
                    directory = dir;
                    
                } else if (strcmp(arguments[i], "&\n") == 0) {
                    flag = 1;
                    arguments[i] = NULL;
                    counter++;
                } 
                i++;
            }
            if (flag == 1) {
                //sigact.sa_handler = sigquit;
                printf("[%d] %d\n", counter, childpid);
                waitpid(childpid, &status, WNOHANG);
                sigaction(SIGCHLD, &sigact, NULL);
            } else {
                waitpid(childpid,&status,0);
            }
        }
    } else {
        perror("fork");
        exit(-1);
    }
}

int main (int argc, char *argv[]) {
    char *input = malloc(256);
    char **in;
    directory = malloc(100);
    home = malloc(256);
    char * str = malloc(256);
    FILE *fp;
    char *input2 = malloc(256);
    char ** args;
    fp = fopen(".CIS3110_profile", "r");
    while (fgets(input2, 256, fp) != NULL) {
        args = argumentSplit(input2);
        if (strstr(args[1], "HOME") != NULL) {
            strcpy(str, args[1]);
            str += 5;
        } 
    }       
    str = strtok(str, "\n");
    home = str;
    free(input2);
    directory = getCurrentWorkingDirectory();
    printf("%s >", directory);
    while (fgets(input, 256, stdin) != NULL) {
        FILE *f = fopen(".CIS3110_history", "ab+");
        char *p = input;
        p[strlen(p) + 1] = '\n';
        if (f != NULL) {
            fputs(input, f);
            fclose (f);
        }
        in = argumentSplit(input);
        if (strcmp(input, "exit\n") == 0 || strcmp(input, " exit\n") == 0) {
            printf("myShell terminating...\n");
            exit(0);
        } 
        forking(in);
        printf("%s >", directory);
    }
    free(input);
    free(in);
}