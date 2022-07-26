import random

from helper import StringToBinary
from errorModules import VRC, LRC, CheckSum


class Sender:
    dataWordFrameSize = 0
    input = ""
    output = ""

    def __init__(self, dataWordFrameSize=8):
        self.dataWordFrameSize = dataWordFrameSize
    
    # IO Related functions
    def readInputFromConsole(self, binary):
        tmp = input("Enter the input: ")
        self.input = StringToBinary(tmp) if not binary else tmp

    def readInputFromFile(self, filename, binary):
        t = ""
        with open(filename, 'r') as f:
            for i in f.readlines():
                t += i
        self.input = StringToBinary(t.replace('\n', '')) if not binary else t.replace('\n', '')

    def writeOutputToFile(self, filename):
        if self.output == "": raise Exception("Output is empty ! Nothing to save")
        with open(filename, 'w') as f:
            f.write(self.output)
            f.close()

    # Error Injection Function
    def injectErrorInOutput(self, loopC=1):
        for i in range(loopC):
            random_bit_location = random.randint(0, len(self.output)-1)
            self.output = self.output[:random_bit_location] +  ['0','1'][random.randint(0,1)] + self.output[random_bit_location+1:]

    # Encode Related Functions

    # VRC Encoding
    def encodeUsingVRC(self):
        self.output = VRC.encode(self.input, self.dataWordFrameSize)

    # LRC Encoding
    def encodeUsingLRC(self, noOfFrames=4):
        self.output = LRC.encode(self.input, self.dataWordFrameSize, noOfFrames)

    # Checksum Encoding
    def encodeUsingChecksum(self, noOfFrames=4):
        self.output = CheckSum.encode(self.input, self.dataWordFrameSize, noOfFrames)


# Run the program
sender = Sender(dataWordFrameSize=8)
sender.readInputFromFile(filename="./assets/sender_input.txt", binary=False)
# sender.encode(method="VRC")
# sender.encode(method="LRC")
# sender.encodeUsingLRC(noOfFrames=4)
sender.encodeUsingChecksum(noOfFrames=4)
sender.injectErrorInOutput(loopC=10)
sender.writeOutputToFile(filename="assets/sender_output.txt")