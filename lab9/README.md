# Compilation Instructions

If using GCC, use the following command in the terminal:
```bash
gcc -o CS332 lab9.c # This will compile the filename and return an executable with the executable name.

gcc -o lab9 lab9.c # This will compile lab1.c and output an executable called lab9.

```
To execute the compiled program in the terminal use the following command:
```bash
./lab9
 ```

# My Files
## lab9.c

### main()
The main function starts by checking if the command-line arguments are provided correctly. Next, signal handlers are established for SIGINT (Interrupt a process from keyboard (e.g., pressing Control-C) and SIGTSTP (Interrupt a process to stop from keyboard (e.g., pressing Control-Z). Next, the program forks a child process. In the child process, it executes the command specified in the command-line arguments using execvp(). The parent process continues to the next step. The parent process waits for the child process to terminate. Next, it checks if the child process terminated normally. The function handles any errors that may occur before printing and exiting the program.

### sigint_handler() & sigtstp_handler()
These are signal handler functions; sigint_handler is called when the child process is interrupted, while sigtstp_handler is called when the child process is suspended.


# Github Repository
https://github.com/tatyanamk/CS332

# References
Lab 9 Workbook