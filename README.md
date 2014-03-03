sesATParser
===========

Hayes AT command parser.

This code is currently not stand-alone, but it is used with the
sesTelnetServer so I wanted to post it here.

To use, basically you have to call a function inside your main
loop that reads each byte/character:

if (Serial.available()>0)
{
  ch = Serial.read();
  if (cmdModeCheck(ch)==true) cmdMode();

  // process 'ch' as desired
}
	
cmdModeCheck() will keep track of what has been coming in, looking for a
pause and then the escape sequence ("+++", configurable), followed by
another pause. If the conditions are matched, it returns true and you
can call your cmdMode() function to handle commands, and when you exit
it returns back to normal operation.

Examples will be provided later...
	