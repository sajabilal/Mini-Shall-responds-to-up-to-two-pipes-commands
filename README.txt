Mini Shell part2
Authored by Saja abu Aisheh
308444223

==Description==

the program deals with command with up to two pipes. 
the program takes input from the user, arranges it in an allocated array with length of each word, where then the input is split into strings and each command is added to a separate allocated array.
arrays are passed to execvp() functions which executes the shell commands through "fork"ed number of processes according to number of pipes inserted.

in this excercise i reused ex1 and ex2a; allocating function has been taken from ex1, but this time taken from the mail body of ex1 into a while new function.

from ex2:
1- function "inputForExecNoPipe" has been taken as is, and reused.old name is inputForExec.  
2- function "processExecNoPipe" has been taken from the main body and seperated into a different function for an easier use.
3- the previously mentioned functions has been used as base for the following functions with extra work added for meeting the purposes: 
     -processExecNoPipe base for 1- processExecWithPipe  2- processExecWithTwoPipes 
     -inputForExecNoPipe base for 1- inputForExecWithPipe 2- inputForExecWithTwoPipe

==functions== 

void inputForExecNoPipe(char* ptr, int stringLength,int count, char** command)
transforms string in ptr from string with nums into separate commands and flags in array called command

void processExecNoPipe (char** command)
execution of a command without a pipe

void processExecWithPipe (char** command, char** command2)
execution of two commands with one pipe

void processExecWithTwoPipes (char** command, char** command2, char** command3)
execution of 3 commands with two pipes

int allocating (char* ptr, char input[])
rranging the input as strings and lengths into allocated memory

void  inputForExecWithPipe(char* ptrr, int stringLength,int count, char** command, char** command2)
transforms string in ptr from string with nums and one pipeinto separate commands in array called command and command2, each command in an array

void  inputForExecWithTwoPipe(char* ptrr, int stringLength,int count, char** command, char** command2, char** command3)
transforms string in ptr from string with nums and two pipes into separate commands in array called command and command2 and command3, each command in an array

void clearPointer (char** command)
frees allocated memory after usage is completed

==no files used== 

==how to compile== 
compile: gcc ex3.c -o ex3 
run: ./ex3


==input== 
a sentence of maximum length of 512, which has command seperated with pipes and spaces and flags 

==output=== 
each round : 
output of the inserted shell command (with or without pipes)

final round :
1-number of inserted commands
2-overall number of pipes


