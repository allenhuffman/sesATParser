/*---------------------------------------------------------------------------*/
/* 
 Hayes AT Command Parser
 
 Demonstrates monitoring for "+++" escape sequence to enter Command Mode.
 
 This software was written to...
 
 References:
 
 http://en.wikipedia.org/wiki/Hayes_command_set
 http://playground.arduino.cc/Code/TimingRollover
 
 TODO:
 Make an echoPrint() function that will only print if in half duplex mode.
 
 2013-03-28 0.0 allenh - Initial version.
 */
/*---------------------------------------------------------------------------*/

#include "sesATParserConfig.h"

// Define the escape sequence.
//#define ESC_GUARD_TIME  1000 // Seconds required before/after escape sequence.
//#define ESC_CHARACTER   '+'  // Default escape character.

/*---------------------------------------------------------------------------*/
// Globals.

// For Command Mode.
unsigned int escGuardTime = ESC_GUARD_TIME; // Delay before/after esc sequence.
static char  escCharacter = ESC_CHARACTER;  // Escape character

/*---------------------------------------------------------------------------*/
// Process some Hayes modem style "AT" commands.
#define CR           13
#define BEL          7
#define BS           8
#define CAN          24

byte readCmdLine(char *cmdLine, size_t len)
{
  char    ch;
  byte    cmdLen = 0;
  boolean done;

  done = false;
  while(!done)
  {
    //ledBlink();
    if (Serial.available()>0)
    {  
      ch = Serial.read();
      switch(ch)
      {
      case CR:
        Serial.println();
        cmdLine[cmdLen] = '\0';
        done = true;
        break;

      case CAN:
        Serial.println("[CAN]");
        cmdLen = 0;
        break;

      case BS:
        if (cmdLen>0)
        {
          Serial.write(BS);
          Serial.print(" ");
          Serial.write(BS);
          cmdLen--;
        }
        break;

      default:
        // If there is room, store any printable characters in the cmdline.
        if (cmdLen<len)
        {
          if ((ch>31) && (ch<127)) // isprint(ch) does not work.
          {
            Serial.print(ch);
            cmdLine[cmdLen] = toupper(ch);
            cmdLen++;
          }
        }
        else
        {
          Serial.write(BEL); // Overflow. Ring 'dat bell.
        }
        break;
      } // end of switch(ch)           
    } // end of if (Serial.available()>0)
  } // end of while(!done)

  return cmdLen;
}

/*---------------------------------------------------------------------------*/
// Magic numbers.
#define ESC_TIMES       3    // Number of escape characters ("+++").

boolean cmdModeCheck(char ch)
{
  static unsigned long escCheckTime = 0; // Next time to check.
  static byte          escCounter = 0;   // Number of esc chars seen.

  // If no character is being passed in, we are just doing a check to see if
  // we are in a "wait for end guard time" mode.
  if (ch==0)
  {
    // See if we are waiting to enter command mode.
    // if (escSequence[escCounter]=='\0')
    if(escCounter==ESC_TIMES)
    {
      // Yep, we have already found all the escape sequence characters.
      if ((long)(millis()-escCheckTime) >= 0)
      {
        // And the pause has been long enough! We found an escape sequence.
        escCounter = 0;
        escCheckTime = millis()+escGuardTime;

        return true; // Yes, it is time for Command Mode.
      }
    }
  }
  else // if (ch==0)
  {
    // If there has been a pause since the last input character...
    if ((long)(millis()-escCheckTime) >= 0)
    {
      // Check to see if it's an escape byte.
      // if (ch==escSequence[escCounter])
      if (ch==escCharacter)
      {
        // Move to next character to look for.
        escCounter++;

        // Are we out of escape characters to check for?
        // if (escSequence[escCounter]=='\0')
        if (escCounter>=ESC_TIMES)
        {
          // Set after delay to signify end of escape sequence.
          escCheckTime = millis()+escGuardTime;
        }  
      }
      else
      {
        // Reset. Not an escape character.
        escCounter = 0;
        escCheckTime = millis()+escGuardTime;
      }
    }
    else
    {
      // Reset. Not an escape character.
      escCounter = 0;
      escCheckTime = millis()+escGuardTime;
    }
  } // end of if (ch==0) else

  return false; // No, it is not time for Command Mode.
}
/*---------------------------------------------------------------------------*/

// End of file.
