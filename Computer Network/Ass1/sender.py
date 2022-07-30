import random
from traceback import print_tb

from helper import StringToBinary, generateDivisor, polynomialStringParser
from errorModules import CRC, VRC, LRC, CheckSum
from config import availableCRCPolynomials

# ? Sender Class
class Sender:
    dataWordFrameSize = 0
    input = ""
    rawInput = "" # Holds input whether it was binary or string
    output = ""
    outputWithoutAnyError = ""

    def __init__(self, dataWordFrameSize=8):
        self.dataWordFrameSize = dataWordFrameSize
    
    # IO Related functions
    def readInputFromConsole(self, binary):
        tmp = input("Enter the input: ")
        self.rawInput = tmp
        self.input = StringToBinary(tmp) if not binary else tmp

    def readInputFromFile(self, filename, binary):
        t = ""
        with open(filename, 'r') as f:
            for i in f.readlines():
                t += i
        self.rawInput = t.replace('\n', '')
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
            self.output = self.output[:random_bit_location] +  ('0' if self.output[random_bit_location] == '1' else '1') + self.output[random_bit_location+1:]

    # Inject error at specific index
    def injectErrorAtIndex(self, index):
        self.output = self.output[:index] + ('0' if self.output[index] == '1' else '1') + self.output[index+1:]

    # Encode Related Wrapper unctions
    # VRC Encoding
    def encodeUsingVRC(self):
        self.output = VRC.encode(self.input, self.dataWordFrameSize)
        self.outputWithoutAnyError = self.output

    # LRC Encoding
    def encodeUsingLRC(self, noOfOriginalDataFramesPerGroup=4):
        self.output = LRC.encode(self.input, self.dataWordFrameSize, noOfOriginalDataFramesPerGroup)
        self.outputWithoutAnyError = self.output

    # Checksum Encoding
    def encodeUsingChecksum(self, noOfOriginalDataFramesPerGroup=4):
        self.output = CheckSum.encode(self.input, self.dataWordFrameSize, noOfOriginalDataFramesPerGroup)
        self.outputWithoutAnyError = self.output

    # CRC Encoding
    def encodeUsingCRC(self, divisor):
        self.output = CRC.encode(self.input, self.dataWordFrameSize, divisor)
        self.outputWithoutAnyError = self.output



# ? ################# DRIVER CODE ###########
if __name__ == "__main__":
    # ! Data word frame size
    dataWordFrameSize = input("Enter no of bits in each data frame of dataword [default : 8]: ")
    dataWordFrameSize =  8 if dataWordFrameSize == '' else int(dataWordFrameSize)
    
    # ! Sender object
    sender = Sender(dataWordFrameSize=dataWordFrameSize)

    # ! Take input
    inputSourceAsTerminal = input("Do you want to use terminal as input source [y/n] : ")
    inputIsBinary = input("Do you want to input binary data [y/n] : ")
    if inputSourceAsTerminal.lower() == "y":
        sender.readInputFromConsole( binary= (inputIsBinary.lower() == "y"))
    else:
        filename = input("Enter the filename [default : assets/sender_input.txt ]: ")
        filename = "assets/sender_input.txt" if filename == '' else filename
        sender.readInputFromFile(filename, binary= (inputIsBinary.lower() == "y"))

    # ! Choose encoding method
    encodingMethod = input("Enter the encoding method [VRC, LRC, CRC, CHECKSUM] : ")
    if encodingMethod not in ["VRC", "LRC", "CRC", "CHECKSUM"] : raise Exception("Invalid encoding method") # Check for invalid input
    selectedPolynomial = ""

    if encodingMethod == "VRC":
        sender.encodeUsingVRC()
    elif encodingMethod == "LRC":
        noOfOriginalDataFramesPerGroup = input("Enter the no of data frames per group [default : 4]: ")
        noOfOriginalDataFramesPerGroup = 4 if noOfOriginalDataFramesPerGroup == '' else int(noOfOriginalDataFramesPerGroup)
        sender.encodeUsingLRC(noOfOriginalDataFramesPerGroup=noOfOriginalDataFramesPerGroup)
    elif encodingMethod == "CHECKSUM":
        noOfOriginalDataFramesPerGroup = input("Enter the no of data frames per group [default : 4]: ")
        noOfOriginalDataFramesPerGroup = 4 if noOfOriginalDataFramesPerGroup == '' else int(noOfOriginalDataFramesPerGroup)
        sender.encodeUsingChecksum(noOfOriginalDataFramesPerGroup=noOfOriginalDataFramesPerGroup)
    elif encodingMethod == "CRC":
        print("=== Available polynomials ===")
        for i in availableCRCPolynomials:
            print(i, end=", ")
        print(end="\n")
        selectedPolynomial = input("Enter the polynomial [default : CRC_4_ITU]: ")
        selectedPolynomial = "CRC_4_ITU" if selectedPolynomial == '' else selectedPolynomial
        parsedPolynomial = polynomialStringParser(input=availableCRCPolynomials[selectedPolynomial])
        divisor = generateDivisor(parsedPolynomial)
        sender.encodeUsingCRC(divisor=divisor)
    
    # ! Inject error
    if input("Do you want to inject error in output [y/n] : ").lower() == "y":
        if input("Mnaully inject error [y/n] : ").lower() == "y":
            specificBitsToInjectError = input("Enter the specific bits to inject error [seperate by commas] : ")
            specificBitsToInjectError = [int(i.strip()) for i in specificBitsToInjectError.split(",")]
            for i in specificBitsToInjectError:
                sender.injectErrorAtIndex(i)
        else:
            loopC = input("Enter the no of times you want to inject error [default : 1]: ")
            loopC = 1 if loopC == '' else int(loopC)
            sender.injectErrorInOutput(loopC=loopC)
    
    # ! File name to store output
    outputFileName = input("Enter the filename to store output [default : assets/sender_output.txt ]: ")
    outputFileName = "assets/sender_output.txt" if outputFileName == '' else outputFileName
    sender.writeOutputToFile(outputFileName)

    # ! Print data
    print("=========================================================================================")
    print("Raw Input              : ", sender.rawInput)
    print("Final Input            : ", sender.input)
    print("Encoding technique     : ", encodingMethod+" "+selectedPolynomial if encodingMethod == "CRC" else encodingMethod)
    print("Output [Without error] : ", sender.outputWithoutAnyError),
    print("Output [May have error]: ", sender.output)
    print("Written output in file : ", outputFileName)
    print("=========================================================================================")
        
    