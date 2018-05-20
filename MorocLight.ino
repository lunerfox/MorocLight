// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       MorocLight.ino
    Created:	5/20/2018 2:00:18 PM
    Author:     LUNERIBOX\Luner
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
}

// Add the main program code into the continuous loop() function
void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}
