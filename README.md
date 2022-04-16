# Mini-Shall-responds-to-up-to-two-pipes-commands
the program deals with command with up to two pipes. 
the program takes input from the user, arranges it in an allocated array with length of each word, where then the input is split into strings and each command is added to a separate allocated array.
arrays are passed to execvp() functions which executes the shell commands through "fork"ed number of processes according to number of pipes inserted.

in this excercise i reused Mini Shell code (published on my Git); allocating function has been taken from ex1, but this time taken from the mail body of ex1 into a while new function.

from Mini shell code (published on my Git):
1- function "inputForExecNoPipe" has been taken as is, and reused.old name is inputForExec.  
2- function "processExecNoPipe" has been taken from the main body and seperated into a different function for an easier use.
3- the previously mentioned functions has been used as base for the following functions with extra work added for meeting the purposes: 
     -processExecNoPipe base for 1- processExecWithPipe  2- processExecWithTwoPipes 
     -inputForExecNoPipe base for 1- inputForExecWithPipe 2- inputForExecWithTwoPipe
