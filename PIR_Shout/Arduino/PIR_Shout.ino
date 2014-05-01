
/*
 *                          ####################
 *                          ## PIR Booby Trap ##
 *                          ## Arduino-Client ##  
 *                          ## V1.0.0         ## 
 *                          ####################
 *
 * This code is intended to allow us to look for movement in a room
 * using any 3 pin PIR (5V, OUT GND) and then announces to whatever is
 * listening to our serial via a 'Shout' (look for this string in serial
 * reads).
 * 
 * It then sleeps for any given amount of time before starting to look 
 * for further movement in that space
 *
 * I used this code to setup a booby trap on the pc end, via included 
 * (hopefully) processing code to play a sound when it sees a 'Shout' 
 * (literally this string) over serial.
 *
 * TO UNDERSTAND CODE UNCOMMENT ALL DEBUG RELEVANT CODE.
 *
 * @author Rob Hendricks / RobstaHendricks@gmail.com
 * @date 29th February 2012 (Yes, that's a leap year :] )
 *
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 3.0" license
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 *
 * The PIR sensor was bought on ebay. It seems that there are a lot of 
 * cheap components on there that, if you are willing to wait, may be
 * cheaper than buying from the Arduino direct distributors.
 *
 * For guidance purposes only, my PIR took 3 weeks to ship from a Hong Kong
 * auctioneer to England, though I have ordered other items that took a lot
 * less time to arrive
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

//set up the LED pin and PIR pin
int ledPin = 13;
int PIRPin = 12;
int iPIRValue = 0;

//These are used to prevent us shouting all the time
boolean bShouting;
boolean bShouted;

//We will delay the calibration to give a chance to move away from the sensor
int iDelayCalibration = 5;

//iShoutCount is used to prevent us constantly shouting
//That we have seen movement - Leave this as 0
int iShoutCount = 0;
//iShoutLimit is the amount of seconds after sensing movement that
//we will ignore all movement sensed - Change this to how ever many
//seconds you want to ignore movement after sensing it.
//Currently once a minute!
int iShoutLimit = 60;

//The PIR takes time to calibrate to it's surroundings.
//30 should do, but the more the better
int iCalibrationTimeout = 40;

//Lets label exactly what will be 'shouted' so we can configure this
//if necessary, must match if using my processing code at both ends.
String sShoutString = "Shout";

void setup() 
{
  //We want to allow others to listen for our movement
  Serial.begin(9600);
  
  //We'll be checking for current Serial info, so clear any serial info before
  //beginning!
  Serial.flush();

  //This is used to print a count down to allow us to move 
  //away from the PIR before it starts callibrating
  int iCount = iDelayCalibration;
  
  //Let our listeners know that they need to wait for
  //the PIR to calibrate and start a count down
  //The Serial.println can become DEBUG also, but keep timers!!!
  Serial.println("Lets get outta this room.");
  for(int i = 0; i < iDelayCalibration; i++)
  {
    delay(1000);
    Serial.println(iCount);
    iCount--;

  }

  //Set up or pins
  pinMode(ledPin, OUTPUT);
  pinMode(PIRPin, INPUT);


  //This is used to print a count down during calibrating
  //the PIR to be picked up by what ever is listening to
  //our serial
  iCount = iCalibrationTimeout;

  //Let our listeners know that they need to wait for
  //the PIR to calibrate and start a count down
  //The Serial.println can become DEBUG also, but keep timers!!!
  Serial.println("Calibrating Sensor");
  
  for(int j = 0; j < iCalibrationTimeout; j++)
  {
    delay(1000);
    Serial.println(iCount);
    iCount--;

  }
  
  Serial.println("Calibration Complete\n");
  
  //Keep this delay if DEBUG, you'll prob not see that the 
  //calibration is complete if the DEBUG stuff below is all 
  //uncommented
  //The delay can become DEBUG also, when sure all is in installation
  delay(2000);

  //Ok, so now we want to start looking for movement so set these flags
  bShouting = false;
  bShouted = false;
}


void loop() 
{
  //Check the PIR for any movement, LOW = no movement, HIGH = something moved
  iPIRValue = digitalRead(PIRPin);

  //Have we just shouted that there was some movement?
  //If no, then this is new movement and we'll check the PIR's
  //Current state
  if(bShouted == false)
  {
    //We know that we are ready to check for more movement
    if (iPIRValue == HIGH) 
    {
      //Something moved!
      //We'll switch off the LED
      digitalWrite(ledPin, LOW);

      //If we want to wait a certain amount of seconds before 'shouting'
      //then we'll do it here, else set iShoutDelay = 0;
      int iShoutDelay = 2;
      delay(iShoutDelay*1000);

      //And we'll shout to anything listening that there was movement
      Serial.println(sShoutString);

      //And we'll set some logic up to prevent checking for more 
      //movement before the shout time limiter is up
      bShouting = true;
      delay(1000);
    } 
    else 
    {
      //Make sure the LED is on to indicate awaiting movement.
      digitalWrite(ledPin, HIGH);

      //DEBUG The below line is for use setting up or debugging the PIR
      //It simply prints a '.' every time we don't get a reading
      //AKA no movement sensed
      //Serial.println(".");

      //We can continue looking for movement at next iteration (start) of loop()
      bShouting = false;
    }
  }

  //Have we just sensed motion?
  if(bShouting)
  {
    //Yes, we'll start the shout time limiter
    if(bShouted == false)
    {
      //DEBUG, this marks the START of the counter that prevents 
      //the signal to be sent to what ever is listening for the shout 
      //more often than the shout limit
      //Serial.println("Loop Started - " + String(millis()/1000));

      //Ok, so we just made a shout, so lets not look for any movement for a while
      bShouted = true;
      bShouting = false;
    }
    else
    {
      //Nothing for now
    }
  }

  //Have we just shouted to whatever is listening for our shout?
  if(bShouted)
  {
    //Yes, begin the shout time limiter so we won't receive another shout
    //until we are ready
    if(iShoutCount < iShoutLimit)
    {
      //DEBUG, this is the counter that prevents the signal to be
      //sent to what ever is listening for the shout more often than
      //the shout time limiter
      //Serial.println(String(iShoutCount) + " Shouting = " + String(bShouting) + " Shouted = " + String(bShouted));
      
      //DEBUG Simple countdown 'til next 'shout' (the '-1' will display a 0 when time is up)
      //Serial.println(String((iShoutLimit - iShoutCount) - 1));
      
      iShoutCount++;
      delay(1000);
    }
    //We are ready to make another shout
    else
    {
      //DEBUG, this marks the END of the counter that prevents 
      //the signal to be sent to what ever is listening for the shout 
      //more often than the shout time limiter
      //Serial.println("Loop Ended - " + String(millis()/1000));

      //Logic to say that we can now look for more movement
      bShouting = true;
      bShouted = false;

      //Reset the Shout time limiter
      iShoutCount = 0;

      //More DEBUG to state whether we are shouting, or have shouted
      //Serial.println("Shouting = " + String(bShouting) + " Shouted = " + String(bShouted));
      delay(1000);
    }

  }
}


