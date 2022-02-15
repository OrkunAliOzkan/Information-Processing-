import subprocess
import time



def send_on_jtag(cmd):

    assert cmd == 'n' or cmd == 'f', "Please make the cmd a single character"    # check if atleast one character is being sent down
    # inputCmd = "nios2-terminal {}".format(cmd);                 # call nios2-terminal and insert characters using <<<
    # subprocess allows python to run a bash command
    output = subprocess.Popen('C:\\intelFPGA_lite\\18.0\\quartus\\bin64\\nios2-terminal.exe', shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    strthing = cmd + '\n'
    output.stdin.write(bytes(strthing,'utf-8'))
    output.stdin.flush()
    line = output.stdout.readlines()
    print(line)

    #vals = output.stdout                                            # extract the output from the subprocess call

    #vals = vals.decode("utf-8")                                     # turn the byte-chars into a string

    #vals = vals.split('<-->')                                       # split the string according to the defined delimiters <-->

    #return vals[1].strip()                                          # return the data within the delimtiers <-->



def perform_computation():
    var = input('enter n or f:')
    res = send_on_jtag(var)                                   # example of how to use send_on_jtag function
    print(res)
    #time.sleep(1000)




def main():
    perform_computation()

   

if __name__ == '__main__':
    main()