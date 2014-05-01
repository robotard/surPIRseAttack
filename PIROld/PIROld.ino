
//set up the LED pin and PIR pin
int ledPin = 13;
int PIRPin = 12;
int iPIRValue = 0;

//These are used to prevent us shouting all the time
boolean bShouting;
boolean bShouted;

//iShoutCount is used to prevent us constantly shouting
//That we have seen movement - Leave this as 0
int iShoutCount = 0;
//iShoutLimit is the amount of seconds after sensing movement that
//we will ignore all movement sensed - Change this to how ever many
//seconds you want to ignore movement after sensing it
int iShoutLimit = 10;

//The PIR takes time to calibrate to it's surroundings.
//30 should do, but the more the better
int iCalibrationTimeout = 40;

void setup() 
{
  //We want to allow others to listen for our movement
  Serial.begin(9600);
  
  //This is used to print a count down during calibrating
  //the PIR to be picked up by what ever is listening to
  //our serial
  int iCount = iCalibrationTimeout;
  
  //Set up or pins
  pinMode(ledPin, OUTPUT);
  pinMode(PIRPin, INPUT);
  
  //Let our listeners know that they need to wait for
  //the PIR to calibrate and start a count down
  Serial.println("Calibrating Sensor");
  for(int i = 0; i < iCalibrationTimeout; i++)
  {
    delay(1000);
    Serial.println(iCount);
    iCount--;
    
  }
  Serial.println("Calibration Complete\n");
  //Keep this delay, as if debugging, you'll prob not see that the 
  //calibration is complete if the debugging stuff below is all 
  //uncommented
  delay(2000);
  
  //Ok, so now we want to start looking for movement
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
      
      //And we'll shout to anything listening that there was movement
      Serial.println("Shout");
      
      //And we'll set some logic up to prevent checking for more 
      //movement before the shout time limiter is up
      bShouting = true;
      delay(1000);
    } 
    else 
    {
      //Make sure the LED is on
      digitalWrite(ledPin, HIGH);
      
      //The below line is for use setting up or debugging the PIR
      //It simply prints a '.' every time we don't get a reading
      //AKA no movement sensed
      //Serial.println(".");
      
      //We can continue looking for movement at next loop()
      bShouting = false;
    }
  }

  //Have we just sensed motion?
  if(bShouting)
  {
    //Yes, we'll start the shout time limiter
    if(bShouted == false)
    {
      //Debugging, this marks the START of the counter that prevents 
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
      //Debugging, this is the counter that prevents the signal to be
      //sent to what ever is listening for the shout more often than
      //the shout time limiter
      //Serial.println(String(iShoutCount) + " Shouting = " + String(bShouting) + " Shouted = " + String(bShouted));
      iShoutCount++;
      delay(1000);
    }
    //We are ready to make another shout
    else
    {
      //Debugging, this marks the END of the counter that prevents 
      //the signal to be sent to what ever is listening for the shout 
      //more often than the shout time limiter
      //Serial.println("Loop Ended - " + String(millis()/1000));
      
      //Logic to say that we can now look for more movement
      bShouting = true;
      bShouted = false;
      
      //Reset the Shout time limiter
      iShoutCount = 0;
      
      //More Debugging to state whether we are shouting, or have shouted
      //Serial.println("Shouting = " + String(bShouting) + " Shouted = " + String(bShouted));
      delay(1000);
    }
    
  }
}

