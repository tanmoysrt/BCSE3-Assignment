from helper import BinaryToString, ReadNoOfZerosAndOnes, buildFrames
from errorModules import CRC, VRC, LRC, CheckSum


class Receiver:
    input = ""
    output = ""
    
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

    # Decode Methods

    # VRC Decoding 
    def decodeUsingVRC(self, dataWordFrameSize):
        output, errorFound = VRC.decode(self.input, dataWordFrameSize)
        self.output = output
        print("VRC Decode Completed !")
        print("Check : "+("PASS" if not errorFound else "FAILED"))

    # LRC Decoding
    def decodeUsingLRC(self, dataWordFrameSize, noOfDataFrames):
        output, errorFound = LRC.decode(self.input, dataWordFrameSize, noOfDataFrames)
        self.output = output
        print("LRC Decode Completed !")
        print("Check : "+("PASS" if not errorFound else "FAILED"))

    # Checksum Decoding
    def decodeUsingChecksum(self, dataWordFrameSize, noOfDataFrames):
        output, errorFound = CheckSum.decode(self.input, dataWordFrameSize, noOfDataFrames)
        self.output = output
        print("Checksum Decode Completed !")
        print("Check : "+("PASS" if not errorFound else "FAILED"))

    # CRC Decoding
    def decodeUsingCRC(self, dataWordFrameSize, divisor):
        output, errorFound = CRC.decode(self.input, dataWordFrameSize, divisor)
        self.output = output
        print("CRC Decode Completed !")
        print("Check : "+("PASS" if not errorFound else "FAILED"))



receiver = Receiver()
receiver.readInputFromFile(filename="assets/sender_output.txt")
# receiver.decodeUsingVRC(dataWordFrameSize=8)
# receiver.decodeUsingLRC(dataWordFrameSize=8, noOfDataFrames=4)
# receiver.decodeUsingChecksum(dataWordFrameSize=8, noOfDataFrames=4)
receiver.decodeUsingCRC(dataWordFrameSize=8, divisor="11001")
receiver.writeOutputToFile(filename="assets/output_receiver_binary.txt", binary=True)
receiver.writeOutputToFile(filename="assets/output_receiver_data.txt", binary=False)