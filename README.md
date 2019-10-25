# Simple Shell
A simple UNIX shell program written in C using system calls to interact with the kernel. The program uses the `fork/exec` C sytem calls to spawn child processes and execute commands.

## Programming Language

> ### C

## List of Commands made:
  - `cd` : **C**hange **D**irectory command [identical to the bash cd command].
  - `pwd` : **P**rint **W**orking **D**irectory command [identical to the basd pwd command].
  - `exit` : Exits the shell.

## Built-in Commands

| Command | Arguments | Syntax (Example) | Description |
   | --- | --- | --- | --- |
   | ` cd <dir> ` | __Directory name__ | ` cd myDirectory ` | Changes current directory to the one specified. [` .. ` for previous directory] |
   | ` pwd ` | __no arguments__ | ` pwd ` | Prints current workin directly to console |
   | ` exit ` | __no arguments__ | ` exit ` | prints an exit message and eits the shell |
   
> #### NOTE: Shell can run other terminal commands as well.
