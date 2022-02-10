import subprocess

def send_on_jtag(cmd):
    assert cmd == 'n' or 'f', "Please make the cmd a single character"    # check if atleast one character is being sent down

    inputCmd = "nios2-terminal <<< {}".format(cmd);                 # call nios2-terminal and insert characters using <<<

    # subprocess allows python to run a bash command
    output = subprocess.run(inputCmd, shell=True, executable='/bin/bash', stdout=subprocess.PIPE)

    #vals = output.stdout                                            # extract the output from the subprocess call
    #vals = vals.decode("utf-8")                                     # turn the byte-chars into a string
    #vals = vals.split('<-->')                                       # split the string according to the defined delimiters <-->

    #return vals[1].strip()                                          # return the data within the delimtiers <-->

def perform_computation():
    res = send_on_jtag('testing')                                   # example of how to use send_on_jtag function
    print(res)

def main():
    perform_computation()
    
if __name__ == '__main__':
    main()