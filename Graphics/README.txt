The following project contains to applications:
1. Population progress - app for visualising a line chart of the celuar automata's participants' population flow.
2. Field state - app for visualising a current field state in a map format. 
Additional information will be provided in each app's folder.

Installation:

1. Install the following packages from the software center:
	- libsdl2-gfx-dev
	- libsdl2-ttf-dev

2. Clean the project - 'make clean' command
	Cleaning is required in order to delete files if any compiled for other machines.

3. Compile the project - 'make' command
	Compilation is required in order to create executables that will run on your machine.
	
4. Ensure valid fonts' URLs in the apps' configuration files!
	
- each library has an executable, which is a simple test.
	Run all tests to ensure that the libraries are working correctly.
	Correct tests' results will be provided in the libraries' folders.
- executables are typically named 'myapp'

- applications are parallelized regarding the Parallel Computing model. 
	Unfortunatelly the data file reading needs to be serialized and a graphic API can only be called from the main thread
		leaving only the matematical computing parallelized.
	Performance results show us that parallel execution is slower than serial 
		which means that aplying Parallel computing to such applications is ineffective.
	Distributed computing model might be a better idea 
		if we want to use multi-core system efficiently with such applications.