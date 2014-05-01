
/*
 *                          #######################
 *                          ## PIR Booby Trap    ##
 *                          ## Processing-Client ##  
 *                          ## V1.0.0            ##                      
 *                          #######################
 *
 * This is the processing end of code that is intended (with Arduino code 
 * to allow us to look for movement in a room using any 3 pin PIR (5V, OUT GND) 
 * and then announces to whatever is listening to our serial via a 'Shout' 
 * (look for this string in serial reads).
 * 
 * It then sleeps for any given amount of time before starting to look 
 * for further movement in that space
 *
 * I used this code to setup a booby trap on the pc end, via included 
 * (hopefully) Arduino code to play a sound when it sees a 'Shout' 
 * (literally this string) over serial.
 *
 * N.B. THIS USES THE Ess LIBRARY. Find this on the processing libraries
 * section. http://processing.org/reference/libraries/
 *
 * @author Rob Hendricks / RobstaHendricks@gmail.com
 * @date 29th February 2012 (Yes, that's a leap year :] )
 *
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 3.0" license
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 * This end of the code could be edited to do anything we wish when movement is
 * found, but please accredit the original code even if modified. :]
 *
 * ENJOY.
 *
 * Robsta :]
 *
 * ## Change Log
 *
 * V1.0.0
 *
 * Initial code completion
 *
 */

//Import the Serial and Audio libraries (should be in the relevant sketchbook
// 'libraries' folder.
import processing.serial.*;
import krister.Ess.*;

//Want to log info? Keep all 'Log' sections
//else, they're just for file logging...
PrintWriter pwLog;
String sLogFile = "Log.txt";
String sLog = "";

//The Serial Port we'll listen for 'shouts' on
Serial port;

//The info that we get over Serial
String sSerialRead; 

//Lets label exactly what will be 'shouted' so we can configure this
//if necessary, must match if using my arduino code at both ends.
String sShoutString = "Shout";

//This is for playing back the MP3 file
AudioChannel cAudioChannel;

//Set name of the MP3 file, should be in 'PIR_Shout/data' folder.
String sAudioFile = "buzzsaw.mp3";

//Mainly DEBUG but we can count how many movements have occured.
int iShoutCount = 0;

void setup()  
{
  //Make a big box for visual feedback
  size(500, 500);
  frameRate(10);

  pwLog = createWriter(sLogFile);

  // Open the port that the board is connected to and use the same speed 
  // (9600 bps in our case)

  //The port for my machine seems always to be 0
  int iSerialPortNumber = 0;

  //Lets see what devices are connected and print to console
  println(Serial.list());

  //Haven't worked out a .ToString on Serial.List enumeration yet
  //Log(sLog);

  //Set the port to our correct port number, like I said, mine's 0
  String sPort = Serial.list()[iSerialPortNumber];

  //Open the port 
  port = new Serial(this, sPort, 9600);

  //Start the Ess audio library
  Ess.start(this);

  // lets open our mp3 file (included in the 'data' folder of the project
  cAudioChannel = new AudioChannel(); 
  cAudioChannel.loadSound(sAudioFile);
}

void draw()
{  

  //Do we have an available serial feed?
  if (0 < port.available()) 
  {  
    //a Serial feed is available

      //Read serial info until a new line is found ('\n' is a new line)
    sSerialRead = port.readStringUntil('\n');

    //Check whether we found real String info, avoid null ref exceptions
    if (sSerialRead != null)
    {
      //This line is super important, I (at least) cannot get stuff working
      //without this line.
      sSerialRead = trim(sSerialRead);

      //DEBUG - Display ANY serial comms
      //Careful though, this also displays calibration info.
      //I'd leave in, and control from the Arduino end what to display.
      // (additional debugging options on Arduino end.
      sLog = sSerialRead;
      println(sLog);
      Log(sLog);
    }
  }

  //This is the visualisations pane's background colour.
  background(255);   

  //Check whether we found real String info, avoid null ref exceptions
  if (sSerialRead != null)
  {
    //Will check for the 'shout' string being announced. This has to match
    //to the Arduino code if you are using my example else we will never see
    //anything.
    //
    //N.B. We use String.equals("String to compare") as String is an object
    //and the == comparison would compare to object type String, rather than
    //the actual contents of the String
    if (sSerialRead.equals(sShoutString)) 
    {
      iShoutCount++;

      //DEBUG - keep count of 'shouts'.
      sLog = "Shouted " + iShoutCount + " times";
      println(sLog);
      Log(sLog);

      //DEBUG - exact info as to when a shout occured (we have a real clock at
      //this end
      sLog = nf(hour(), 2) + ":" + nf(minute(), 2) + ":" + nf(second(), 2) + " " + nf(day(), 2) + "//" + nf(month(), 2) + "//" + nf(year(), 2);
      println(sLog);
      Log(sLog);

      //DEBUG - soft info, but yea we heard a 'shout'.
      sLog = "SHOUT ANNOUNCED!"; 
      println(sLog);
      Log(sLog);

      //Again kinda DEBUG - visualises a shout if audio muted.
      fill(0);

      //Lets freak out who ever moved! 
      //Plays our MP3 sound file
      cAudioChannel.play();
    }
    else
    {
      //Basically DEBUG stuff to visualise no movement

      //Make on screen rectangle White
      fill(255);
    }
  }  

  //Reset sSerialRead to prevent looping! Important, as we may not get a readout
  //for some time.
  sSerialRead = "";

  //DEBUG - Kinda irrlevant other than to display some kind of visual 
  //recognition so we can test without sound on.
  rect(50, 50, 400, 400);
} 

//Segregated logging function
public void Log(String sLog)
{
  try
  {
    //Logging to file
    pwLog.println(sLog);
    pwLog.flush();
  }
  //  catch()
  //  {
  //    //Later
  //  }
  finally
  {
    //Later
  }
}

//Cleanup stuff!
public void stop() 
{ 
  //Close logging file
  pwLog.close();

  //Close audio lib
  Ess.stop(); 

  //Cleanup
  super.stop();
} 

