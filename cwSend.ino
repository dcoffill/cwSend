#include <arduino.h>

const byte led_pin = 8;

// Default space unit
const byte element_wait = 1000;


/* This lookup table contains binary encodings of of each ASCII character's morse code representation, which are decoded by looking at the LSB and shifting.  A 'dot' is represented with a 0, and a 'dash' by a 1.  The byte-representation for each character contains a "header" of zero or more 0s, a 1 indicating that the encoding has actually started, 

For example, D is '-..' in Morse code.  It would be encoded as 00110000, where (assuming LSB is position 0) the bit in position 4 represents the start bit, the following 1 represents a dash, and the final two zeroes represent the two dots.
*/
const byte symbol_table[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //Starts from ASCII 32 (space)
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, // ASCII capital letters, A-Z
0xA0, 0x18, 0x58, 0x30, 0x40, 0x48, 0x70, 0x08, 0x20, 0xE8,
0xB0, 0x28, 0xC0, 0x60, 0xF0, 0x68, 0xB8, 0x50, 0x10, 0xC0,
0x90, 0x88, 0xD0, 0x98, 0xD8, 0x38};

void setup()
{
	pinMode(led_pin, OUTPUT);
	Serial.begin(9600);
}


void loop()
{
	Serial.println("Starting function");
	send_string("HI K");
	send_string("HELLO WORLD");
}

/* This function takes an input string and signals an output device with the corresponding
morse code representation. */
void send_string(char input_string[])
{
	for (byte count = 0; input_string[count] != '\0'; ++count)
	{
		Serial.println(input_string[count]); // Print the current letter to the serial console
		if (input_string[count] == ' ')
		{
			word_space();
			continue;
		}

		// Look up the encoding in the table
		byte i, char_code = ( symbol_table[ input_string[count] - 30] ) >> 1; 

		// Find the beginning of the actual morse encoding
		for (i = 1; i < 7; ++i)
		{
			if(char_code&0x01)
			{ //If bit 0 is a 1, then let's stop analysis and start keying
				char_code = char_code>>1;
				break;
			}
			char_code = char_code>>1;
		}
		
		//Keying loop, where the magic happens!
		for (i; i < 7; ++i)
		{
			if(char_code & 0x01)
			{
				dash();
				Serial.println("-");
			}
			else {
				dot();
				Serial.println(".");
			}
			letter_space();
			char_code = char_code>>1; 
		}
	}
}

void dot()
{
	digitalWrite(led_pin, HIGH);
	delay(element_wait); // milliseconds
	digitalWrite(led_pin, HIGH);
	delay(element_wait);
}

void dash()
{

	digitalWrite(led_pin, HIGH);
	delay(element_wait * 3); // milliseconds
	digitalWrite(led_pin, LOW);
	delay(element_wait);
}

void space()
{
	delay(100);
}

void letter_space()
{
	delay(element_wait*2);
}

void word_space()
{
	delay(element_wait*6);
}

