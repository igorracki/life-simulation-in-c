Field state app.
This app displays graphically state of an environment 
	represented by simple char matrix.

Compile using 'make' command.
Run command: ./myapp <data_file> <config_file> [<thread_count>]
<data_file> - file , containing char matrix with its properties. The matrix represent a real environment with objects.
<config_file> - file, containg graphic/screen/picture properties
The following paragraphs reveals files' formats.

data_file:
<object_types_count N>
<object_type_1_name> <object_type_1_char>
..
<object_type_N_name> <object_type_N_char>
<field_height H> <field_width W>
<H lines of W chars>

config_file:
windowWidth=<integer>
windowHeight=<integer>
padding=<integer> - padding between main graphic objects
agendaWidth=<integer> - approximate width of the agenda
fontSize=<integer>
fontURL=<OS_font_URL> - SDL needs URL to an existing system font in order to draw a text
backgroundColor=<r,g,b>
textColor=<r,g,b> - color of main graphic objects
objectColors=<color_count N> - dots colors
	<r,g,b color 1>
	...
	<r,g,b color N>
	
File examples:
	- data.txt
	- mapConfig.txt