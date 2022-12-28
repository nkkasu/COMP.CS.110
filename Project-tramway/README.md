## Tramway project

CLI for Tramways and stops that are read from file or added separately.

User gives a file name that program tries to open. If text file is
not found, program ends into EXIT_FAILURE. If text file is found,
information in file is parsed into a data structure. Information is
only parsed into a data structure, if information is in the right
format in the text file. If format of any of the lines is incorrect,
program ends into a EXIT_FAILURE. After file is read and put into a
data structure, command interface will be run. User can give different
commands in the command interface, such as Lines, stops, Line <linename>
Distance <line> <stop1> <stop2>, Stops, STOP <stopname>, Stop <stopname>
addline <linename>, addstop <linename> <stopname> remove <stopname>.
Longer stop/line names can be put around quote marks and will be handled
as one stop/line. Program will check that user input is in a correct format
and user gives right input values. If values are incorrect, program will
print accordingly. If values are correct, commands are processed and put
into different functions depending on command. Functions do different things
depending on command. Command can print all lines for example. User is prompted
of commands until user gives command QUIT. If user gives quit command program is
terminated with EXIT_SUCCESS.

![image](https://user-images.githubusercontent.com/100607632/209876385-e555b27b-6873-4fdf-be72-c93ec8cb18d0.png)
