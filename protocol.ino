/*

	trabajo custom protocol
*/

int LED = D4;
int CLOCK = D3;
int DATA = D2;
bool CLOCK_STATE;
int IN_CLOCK = D1;
int IN_DATA = D0;


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

char* NUM_0 = "-----";
char* NUM_1 = ".----";
char* NUM_2 = "..---";
char* NUM_3 = "...--";
char* NUM_4 = "....-";
char* NUM_5 = ".....";
char* NUM_6 = "-....";
char* NUM_7 = "--...";
char* NUM_8 = "---..";
char* NUM_9 = "----.";

char* SPACE = "..--";

char* numbers[10] = {
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
};
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
	pinMode(LED, OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(DATA, OUTPUT);
	pinMode(IN_CLOCK, INPUT);
	pinMode(IN_DATA, INPUT);

	Serial.begin(9600);
}

void write_string(char* str) {
	Serial.print("SEND:");
	Serial.println(str);
	char* current_char = str;
	while(*current_char != NULL) {
		char my_char = *current_char;
		if (my_char >= 'a' && my_char <= 'z') my_char -= to_lower;
		if (my_char >= first_char && my_char <= last_char) {
			my_char -= first_char;
			char* char_code = letters[my_char];
			write_letter(char_code);
		} else if (my_char == ' ') {
			write_letter(SPACE);
		} else if (my_char >= '0' && my_char <= '9') {
			my_char -= '0';
			char* char_code = numbers[my_char];
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

char data[1024];
int data_size;
char data_decoded[1024];
int TIMEOUT = 100;

char* read_string(int max_wait) {
	Serial.println("waiting string");
	data_size = 0;
	bool in_clock_state = digitalRead(IN_CLOCK);
	int in_timeout = max_wait + millis();
	while (true) {
		bool new_clock = digitalRead(IN_CLOCK);
		int new_millis = millis();
		if (new_clock != in_clock_state) {
			in_clock_state = new_clock;
			in_timeout = TIMEOUT + new_millis;
			bool value = digitalRead(IN_DATA);
			data[data_size] = value;
			data_size += 1;
		} else if (new_millis > in_timeout) {
			break;
		}
		yield();
	}
	if (data_size == 0) return NULL;
	Serial.print("size:");
	Serial.println(data_size);
	int data_cursor = 0;
	int string_cursor = 0;

	char code_buffer[8];
	int code_cursor = 0;
	
	while (data_cursor < data_size) {
		if (data[data_cursor] == 0) {
			data_cursor += 1;
			if (data[data_cursor] == 0) {
				code_buffer[code_cursor] = 0;
				char letter = decode_letter(code_buffer);
				if (letter != 0) {
					data_decoded[string_cursor] = letter;
					string_cursor += 1;
				}
				code_cursor = 0;
			}
			
		} else { // data[data_cursor] == 1
			data_cursor += 1;
			if (data[data_cursor] == 1) {
				data_cursor += 1;
				// decode dash
				code_buffer[code_cursor] = '-';
				++code_cursor;
			} else { // data[data_cursor] == 0
				// decode dot
				code_buffer[code_cursor] = '.';
				++code_cursor;
			}
								 
		}
	}
	if (string_cursor == 0) return NULL;
	data_decoded[string_cursor] = 0;
	Serial.print("RECV:");
	Serial.println(data_decoded);
	return data_decoded;
}

char decode_letter(char* code) {
	if (code[0] == 0) return 0;
	if (code[0] == '.') {
		if (code[1] == 0) return 'E';
		if (code[1] == '.') {
			if (code[2] == 0) return 'I';
			if (code[2] == '.') {
				if (code[3] == 0) return 'S';
				if (code[3] == '.') {
					if (code[4] == 0) return 'H';
					if (code[4] == '.') return '5';
					if (code[4] == '-') return '4';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return 'V';
					if (code[4] == '-') return '3';
				}
			} else if (code[2] == '-') {
				if (code[3] == 0) return 'U';
				if (code[3] == '.') {
					if (code[4] == 0) return 'F';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return ' ';
					if (code[4] == '-') return '2';
				}
			}
		}
		if (code[1] == '-') {
			if (code[2] == 0) return 'A';
			if (code[2] == '.') {
				if (code[3] == 0) return 'R';
				if (code[3] == '.') {
					if (code[4] == 0) return 'L';
				}
			} else if (code[2] == '-') {
				if (code[3] == 0) return 'W';
				if (code[3] == '.') {
					if (code[4] == 0) return 'P';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return 'J';
				}
			}
		}
	}
	if (code[0] == '-') {
		if (code[1] == 0) return 'T';
		if (code[1] == '.') {
			if (code[2] == 0) return 'N';
			if (code[2] == '.') {
				if (code[3] == 0) return 'D';
				if (code[3] == '.') {
					if (code[4] == 0) return 'B';
					if (code[4] == '.') return '6';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return 'X';
				}
			} else if (code[2] == '-') {
				if (code[3] == 0) return 'K';
				if (code[3] == '.') {
					if (code[4] == 0) return 'C';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return 'Y';
				}
			}
		}
		if (code[1] == '-') {
			if (code[2] == 0) return 'M';
			if (code[2] == '.') {
				if (code[3] == 0) return 'G';
				if (code[3] == '.') {
					if (code[4] == 0) return 'Z';
					if (code[4] == '.') return '7';
				}
				if (code[3] == '-') {
					if (code[4] == 0) return 'Q';
				}
			} else if (code[2] == '-') {
				if (code[3] == 0) return 'O';
				if (code[3] == '.') {
					if (code[4] == '.') return '8';
				}
				if (code[3] == '-') {
					if (code[4] == '.') return '9';
					if (code[4] == '-') return '0';
				}
			}
		}
	}

	return '?';
}

void wait(int ms) {
	int target = millis() + ms;
	while (millis() < target) yield();
}

void read_clock() {
	bool _clock = digitalRead(IN_CLOCK);
	bool _data = digitalRead(IN_DATA);
	Serial.print("CLK:");
	Serial.print(_clock);
	Serial.print(" DTA:");
	Serial.print(_data);
	Serial.println();
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(CLOCK, CLOCK_STATE);

	write_string("hi");

	digitalWrite(LED, LOW);
	char* response = read_string(2000);
	if (response != NULL) {
		if (strcmp(response, "HI") == 0) {
			write_string("what is your name");
			response = read_string(2000);
			if (response != NULL) {
				char greeting[64];
				sprintf(greeting, "hello %s", response);
				write_string(greeting);
			}
		}
	}

	wait(1000);

	write_string("bye");
	
	digitalWrite(LED, HIGH);
	wait(1000);
}
