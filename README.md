# synth-in-c

To use this program, you may compile and run the file Synth_interactive_driver.c
(I've tested this project with gcc, just in case any other compiler fails)

After opening the program, a table of notes and their frequencies will be printed

The program is interactive and the instructions are printed next to the options

To add notes to your composition, select 0
  -Then enter a bar (each bar is 2 seconds long). You must enter a single integer. The first bar will be 0
  -Then enter an index (this is a long float, you must enter a number between 0 and 1)
  -Then choose a frequency. A table for frequencies will already be printed above the options

To export your composition, select 7. The program will create a file called output.wav
You can casually double-click the file to hear it

Harmonize (9) is a pretty cool command. Self explanatory. Try it out!

The rest of the commands are for inspecting and diagnosing your masterpiece

This program is mindful of memory. However, to release the memory occupied by your composition please exit the program using the command 10

There are a couple musical pieces included that you can test the program with 

Example for opening one of the files:
  /> 6
  /> minuet.txt
  //                  We have read from the file into our program
  /> 7
  //                  We have exported the composition to output.wav
  /> 10
  //                  We have exited the program and cleared our memory
  Run output.wav
  -delightful music playing
