
import gpiozero
import time

line_clock = gpiozero.InputDevice(23)
line_data = gpiozero.InputDevice(24)

timeout = 0.1 #s

def main():
    init_morse_dict()

    status = False
    next_timeout = time.clock() + timeout

    data = [0] * 100000
    data_size = 0
    while True:
        new_status = line_clock.value
        new_time = time.clock()
        if new_time > next_timeout:
            next_timeout = new_time + timeout
            if data_size != 0:
                print("-----------------")
                decode_buffer(data, data_size)
                data_size = 0
            
        if new_status != status:
            next_timeout = new_time + timeout
            status = new_status
            data[data_size] = line_data.value
            data_size += 1

morse_dict = {
    "A": ".-",
    "B": "-...",
    "C": "-.-.",
    "D": "-..",
    "E": ".",
    "F": "..-.",
    "G": "--.",
    "H": "....",
    "I": "..",
    "J": ".---",
    "K": "-.-",
    "L": ".-..",
    "M": "--",
    "N": "-.",
    "O": "---",
    "P": ".--.",
    "Q": "--.-",
    "R": ".-.",
    "S": "...",
    "T": "-",
    "U": "..-",
    "V": "...-",
    "W": ".--",
    "X": "-..-",
    "Y": "-.--",
    "Z": "--..",
}
morse_dict_rev = {}

def init_morse_dict():
    for key, val in morse_dict.items():
        morse_dict_rev[val] = key

def decode_letter(buf):
    key = "".join(buf)
    if key in morse_dict_rev:
        return morse_dict_rev[key]
    else:
        print("key not found:%s" %key)
        return key
        
def decode_buffer(buf, buf_size):
    cursor = 0
    current_letter = list()
    current_word = list()
    while cursor < buf_size:
        if buf[cursor] == 0:
            cursor += 1
            if buf[cursor] == 0:
                cursor += 1
                current_word.append(decode_letter(current_letter))
                current_letter = list()
        else: #if buf[cursor] == 1
            if buf[cursor+1] == 1:
                cursor += 2
                current_letter.append("-")
            else:
                cursor += 1
                current_letter.append(".")
                
    print(current_word)

main()
