Eric Aguirre
Alexander Taliaferro

# rshell
## Synopsis

Rshell is a basic command shell written in C++ that **prints a prompt** 
containing currently-logged-in user at the current hostname, **reads commands** 
given in a single line(**including connectors: ;, &&, ||, #**), 
and has a built in **exit** function.

## Tests

single_command.sh #tests single commands
multi_command.sh #tests commands with ;, &&, or ||
commented_command.sh #tests commands with comments
exit.sh #tests exit and commands with exit

## List of Known Bugs

Running with a shell script leads to out put being placed after the command prompt
but still runs correctly

## Contributors

eagui020 - Eric Aguirre
atali001 - Alexander Taliaferro

## License

GPL 3.0
