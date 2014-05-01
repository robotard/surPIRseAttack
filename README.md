surPIRseAttack
==============

Linking a PIR sensor to an Arduino, sensing movement, communicating up movement on a Mac and playing a noise

An example of getting an Arduino to talk to a Mac. The PIR motion sensor senses some movement in the room, then waits 3 seconds (for dramatic effect), then 'shouts' to the Mac that there has been motion... 

The Mac (through processing) is looking for a specific shout (can add more through this mechanism) over the USB comms, and in turn, chooses to play a sound of your choosing (chainsaw in my case)...

For dramatic effect, plug into speakers at full volume

N.B. The PIR take ~50 seconds to calibrate... I've put a delay in for you to leave the room and close the door etc.

Creative Commons, so knock yourselves out!

Rob Hendricks

RobHendricks83@gmail.com
www.robustas.co.uk
