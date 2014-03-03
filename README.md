sesATParser
===========

Hayes AT command parser.

NOTE: Currently, this does not actually parse AT commands. It only handles
the "+++" escape sequence to get in to command mode. This is all I needed
initially, but I will be adding a simple parser based on the Hayes modem
style commands. I wanted to post this in case anyone else wanted to work
with it until I get around to finishing my project...

REVISION
========
2014-03-03 allenh - Initial, hastily created README file.

FILES
=====

* README.md - this file
* ATParserDemo.ino - end-user demo program
* sesATParser.ino - the actual AT parser code
* sesATParserConfig.h - config for the AT parser

CONFIGURATION
=============

By default, the AT Parser is set to look for "+++" with a one second gaurd
time (pause in front, and after). It can be altered to look for other escape
sequences, and there is even some commented out code to make it look for
something other than a series of repeating characters. The normal Hayes
standard only allows for setting the escape character, which defaults to
"+", but you could customize it to be "***" with a three second pause before
and after.

```
// Define the escape sequence.
#define ESC_GUARD_TIME  1000 // Seconds required before/after escape sequence.
#define ESC_CHARACTER   '+'  // Default escape character.
```

RUNNING
=======

The demo program prints a message, then will just echo anything typed back to
the console. If there is a one second pause, followed by "+++" and another
one second pause, command mode will be enabled. (Note that by default, the
Serial Monitor in Arduino's IDE will always echo a CR/LF after anything
you send, thus it will be sending "+++CRLF" so the parser will not be
seeing a one second pause after the last "+". To get around this, you have
to turn the Serial Monitor to 'No line ending' so you can type the "+++"
at the top and press Send/Enter and have it sent without any CRLF after
it. Then, once you are in command mode, you can turn back on the CRLF so
you can type commands.)

Just a quick and dirty demo.
