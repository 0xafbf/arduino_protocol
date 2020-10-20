
import gpiozero
import time
import random

in_clock = gpiozero.InputDevice(23)
in_data = gpiozero.InputDevice(24)

out_clock = gpiozero.OutputDevice(25)
out_data = gpiozero.OutputDevice(8)

timeout = 0.1 #s

def read_string():
    status = in_clock.value
    next_timeout = time.clock() + timeout
    data = [0] * 100000
    data_size = 0
    while True:
        new_status = in_clock.value
        new_time = time.clock()
            
        if new_status != status:
            next_timeout = new_time + timeout
            status = new_status
            data[data_size] = in_data.value
            data_size += 1

        elif new_time > next_timeout:
            if data_size != 0:
                return decode_buffer(data, data_size)

def main():
    init_morse_dict()
    while True:
        incoming = read_string()
        if incoming:
            print("NODE:"+incoming)
            if incoming == "HI":
                time.sleep(0.2)
                write_string("HI")
                incoming = read_string()
                print("NODE:"+incoming)
                if incoming == "WHAT IS YOUR NAME":
                    names = ["Raspberry", "Andres", "German"]
                    name_idx = random.randrange(len(names))
                    name = names[name_idx]
                    write_string(name)
                    


def write_string(output):
    print("RPI2:" + output)
    upper = output.upper()
    for letter in upper:
        code = morse_dict.get(letter)
        if code:
            write_letter(code)
        

def write_letter(char_code):
    for bit in char_code:
        if bit == '.':
            write_dot()
        else:
            write_dash()
    write_bit(False)
        
    
def write_dot():
    # codified as 10
    write_bit(True)
    write_bit(False)
    
def write_dash():
    #codified as 110
    write_bit(True)
    write_bit(True)
    write_bit(False)


def write_bit(bit):
    out_data.value = bit
    out_clock.value = not out_clock.value
    time.sleep(0.001)
    

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
morse_dict["1"] = ".----"
morse_dict["2"] = "..---"
morse_dict["3"] = "...--"
morse_dict["4"] = "....-"
morse_dict["5"] = "....."
morse_dict["6"] = "-...."
morse_dict["7"] = "--..."
morse_dict["8"] = "---.."
morse_dict["9"] = "----."
morse_dict["0"] = "-----"

# using other unclaimed codes for special chars:
morse_dict[" "] = "..--"

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
                
    return "".join(current_word)

main()
