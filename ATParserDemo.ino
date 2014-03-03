/*-----------------------------------------------------------------------------
 
 Sub-Etha Software's AT Parser Demo
 By Allen C. Huffman
 www.subethasoftware.com
 
 This is an example of how to use the Sub-Etha Software Hayes AT Parser. To
 configure, edit "sesATParserConfig.h" as appropriate.
 
 2014-03-03 1.00 allenh - Created this demo program.
 
 CHECK BACK FOR UPDATES! Much more still to be done...
 -----------------------------------------------------------------------------*/



void setup()
{
  Serial.begin(9600);
  while(!Serial);

  Serial.println();
  Serial.println(F("AT Parser Demo"));

  showFreeRam();
  
  Serial.println(F("\n"
                   "Initially, we are in normal/data mode. Anything you type\n"
                   "will just be echoed back to the screen.\n"
                   "\n"
                   "To enter command mode, pause for one second, then send\n"
                   "+++ followed by another one second pause. (To do this in\n"
                   "the Arduino Serial Monitor, you must set it to 'No line\n"
                   "ending' to it won't automatically send a CR/LF after you\n"
                   "press Send. That keeps it from ever detecting a one second\n"
                   "pause after the +++. Even better, use a real terminal\n"
                   "program.)\n"
                   "\n"
                   "Once in command mode, send commands (change it back to\n"
                   "'Both NL & CR' so it sends ENTER after you type a command).\n"
                   "The only commands in this demo is 'ATD' and 'BYE'. 'BYE'\n"
                   "will exit back to normal/data mode, and 'ATD' is just an\n"
                   "example command that prints a message."
                   "\n"));
}

/*---------------------------------------------------------------------------*/

void loop()
{
  char ch;

  // If data is available...
  if (Serial.available()>0)
  {
    // Read it.
    ch = Serial.read();
    Serial.write(ch);
  } else {
    ch = 0; // If no data, pass in 0 to cmdModeCheck() for timing.
  }    

  // Pass current character (or 0 if none) to this function and it will do
  // all the timing/counting necessary to determine if command mode has been
  // activated.
  if (cmdModeCheck(ch)==true) cmdMode();

  // Else we keep processing...
}

/*---------------------------------------------------------------------------*/
// This would be your command mode parser. You can use the function
// "readCmdLine()" to read a line and return a NULL terminated C string.
// Then, you can parse that line and handle whatever commands you want.
// Eventually, code will be provided to parse standard "AT" commands and
// handle them, but for now, the only code provided is the "+++" escape
// sequence stuff.
#define CMDLINE_SIZE 80

void cmdMode()
{
  char  cmdLine[CMDLINE_SIZE];
  byte  len;

  Serial.println();
  Serial.println(F("-- command mode activated: 'BYE' to exit --"));

  while(1)
  {
    Serial.print(F("Enter command: "));
    len = readCmdLine(cmdLine, CMDLINE_SIZE);
    if (len>0)
    {
      // cmdLine[] is the command typed, NULL terminated.

      if (strncmp_P(cmdLine, PSTR("BYE"), 3)==0) break;

      if (strncmp_P(cmdLine, PSTR("ATD"), 3)==0)
      {
        Serial.println(F("-- connect command (dial) issued --"));
      }
    }
  } // end of while(1)

  Serial.println(F("-- leaving command mode --"));
}

/*---------------------------------------------------------------------------*/

unsigned int freeRam()
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void showFreeRam()
{
  Serial.print(F("Free RAM: "));
  Serial.println(freeRam());
}

/*---------------------------------------------------------------------------*/
// End of ATParserDemo

