from fileinput import filename
from helper import BinaryToString, ReadNoOfZerosAndOnes, buildFrames, generateDivisor, polynomialStringParser
from errorModules import CRC, VRC, LRC, CheckSum
from config import availableCRCPolynomials


class Receiver:
    dataWordFrameSize=0
    input = ""
    output = ""
    outputData = ""
    errorFound = False
    
    def __init__(self, dataWordFrameSize=8):
        self.dataWordFrameSize = dataWordFrameSize

    # IO Related functions
    def readInputFromConsole(self):
        self.input = input("Enter the input in binary format: ")

    def readInputFromFile(self, filename):
        t = ""
        with open(filename, 'r') as f:
            for i in f.readlines():
                t += i
        self.input = t.replace('\n', '')

    def writeOutputToFile(self, filename, binary):
        tmp = BinaryToString(self.output) if not binary else self.output
        with open(filename, 'w') as f:
            f.write(tmp)
            f.close()

    # Decode Wrapper Methods
    # VRC Decoding 
    def decodeUsingVRC(self):
        output, errorFound = VRC.decode(self.input, self.dataWordFrameSize)
        self.output = output
        self.outputData = BinaryToString(self.output)
        self.errorFound = errorFound

    # LRC Decoding
    def decodeUsingLRC(self, noOfOriginalDataFramesPerGroup):
        output, errorFound = LRC.decode(self.input, self.dataWordFrameSize, noOfOriginalDataFramesPerGroup)
        self.output = output
        self.outputData = BinaryToString(self.output)
        self.errorFound = errorFound

    # Checksum Decoding
    def decodeUsingChecksum(self, noOfOriginalDataFramesPerGroup):
        output, errorFound = CheckSum.decode(self.input, self.dataWordFrameSize, noOfOriginalDataFramesPerGroup)
        self.output = output
        self.outputData = BinaryToString(self.output)
        self.errorFound = errorFound

    # CRC Decoding
    def decodeUsingCRC(self, divisor):
        output, errorFound = CRC.decode(self.input, self.dataWordFrameSize, divisor)
        self.output = output
        self.outputData = BinaryToString(self.output)
        self.errorFound = errorFound


if __name__ == "__main__":
    # ! Data word frame size
    dataWordFrameSize = input("Enter no of bits in each data frame of dataword [default : 8]: ")
    dataWordFrameSize =  8 if dataWordFrameSize == '' else int(dataWordFrameSize)
    
    # ! Receiver object
    receiver = Receiver(dataWordFrameSize=dataWordFrameSize)
    
    # ! Input for receiver
    if input("Do you want to read input from console? (y/n): ").lower() == "y":
        receiver.readInputFromConsole()
    else:
        inputfilename = input("Enter the filename [default : assets/sender_output.txt]: ")
        inputfilename = "assets/sender_output.txt" if inputfilename == '' else inputfilename
        receiver.readInputFromFile(inputfilename)
    

    # ! Choose decoding method
    decodingMethod = input("Enter the decoding method [VRC, LRC, CRC, CHECKSUM] : ")
    if decodingMethod not in ["VRC", "LRC", "CRC", "CHECKSUM"] : raise Exception("Invalid encoding method") # Check for invalid input
    selectedPolynomial = ""

    # ! Decode
    if decodingMethod == "VRC":
        receiver.decodeUsingVRC()
    elif decodingMethod == "LRC":
        noOfOriginalDataFramesPerGroup = input("Enter the no of data frames per group [default : 4]: ")
        noOfOriginalDataFramesPerGroup = 4 if noOfOriginalDataFramesPerGroup == '' else int(noOfOriginalDataFramesPerGroup)
        receiver.decodeUsingLRC(noOfOriginalDataFramesPerGroup=noOfOriginalDataFramesPerGroup)
    elif decodingMethod == "CHECKSUM":
        noOfOriginalDataFramesPerGroup = input("Enter the no of data frames per group [default : 4]: ")
        noOfOriginalDataFramesPerGroup = 4 if noOfOriginalDataFramesPerGroup == '' else int(noOfOriginalDataFramesPerGroup)
        receiver.decodeUsingChecksum(noOfOriginalDataFramesPerGroup=noOfOriginalDataFramesPerGroup)
    elif decodingMethod == "CRC":
        print("=== Available polynomials ===")
        for i in availableCRCPolynomials:
            print(i, end=", ")
        print(end="\n")
        selectedPolynomial = input("Enter the polynomial [default : CRC_4_ITU]: ")
        selectedPolynomial = "CRC_4_ITU" if selectedPolynomial == '' else selectedPolynomial
        parsedPolynomial = polynomialStringParser(input=availableCRCPolynomials[selectedPolynomial])
        divisor = generateDivisor(parsedPolynomial)
        receiver.decodeUsingCRC(divisor=divisor)

    # ! Write output to file
    outputFilename = input("Enter the filename [default : assets/receiver_output.txt]: ")
    outputFilename = "assets/receiver_output.txt" if outputFilename == '' else outputFilename
    tmpFilename = outputFilename.split(".")[0]

    receiver.writeOutputToFile(filename=tmpFilename+"_binary.txt", binary=True)
    receiver.writeOutputToFile(filename=tmpFilename+"_data.txt", binary=False)

    # ! Print data
    print("=========================================================================================")
    print("Input                  : ", receiver.input)
    print("Decoding technique     : ", decodingMethod+" "+selectedPolynomial if decodingMethod == "CRC" else decodingMethod)
    print("Output Binary          : ", receiver.output)
    print("Output Data            : ", receiver.outputData)
    print("Status                 : ", "FAILED" if receiver.errorFound else "PASS")
    print("Written binary output in file : ", tmpFilename+"_binary.txt")
    print("Written binary output in file : ", tmpFilename+"_data.txt")
    print("=========================================================================================")
        