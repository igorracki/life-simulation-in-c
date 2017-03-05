Population progress app.
This app displays a line chart 
	representing the population flow of the participants 
	in the Celuar automata simulation.

Compile using 'make' command.
Run command: ./myapp <data_file> <config_file> [<thread_count>]
<data_file> - file , containing population data set of the participants for each moment.
<config_file> - file, containg graphic/screen/picture properties
The following paragraphs reveals files' formats.

data_file:
<participants_types_count N>
<one_dataset_length L>
<participant_type_1_name>
<participant_type_1_dataset> - L integers separated with ' '
...
<participant_type_N_name>
<participant_type_N_dataset> - L integers separated with ' '

config_file:
windowWidth=<integer>
windowHeight=<integer>
padding=<integer> - padding between main graphic objects
agendaWidth=<integer> - approximate width of the agenda
pointsSize=<integer>
fontSize=<integer>
fontURL=<OS_font_URL> - SDL needs URL to an existing system font in order to draw a text
maxY=<double>
xMainTickStep=<double>
yMainTickStep=<double>
secTickStepDivisor=<integer>
backgroundColor=<r,g,b>
textColor=<r,g,b> - color of main graphic objects
linesColors=<color_count N> - lines colors
	<r,g,b color 1>
	...
	<r,g,b color N>	
	
File examples:
	- data.txt
	- chartConfig.txt