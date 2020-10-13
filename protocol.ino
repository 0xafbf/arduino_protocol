/*

	trabajo custom protocol
*/

int LED = D4;
int CLOCK = D4;
int DATA = D3;
bool CLOCK_STATE;

char* LETTER_A = ".-";
char* LETTER_B = "-...";
char* LETTER_C = "-.-.";
char* LETTER_D = "-..";
char* LETTER_E = ".";
char* LETTER_F = "..-.";
char* LETTER_G = "--.";
char* LETTER_H = "....";
char* LETTER_I = "..";
char* LETTER_J = ".---";
char* LETTER_K = "-.-";
char* LETTER_L = ".-..";
char* LETTER_M = "--";
char* LETTER_N = "-.";
char* LETTER_O = "---";
char* LETTER_P = ".--.";
char* LETTER_Q = "--.-";
char* LETTER_R = ".-.";
char* LETTER_S = "...";
char* LETTER_T = "-";
char* LETTER_U = "..-";
char* LETTER_V = "...-";
char* LETTER_W = ".--";
char* LETTER_X = "-..-";
char* LETTER_Y = "-.--";
char* LETTER_Z = "--..";

char* letters[26] = {
	LETTER_A,
	LETTER_B,
	LETTER_C,
	LETTER_D,
	LETTER_E,
	LETTER_F,
	LETTER_G,
	LETTER_H,
	LETTER_I,
	LETTER_J,
	LETTER_K,
	LETTER_L,
	LETTER_M,
	LETTER_N,
	LETTER_O,
	LETTER_P,
	LETTER_Q,
	LETTER_R,
	LETTER_S,
	LETTER_T,
	LETTER_U,
	LETTER_V,
	LETTER_W,
	LETTER_X,
	LETTER_Y,
	LETTER_Z,
};
char to_lower = 'a' - 'A';
char first_char = 'A';
char last_char = 'Z';

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(CLOCK, OUTPUT);
	pinMode(DATA, OUTPUT);
}

void write_string(char* str) {
	char* current_char = str;
	while(*current_char != NULL) {
		char my_char = *current_char;
		if (my_char > 'Z') my_char -= to_lower;
		if (my_char >= first_char && my_char <= last_char) {
			my_char -= first_char;
			char* char_code = letters[my_char];
			write_letter(char_code);
		}
		current_char += 1;
	}
}

void write_letter(char* letter_chars) {
	char* current_char = letter_chars;
	while (*current_char != NULL) {
		if (*current_char == '.') {
			write_dot();
		} else {
			write_dash();
		}
		++current_char;
	}
	write_bit(LOW);
}

void write_dot() {
	write_bit(HIGH);
	write_bit(LOW);
}

void write_dash() {
	write_bit(HIGH);
	write_bit(HIGH);
	write_bit(LOW);
}

void write_bit(bool bit) {
	digitalWrite(DATA, bit);
	CLOCK_STATE = !CLOCK_STATE;
	digitalWrite(CLOCK, CLOCK_STATE);
	delay(1);
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(CLOCK, CLOCK_STATE);

	write_string("AndresFelipeBotero");
	
	// end with silence
	digitalWrite(CLOCK, HIGH);    // turn the LED off by making the voltage LOW
	delay(1000);                       // wait for a second
  
}
