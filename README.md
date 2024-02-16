**It is necessary to implement an interactive command
interpreter (some kind of shell analogue) under Unix OS control, which reads the command
line from the standard input in a loop, analyzes it and executes the appropriate actions.**

* *Conditions*


The following operations may be present on the command line:
They are listed in descending order of priorities (the priority is the same on one line)


1) | , > , >> , <

2) && , ||

3) ; , &

Parentheses are also allowed, which allow you to change the order of
operations.
An arbitrary number of spaces between
the words that make up the command line is also allowed.

* *The command line is parsed by the Shell according to the following rules:
 
`<Shell command>` →

`<Command with conditional execution>` 

{ [; | &] `<Shell command>`}{ ; | & }

`<Command with conditional execution>` →

`<Command>` { [&& | || ] `<Command with conditional execution>` }

`<Command>` → {`<Input/Output redirection>`}`<Conveyor>` |

`<Conveyor>`{`<Input/Output redirection>`} | ( `<Shell command>`)

`<Input/Output redirection>` →

{`<Input/Output redirection >` } `<Input/Output redirection>` |

{`<Input/Output redirection>`}`<Input/Output redirection >`

`<Input/Output redirection >` → ‘<’ file

`<Input/Output redirection>` → ‘>’ file | ‘>>’ file

`<Conveyor>`→ `<Simple command>` {‘|’ `<Conveyor>`}

`<Simple command>`→ `<the name of command><list of arguments>`

{X} – means that X may be missing;

[x|y] – this means that one of the options must be present : x or y

| - the description of the rules is the same as "OR"

pr1 | …| prN – Conveyor.

Each command is executed as an independent process. The Shell waits for the completion of the last command to check its status in the conditional execution command.
Pipeline completion code = the completion code of the last pipeline command. The rest
of the pipeline commands should also not become "zombies".**

* *Example*

***com1 && (com2; com3)***

Here, the com2 and com3 commands will be executed only if com 1 completes successfully.

***(com1; com2) &***

A sequence of com1 and com2 commands will be executed in the background.
Parentheses (), in addition to performing the grouping function, also perform the function of calling
a new instance of the Shell interpreter.

***ps; ls; (cd namedir; ls; ps) && ls && ps***

The first and last ls will work in the current directory, the ls in parentheses will be executed in the directory
the name dir, ps in parentheses should indicate the presence of an additional Shell process.
