App Launcher

The application launcher is split into two processes. The first process is launchd. This process
is always running and is responsible for launching the launcher UI and applications. 
The launcher UI is a QT/Webkit process that shows graphical icons for each application. The
user can select an application to launch it. The UI will send a launch command to launchd and 
then shutdown to conserve memory resources. 

Commands are sent to launchd via a unix fifo. The fifo (/tmp/launchd) allows for the UI proces to 
communicate with launchd. Currently only two commands exist. They are listed below:

Command 0: Shut down - This command will tell launchd to exit.
Command 1: Launch application - This command will tell launchd to load and run an application.

All commands follow a common structure. They are formated as ASCII strings with comma seperated
fields and ends with a '\0'. All commands have at least two common fields but can have more depending
on the command. The first field is always the string "CMD" and allows for synchronization. The 
second field is always the command number, i.e. 0 for shut down or 1 for launch app.

Launch Application Command

The launch app command has additional fields beyond the required 2 fields. There described as follows:

Name - Name of the application. (For informational use only)
Command - The actual command to execute.
Path - Directory path the launch the command in.
Number of Arguments - the number of argument fields that follow this field.
Arguments - A variable number of fields that represent the arguments to pass to the command.

Limitations:

The follow list are the current limitations of the app launcher.

1. Only one application can be run at a time. Conncurrent applications are not supported due
   to memory constraints.
2. Only DirectFB based application can be launched. All apps are expected to take control
   of the graphical display.


