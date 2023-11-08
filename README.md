# Activity 22: Processes and Signals
## Put your name(s) here

In this activity, you will examine how to "fork" a new process from a program,
work with the other related system calls, and learn how C programs can manage
signals send by the OS.

## Provided Code

You start with:
- `Makefile`
    - a makefile to automate the compilation process
- `fork_example.c`
    - A demo program showing how to use `fork` to create a child process
- `exec_example.c`
    - A demo program showing how to use `exec` and `wait` 
- `exec_helper.c`
    - A simple helper program that prints out its own process id, along with counting
    up from 0 to 99, pausing one second in between
-  `exec2.c`
    - An optional second example with `exec` that looks at multiple processes
    and the interleaving of steps
- `sig_example1.c`
    - A demo program showing how to set up signal handlers and respond to signals
- `sig_example2.c`
    - A demo program showing how a parent process can respond to the SIGCHLD
    signal when a child process ends

## Your Tasks

Be sure to read all code files before compiling them with `make`, to be sure you
understand what they are doing.

### Task 1: The `fork` system call

- Look at the code in `fork_example.c`. This program has the main process fork
a child process, then each process reports on the returned value from `fork` and
its own process id (pid). Make sure what it says makes sense to you
- Compile and run this program. Run it several times. Can you see any change in
the order of the outputs?
- Modify the code so that each process sleeps for a random amount of time.
    - In each part of the second `if` statement, add the lines below to set a
    random generator seed different from the other process, generate 
    a random number between 0 and 2, and then sleep for that amount of time:

    ```
    srand(time(NULL) ^ mypid);
    r_val = rand() % 3;
    sleep(r_val);
    ```

    - Also, add a print statement in each part to print the random value (indenting
    the child process the same way as the existing print statememt)
    - Run this, multiple times. What do you notice?

**Care and caution here!** You need to be careful about using fork to create new processes. If your parent process doesn't ensure that the child quits, you can leave processes running on the server. In fact, if you are not really careful, you can
"fork-bomb" the server, creating more and more processes until you bring the server down.
**Do not do this!**

For now, don't create more than one child process in your code. And after you run a program that
uses `fork`, type `ps` in the terminal, and make sure that no process is still running!

**Be sure to check for stray processes after you are done with this step!**

### Task 2: The `exec` and `wait` system calls

The `exec` family of system calls allows a process to change the program it is running.
It is commonly used to allow a forked child process to run something other than
its parent's executable.

- Look at the `exec_helper.c` program. This just prints out the numbers from 0 to 99,
sleeping one second in between each.
- Next, read through `exec_example.c`. This program forks a child process, then has
it execute `exec_helper`. The parent process prints an end message and the quits.
- Notice that the child process continues even though the parent has ended, and you 
have the terminal prompt back. Type `ps` and see that it is still there.
- Notice that you can't stop the child process with ctrl-c, and you can't use `fg` to
bring it into the foreground.
- Use `kill` followed by the process id you see when you type `ps` to kill the child process
- Change the `exec_helper` program so that it only prints numbers from 0 to 9, or similar
- Run the programs again, and notice that `exec_helper` will end when it reaches
the natural end of its loop!

The `wait` system call, provided in the `sys/wait.h` library, allows the parent process
to wait until (one of) its child process(es) ends, and to see the status of that process.

- Examine the helper function `wait_for_child` in `exec_example.c`. This function 
prints a message, and then waits until a child process exits. It then checks the 
bits of the `status` variable using built-in macros to find out how it ended.
- Add a call to `wait_for_child` to the parent process's part of the `if` statement.
- Try it out and see what happens when the child process ends normally
- Try a riskier proposition:
    - Set the loop in `exec_helper` back to looping 100 times
    - Remake everything
    - Call `exec_example &`, which will run the parent process in the background,
    so you can access the terminal while both programs are running
    - Type `ps`: you should see both `exec_example` and `exec_helper` running
    - Use `kill` or `kill -9` with the process id for `exec_helper` to stop the
    child process
    - Notice what happens with the parent process in that case!

**Be sure to check for stray processes after you are done with this step!**

### Optional Task 2.5: Extra `fork`-`wait` example

This example shows again how processes can interleave so that the exact
ordering is not predictable.

- Read through `exec2.c` and make sure you understand it. Discuss with your 
teammates what you think might happen, and which print statements can occur in
which orders
- Run the program multiple times. Can you see the different orderings?
- If not, copy the code for generating a random delay from the `fork_example`
and have each of the three processes wait a random amount of time before ending

**Be sure to check for stray processes after you are done with this step!**

### Task 3a: Playing with Signals

We are going to experiment with how programs can handle signals, and use
the `kill` shell command to send different signals to a running process.
We will look at two code examples that set up "signal handlers," functions
that can be registered with the OS to be called when a signal arrives.

- Read through `sig_example1.c`. It defines three functions: 
    - `sigalarm_handler` responds to the SIGALRM signal by printing a message, 
    and then queueing up another alarm to run in 5 seconds
    - `sigcont_handler` responds to the SIGCONT signal: the "continue" signal 
    that is normally sent when you restart a paused process
    - `sigint_handler` responds to the SIGINT signal: the "interrupt" signal 
    the is sent when you type ctrl-c in the terminal
- The `main` function assigns each handler to the correct signal, doing some
error-handling in case the assignment fails. It then enters a loop where it 
prints some information about sending signals, and then pauses until a signal
comes in
- Notice that the SIGINT handler prints a message but lets the process continue,
while the SIGCONT handler prints a message and then calls `exit` to end the process.
- Make and run this program directly in the foreground: `./sig_example1`
- Try typing ctrl-c to see what it does
- Try typing ctrl-z to pause the process, then `fg` in the terminal to send the
"continue" signal
- Next, try running the program in the background: `./sig_example1 &`
- Now you have access to the terminal directly. Use the `kill` commands printed
by the program itself to send the two signals, and see how it reacts.
- Modify the program by moving the call to `exit` from the SIGCONT handler to
the SIGINT handler, *or* by removing it altogether. What happens?

**If you need to kill `./sig_example1`** you can use the `kill` command and send
the -9 or -KILL signal, which will cause the process to end!

### Task 3b: Signals and Multiple Processes

- Look at the code for `sig_example2.c`. This combines signal-handling with 
a forked child process (or more than one, if you're brave).
- Update/reset the `exec_helper.c` code so that it counts up to 50, sleeping
one second between each print
- Notice that the parent process, `sig_example2.c` will loop from 100 down to 0,
sleeping one second between each print, after getting its child process going
- Make and run this code once you understand it. Watch how the parent and child
processes interleave, and watch what happens when the child finishes
    - You can make the number of iterations less to save time

**Are you feeling brave and careful, all at once?**

Then try having the parent process spawn more than one child!
- Reduce the number of repetitions completed by `exec_helper` and the parent
process of `sig_example2.c`
- Change the `else` clause that controls what the parent process does after
forking its child so that it is a separate `if` statement:

    ```
    if (pid > 0) {
    ```

This separates the forking of the child and its behavior from the behavior of the parent.

- Wrap the lines that fork and exec the child process in a `for` loop, initially 
looping just twice, so that it forks and execs two separate child processes

- Test this, carefully! Make sure that all three processes end cleanly!

 - Once it works for 2, try it for 3 or 4 child processes!

- **Remember to check with `ps` whether stray processes exist, and use `kill` 
to shut them down if needed!**

- You could also have each process pause randomly rathern that for exactly one
second between each print, just to experiment!



## References

- fork, exec, wait, and exit
    - [*Dive into Systems*, Section 13.2.3](https://diveintosystems.org/book/C13-OS/processes.html)
- Time library in C
    - [C Programming/POSIX Reference/unistd.h](https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/unistd.h)
    - [IBM Docs on sys/types.h](https://www.ibm.com/docs/en/zos/3.1.0?topic=files-systypesh-typedef-symbols-structures)
    - [IBM Docs on sys/wait.h](https://www.ibm.com/docs/en/zos/3.1.0?topic=files-syswaith-hold-processes)
