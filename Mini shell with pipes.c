#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>

void inputForExecNoPipe(char* ptr, int stringLength,int count, char** command);//transforms string in ptr from string with nums into separate commands and flags in array called command
void processExecNoPipe (char** command);//execution of a command without a pipe
void processExecWithPipe (char** command, char** command2);//execution of two commands with one pipe
void processExecWithTwoPipes (char** command, char** command2, char** command3);//execution of 3 commands with two pipes
int allocating (char* ptr, char input[]);//arranging the input as strings and lengths into allocated memory
void  inputForExecWithPipe(char* ptrr, int stringLength,int count, char** command, char** command2);//transforms string in ptr from string with nums and one pipeinto separate commands in array called command and command2, each command in an array
void  inputForExecWithTwoPipe(char* ptrr, int stringLength,int count, char** command, char** command2, char** command3);//transforms string in ptr from string with nums and two pipes into separate commands in array called command and command2 and command3, each command in an array
void clearPointer (char** command);//frees allocated memory after usage is completed
int main() {

    int sumPipe = 0;//overall number of pipes used
    char input[150];
    int count = 1; //counts the number of commands inserted(done is automatically counted)
    struct passwd *p;
    p = getpwuid(getuid());
    int PATH_MAX=150;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {//user and directory ready, user input wanted

        if ((getpwuid(getuid())) == NULL)//failed to find the user, insert as user null
            printf("null @%s>",  cwd );

        else  printf("%s@%s>", p->pw_name, cwd );//user is determined and directory is ready
        fgets(input,512,stdin);

        int j = 0;
        while (input[0]== ' ') {//removing spaces before the command
            for (j = 0; j < strlen(input); j++)
                input[j] = input[j + 1];
        }

        int result= 100;
        if(input[0]=='d'&& input[1]=='o'&& input[2]=='n'&&input[3]=='e')//determining if the input is "done", if so script is terminated
            result = 0;

        int stringLength = strlen(input);

        char **command = malloc(count * sizeof(int));//allocating memory for pointers to strings of fixed command

        if (!command)// exit the program immediately if there is no enough memory.
        {
            printf("malloc failed\n");
            exit(0);
        }

        while (result != 0)//continueS entries until "done"

        {
            char *ptr = malloc(strlen(input) *
                               sizeof(int));//pointer to allocated memory where input is saved with words length instead of spaces in between

            if (!ptr)// exit the program immediately if there is no enough memory.
            {
                printf("malloc failed\n");
                exit(0);
            }


            if (result == 0) {//if input is "done, script terminated
                free(*command);
                exit(0);
        }
            else if (result != 0) {//input not "done", more commands to be inserted
                if (input[0] == 'c' && input[1] == 'd' && input[2] == ' '){//if input has cd, prints out msg, and new input to be inserted instead
                    printf("command not supported (Yet)\n");
                }
                else{
                    int pipeNum = allocating(ptr, input);//pipe number of each entry is returned, after input is arranged in memory with ptr pointing at it
                sumPipe = sumPipe + pipeNum;
                if (pipeNum == 0) {//in case no pipes inserted
                    inputForExecNoPipe(ptr, strlen(input), count,command);//as explained in declaration of the function
                    processExecNoPipe(command);//as explained in declaration of the function
                    wait(NULL);
                } else if (pipeNum == 1) {//in case one pipes inserted
                    char **command2 = malloc(count * sizeof(int));//allocating memory for pointers to strings of fixed command number2
                    if (!command2)// exit the program immediately if there is no enough memory.
                    {
                        printf("malloc failed\n");
                        exit(0);
                    }

                    inputForExecWithPipe(ptr, strlen(input), count, command,command2);//preparing command to be inserted to execvp()
                    processExecWithPipe(command, command2);
//                    clearPointer(command2);

                } else if (pipeNum == 2) {//in case 2 pipes inserted
                    char **command2 = malloc(count * sizeof(int));//allocating memory for pointers to strings of fixed command number2
                    if (!command2)// exit the program immediately if there is no enough memory.
                    {
                        printf("malloc failed\n");
                        exit(0);
                    }
                    char **command3 = malloc(count * sizeof(int));//allocating memory for pointers to strings of fixed command
                    if (!command3)// exit the program immediately if there is no enough memory.
                    {
                        printf("malloc failed\n");
                        exit(0);
                    }
                    inputForExecWithTwoPipe(ptr, strlen(input), count, command, command2,command3);
                    if (command2[0] == NULL) {//in case of two pipes without a command in between
                        processExecWithPipe(command, command3);
                    } else {
                        processExecWithTwoPipes(command, command2, command3);
                    }
//                    clearPointer(command2);
//                    clearPointer(command3);
                } else
                    printf("pipes number exceeded 2, please enter another command\n");

            }
            }

//            clearPointer(command);
                count++;
                //preparing command line again and input from user(like the beginning of the code)
                struct passwd *p;
                p = getpwuid(getuid());
                int PATH_MAX = 150;
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {//user and directory ready, user input wanted
                    if ((getpwuid(getuid())) == NULL)//failed to find the user
                        printf("null @%s>", cwd);

                    else printf("%s@%s>", p->pw_name, cwd);
                    fgets(input, 512, stdin);
                    while (input[0] == ' ') {
                        for (j = 0; j < strlen(input); j++)
                            input[j] = input[j + 1];
                    }

                    result = 100;
                    if (input[0] == 'd' && input[1] == 'o' && input[2] == 'n' && input[3] == 'e')
                        result = 0;
                } else {
                    perror("getcwd() error");
                    return 1;
                }


        }



    } else {//incase no directory found, an exception appears
        perror("getcwd() error");
        return 1;
    }
        printf("Num of commands: %d\n", count);
    printf("number of pipes; %d\n", sumPipe);
    printf("See You Next Time !\n");

    return 0;
    }





 void inputForExecNoPipe(char* ptrr, int stringLength,int count, char** command){
    int k =0;
    int t =0;
    int j;
    char commandTemp[150];
    for (j = 0; j < stringLength; j++) {//loops until the end of the first word

        while (ptrr[j]== ' '){//shifts to the first filled element of "input"
            j++;
        }
        if (ptrr[j] >= '0' && ptrr[j] <= '9'&& ptrr[j -1] != '-') {//a number found, a word is completed
            commandTemp[t] = '\0';//null is added to the end of the commandTemp
            command[k] =  malloc(t * sizeof(char));//memory allocated for the word
            strcpy(command[k] ,commandTemp);//the word added to the allocated memory
            t=0;
            k++;
            if(ptrr[j+1] == '\0'){//if it is the las word then a null is added to the next element

                command[k] =NULL;
            }
            if (ptrr[j+1] >= '0' && ptrr[j+1] <= '9') {
                j++;
            }
        } else if (ptrr[j] != '"'){//incase no number found, the word is not completed and chars are added to commandTemp
            commandTemp[t] = ptrr[j];
            t++;
        }
        if (ptrr[j] == '"'){
            j++;
            while (ptrr[j] != '"'){
                commandTemp[t] = ptrr[j];
                t++;
                j++;
            }
            commandTemp[t] = '\0';
            t++;
            j++;

            commandTemp[t] = '\0';//null is added to the end of the commandTemp
            command[k] = malloc(t * sizeof(char));//memory allocated for the word


            strcpy(command[k], commandTemp);//the word added to the allocated memory
            t = 0;
            k++;
            command[k] = '\0';
k++;
        }

    }

    free(ptrr);



 }
void  processExecNoPipe (char** command) {

    int pid = fork();//starting a parent and child processes
    if (pid < 0) {//incase of failure of starting child process
        perror("fork() error");
        exit (1);
    } else if (pid == 0)  {//child process number 1 to run
        execvp(command[0], (char *const *) command);//executing the input command
        perror("command not found\n");
        exit(0);
    }

}
void processExecWithPipe (char** command, char** command2) {

    int fd[2];
    if(pipe(fd)==-1)//exit if pipe werent created
        exit(1);

    int pid = fork();//starting a parent and child processes executes first command
    if (pid < 0) {//in case of failure of starting child process
        perror("fork() error");
        exit (1);
    } else if (pid == 0) {//child process number 1 to run
            close(STDOUT_FILENO);
            dup(fd[1]);//copies stdin pipe
//close both sides of the pipe
            close(fd[1]);
            execvp(command[0], (char *const *) command);//executing the input command
            perror("command not found\n");
            exit(1);

    }
    int pid2 = fork();//starting a parent and child processes executes second command
    if (pid2 < 0) {//incase of failure of starting child process
        perror("fork() error");
        exit (1);
    }
    else if (pid2 == 0) {//child process number 1 to run
            close(STDIN_FILENO);
            dup(fd[0]);//copies stdout
//close both sides of the pipe
            close(fd[1]);
            execvp(command2[0], (char *const *) command2);//executing the input command
            perror("command not found\n");
            exit(1);

    }

    //close the original pipe
    close(fd[0]);
    close(fd[1]);
wait(0);
wait(0);
}
void processExecWithTwoPipes (char** command, char** command2,char** command3 ) {

    int fd[2];
    int fd2[2];
    if(pipe(fd)==-1)//exit if pipe1 werent created
        exit(1);

    if(pipe(fd2)==-1)//exit if pipe2 werent created
        exit(1);

    int pid = fork();//starting a parent and child processes, executes command no1
    if (pid < 0) {//in case of failure of starting child process
        perror("fork() error");
        exit (1);
    } else if (pid == 0) {//child process number 1 to run
        dup2(fd[1],1);//copies the pipe1, write opened , read close
        //close all sides of the two pipes
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        execvp(command[0], (char *const *) command);//executing the input command no1
        perror("command not found\n");
        exit(1);

    }
    fflush(stdout);
    int pid2 = fork();//starting a parent and child processes, executes command no2
    if (pid2 < 0) {//incase of failure of starting child process
        perror("fork() error");
        exit (1);
    }
    else if (pid2 == 0) {//child process number 1 to run command no2
        dup2(fd[0],0);//copies the pipe1, write close , read open
        dup2(fd2[1],1);//copies the pipe2, write opened , read close
        //close all sides of the two pipes
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        execvp(command2[0], (char *const *) command2);//executing the input command no2
        perror("command not found\n");
        exit(0);

    }
    fflush(stdout);
    int pid3 = fork();//starting a parent and child processes, executes command no3
    if (pid3 < 0) {//incase of failure of starting child process
        perror("fork() error");
        exit (1);
    } else if (pid3 == 0) {//child process number 1 to run command3

        dup2(fd2[0],0);//copies the pipe2, write close , read open
        //close all sides of the two pipes
        close(fd[0]);
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]);
        execvp(command3[0], (char *const *) command3);//executing the input command no3
        perror("command not found\n");
        exit(0);

    }

    //close the original pipe
    close(fd[0]);
    close(fd[1]);
    close(fd2[0]);
    close(fd2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
}
int allocating (char* ptr, char input[]){

    int startPoint = 0; // start reading point after each word passed over
    int length = 0;//length of each word passed over
    char* ptr1= ptr;
    int i;
    int stringLength = strlen(input);
    int length1 = 0;
    int pipeNum = 0;;
    for (i = 0; i<stringLength; i++) {//loops over the sentence until end

        if(input[i] == '"'){
            int loop1 =0;
            char temp1[strlen(input)];
length++;
            temp1[loop1] = input[startPoint];
            loop1++;
            startPoint++;
            i++;

            while (input[i]!= '"'  ){
                temp1[loop1] = input[startPoint];
length++;
                loop1++;
                startPoint++;
                i++;
            }

            temp1[loop1] = input[startPoint];
            loop1++;
            startPoint++;
            i++;
length++;
        }

        if (input[i] != ' ' &&  input[i] != '\n' ){//increment length between spaces or new line
            length++;};
        if (input[i] == '|') {
            if(input[i+1]=='|') {
                input[i + 1] = ' ';
            }
            pipeNum++;

        }

        if ((input[i] == ' ' ) || i == stringLength-1 || input[startPoint] == '\n' || input[startPoint] =='\t' ) {//conditions of the competence of a full word to be passed to temp array

            int loop;

            char temp[strlen(input)];
            for (loop = 0;
                 input[startPoint] != '\t' && input[startPoint] != '\n' && input[startPoint] != ' ' &&
                 input[startPoint] != '\0' && startPoint != stringLength -
                                                            1; loop++) {//passing the elements of the temp array into the allocated array until one of the conditions is not met

                temp[loop] = input[startPoint];

                startPoint = startPoint + 1;

            }

            while (input[i] == ' ' && i < stringLength && input[i + 1] == ' ') {//skipping multiple spaces in a row in sentence
                startPoint++;
                i++;
            }

            length1 = length1 + length;//overall length of string inside allocated memory
            //in case string added to temp array, updates on variables takes place
                startPoint++; //steps over a single space
                strcpy(ptr, temp);//adding word to allocated memory
                ptr = ptr1 + length1;//re-allocating the pointer
                char lengthString[3];
                sprintf(lengthString, "%d", length);
                strcpy(ptr, lengthString);

                ptr = ptr1 + length1 + strlen(lengthString);
            length1= length1 + strlen(lengthString);

            length = 0;


        }
    }
    ptr = ptr1 + length1 + 1;
    strcpy(ptr, "\0");
    return pipeNum;
}
void inputForExecWithTwoPipe(char* ptrr, int stringLength,int count, char** command, char** command2,char** command3) {
    int k = 0;
    int t = 0;
    int j;
    int i=0;
    int w=0;
    char commandTemp[150];

    for (j = 0; j < strlen(ptrr) && ptrr[j]!= '|'; j++) {//loops until the the first Pipe

        if ((ptrr[j] >= '0' && ptrr[j] <= '9' && ptrr[j -1] != '-')) {//a number found, a word is completed

            commandTemp[t] = '\0';//null is added to the end of the commandTemp
            command[k] = malloc(t * sizeof(char));//memory allocated for the word
            strcpy(command[k], commandTemp);//the word added to the allocated memory
            t = 0;
            k++;

            while (ptrr[j + 1] >= '0' && ptrr[j + 1] <= '9') {
                j++;
            }
        } else  {//incase no number found, the word is not completed and chars are added to commandTemp
            commandTemp[t] = ptrr[j];
            t++;
            if(ptrr[j+1]== '|'){//command number one is completed
                commandTemp[t] = '\0';//null is added to the end of the commandTemp
                command[k] = malloc(t * sizeof(char));//memory allocated for the word
                strcpy(command[k], commandTemp);//the word added to the allocated memory
                t = 0;
                k++;
                command[k] = NULL;
                k++;
            }
        }
    }
    while((ptrr[j + 1] >= '0' && ptrr[j + 1] <= '9')){//incase length of the command is more than 1 digit
        j++;

    }
    k =0;
    t =0;
    for (i=j+1; i < strlen(ptrr) && ptrr[i]!= '|'; i++) {//loops until the end of the second command


        if (ptrr[i] >= '0' && ptrr[i] <= '9'&& ptrr[i -1] != '-') {//a number found, a word is completed
            commandTemp[t] = '\0';//null is added to the end of the commandTemp
            command2[k] = malloc(t * sizeof(char));//memory allocated for the word
            strcpy(command2[k], commandTemp);//the word added to the allocated memory
            t = 0;
            k++;

            while (ptrr[i + 1] >= '0' && ptrr[i + 1] <= '9') {
                i++;
            }
        } else {//incase no number found, the word is not completed and chars are added to commandTemp
            commandTemp[t] = ptrr[i];
            t++;
            if(ptrr[i+1]== '|'){
                commandTemp[t] = '\0';//null is added to the end of the commandTemp
                command2[k] = malloc(t * sizeof(char));//memory allocated for the word
                strcpy(command2[k], commandTemp);//the word added to the allocated memory
                t = 0;
                k++;
                command2[k] = NULL;
                k++;
            }
        }
    }
    while((ptrr[i + 1] >= '0' && ptrr[i + 1] <= '9')||ptrr[i+1] =='|') {
        if (ptrr[i+1] =='|' && ptrr[i+3] =='|'){//in case two pipes without a command in between is found, with spaces
           i++;
        }
        i++;
    }
    k =0;
    t =0;

    for (w=i+1; ptrr[w] != '\0'; w++) {//loops until the end of the third command

        while (ptrr[w] == ' ') {//shifts to the first filled element of "input"
            w++;
        }

        if (ptrr[w] >= '0' && ptrr[w] <= '9'&& ptrr[w -1] != '-') {//a number found, a word is completed
            commandTemp[t] = '\0';//null is added to the end of the commandTemp
            command3[k] = malloc(t * sizeof(char));//memory allocated for the word
            strcpy(command3[k], commandTemp);//the word added to the allocated memory
            t = 0;
            k++;
            if (ptrr[w+ 1] == '\0') {//if it is the las word then a null is added to the next element

                command3[k] = NULL;
            }
            if (ptrr[w + 1] >= '0' && ptrr[w + 1] <= '9') {
                w++;
            }
        } else {//in case no number found, the word is not completed and chars are added to commandTemp

            commandTemp[t] = ptrr[w];
            t++;
        }
    }
free(ptrr);

}
void  inputForExecWithPipe(char* ptrr, int stringLength,int count, char** command, char** command2) {//this function processes the input for it to be used as input for execvp() function
        int k = 0;
        int t = 0;
        int j;
        int i=0;
        char commandTemp[150];

        for (j = 0; j < sizeof(ptrr) && ptrr[j]!= '|'; j++) {//loops until the the first Pipe


            if ((ptrr[j] >= '0' && ptrr[j] <= '9' && ptrr[j -1] != '-')) {//a number found, a word is completed

                commandTemp[t] = '\0';//null is added to the end of the commandTemp

                command[k] = malloc(t * sizeof(char));//memory allocated for the word
                strcpy(command[k], commandTemp);//the word added to the allocated memory
                t = 0;
                k++;

                while (ptrr[j + 1] >= '0' && ptrr[j + 1] <= '9') {
                    j++;
                }
            } else if (ptrr[j] != '"' ) {//incase no number found, the word is not completed and chars are added to commandTemp
                commandTemp[t] = ptrr[j];
                t++;
                if(ptrr[j+1]== '|'){
                    commandTemp[t] = '\0';//null is added to the end of the commandTemp

                    command[k] = malloc(t * sizeof(char));//memory allocated for the word
                    strcpy(command[k], commandTemp);//the word added to the allocated memory
                    t = 0;
                    k++;
                    command[k] = NULL;
                    k++;
                }
            }

            if (ptrr[j] == '"'){
                j++;
                while (ptrr[j] != '"'){
                    commandTemp[t] = ptrr[j];
                    t++;
                    j++;
                }
                commandTemp[t] = '\0';
                j++;
                commandTemp[t] = '\0';//null is added to the end of the commandTemp
                command[k] = malloc(t * sizeof(char));//memory allocated for the word
                strcpy(command[k], commandTemp);//the word added to the allocated memory
                t = 0;
                k++;
                command[k] = '\0';
k++;
j++;
            }

        }

while (ptrr[j + 1] >= '0' && ptrr[j + 1] <= '9') j++;

        k =0;
        t =0;


    for (i=j+1;  ptrr[i] != '\0'; i++) {//loops until the end of the word;
    if ((ptrr[i] >= '0' && ptrr[i] <= '9' && ptrr[i -1] != '-')) {//a number found, a word is completed
        commandTemp[t] = '\0';//null is added to the end of the commandTemp
        command2[k] = malloc(t * sizeof(char));//memory allocated for the word
        strcpy(command2[k], commandTemp);//the word added to the allocated memory
        t = 0;
        k++;
        if (ptrr[i+ 1] == '\0') {//if it is the las word then a null is added to the next element

            command2[k] = NULL;
        }
        while (ptrr[i + 1] >= '0' && ptrr[i + 1] <= '9') {
            i++;
        }
    } else {//incase no number found, the word is not completed and chars are added to commandTemp
        commandTemp[t] = ptrr[i];
        t++;
    }
}
free(ptrr);
}
void clearPointer (char** command){
wait(NULL);
int len = strlen(*command);
    for (int w = 0; w <= len; w++) {//freeing command array of pointers to strings
        free(command[w]);
    }
    free(command);
}
