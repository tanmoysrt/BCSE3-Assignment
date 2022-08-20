from math import remainder
from helper import ReadNoOfZerosAndOnes, addBinaryStringUsingOnesComplement, buildFrames, complementOfBinaryString, divisonCRC


class VRC:
    @staticmethod
    def encode(binaryInputString:str, dataWordFrameSize:int):
        # ? Even Parity VRC
        output = ""
        for i in buildFrames(binaryInputString, dataWordFrameSize):
            _ , noOfOnes = ReadNoOfZerosAndOnes(i)
            output += i + ("1" if noOfOnes % 2 == 1 else "0")
        return output

    @staticmethod
    def decode(binaryInputString:str, dataWordFrameSize):
        frames = buildFrames(binaryInputString, dataWordFrameSize+1)
        errorFound = False
        output = ""
        for i in frames:
            # ? Even Parity VRC
            _ , noOfOnes = ReadNoOfZerosAndOnes(i)
            if noOfOnes % 2 == 0:
                i = i[:-1]
                output += i
            else:
                errorFound = True

        return output, errorFound

class LRC:
    @staticmethod
    def encode(binaryInputString:str, dataWordFrameSize:int, noOfOriginalDataFramesPerGroup:int=4):
        output = ""
        frames = buildFrames(binaryInputString, dataWordFrameSize)
        # Check whether we can split in to equal length of frames
        noOfFramesRequiredToBeAdded = noOfOriginalDataFramesPerGroup - len(frames)%noOfOriginalDataFramesPerGroup
        if noOfFramesRequiredToBeAdded > 0 and noOfFramesRequiredToBeAdded != noOfOriginalDataFramesPerGroup:
            for _ in range(noOfFramesRequiredToBeAdded):
                frames.append("0"*dataWordFrameSize)
        
        # Iterate over the frames to calculate the parity
        for i in range(0, len(frames), noOfOriginalDataFramesPerGroup):
            tmpFrames = frames[i:i+noOfOriginalDataFramesPerGroup]
            parity = ""
            for index in range(dataWordFrameSize)[::-1]:
                noOfOnes = 0
                # Count no of ones in frame
                for frame in tmpFrames:
                    if frame[index] == "1":
                        noOfOnes += 1
                # Add parity bit
                parity = ("1" if noOfOnes % 2 != 0 else "0") + parity
                

            # Append to output
            output = output + ''.join(tmpFrames) + parity

        return output

    @staticmethod
    def decode(binaryInputString:str, dataWordFrameSize:int, noOfOriginalDataFramesPerGroup:int=4):
        # Split frames from data
        frames = buildFrames(binaryInputString, dataWordFrameSize)
        errorFound = False
        output = ""
        # Iterate over the frames
        for i in range(0, len(frames), noOfOriginalDataFramesPerGroup+1):
            tmp_frames = frames[i:i+noOfOriginalDataFramesPerGroup+1]
            parity_frame = tmp_frames[-1]
            frameErrorFound = False
            # Verify parity
            for index in range(dataWordFrameSize)[::-1]:
                noOfOnes = 0
                for frame in tmp_frames[:-1]:
                    noOfOnes = noOfOnes + (1 if frame[index] == '1' else 0)
                noOfOnes = noOfOnes + (1 if parity_frame[index] == '1' else 0)
                if noOfOnes % 2 != 0:
                    frameErrorFound = True
                    break
            if frameErrorFound:
                errorFound = True
            else:
                output += ''.join(tmp_frames[:-1])
        
        return output, errorFound

class CheckSum:
    @staticmethod
    def encode(binaryInputString:str, dataWordFrameSize:int, noOfOriginalDataFramesPerGroup:int=4):
        output = ""
        frames = buildFrames(binaryInputString, dataWordFrameSize)
        # Check whether we can split in to equal length of frames
        noOfFramesRequiredToBeAdded = noOfOriginalDataFramesPerGroup - len(frames)%noOfOriginalDataFramesPerGroup
        if noOfFramesRequiredToBeAdded > 0 and noOfFramesRequiredToBeAdded != noOfOriginalDataFramesPerGroup:
            for _ in range(noOfFramesRequiredToBeAdded):
                frames.append("0"*dataWordFrameSize)

        # Iterate over the frames to calculate the checksum
        for i in range(0, len(frames), noOfOriginalDataFramesPerGroup):
            tmpFrames = frames[i:i+noOfOriginalDataFramesPerGroup]
            tmp = tmpFrames[0]
            frameString = ""
            for frame in tmpFrames[1:]:
                tmp = addBinaryStringUsingOnesComplement(tmp, frame)
                frameString += frame
            
            # Complement and get checksum
            checksum = complementOfBinaryString(tmp)
            output = output + tmpFrames[0] + frameString + checksum
        
        return output

    @staticmethod
    def decode(binaryInputString:str, dataWordFrameSize:int, noOfOriginalDataFramesPerGroup:int=4):
        output = ""
        frames = buildFrames(binaryInputString, dataWordFrameSize)
        errorFound = False

        # Iterate over the frames to calculate the checksum and verify 
        for i in range(0, len(frames), noOfOriginalDataFramesPerGroup+1):
            tmpFrames = frames[i:i+noOfOriginalDataFramesPerGroup+1]
            tmp = tmpFrames[0]
            frameString = ""
            for frame in tmpFrames[1:]:
                tmp = addBinaryStringUsingOnesComplement(tmp, frame)
                frameString += frame
            
            # Complement and get checksum
            checksum = complementOfBinaryString(tmp)
            if checksum == "0"*dataWordFrameSize:
                output += (tmpFrames[0] + frameString[:-dataWordFrameSize])
            else:
                errorFound = True
        
        return output, errorFound

class CRC:
    @staticmethod
    def encode(binaryInputString:str, dataWordFrameSize:int, divisor:str):
        output = ""
        crcSize = len(divisor)-1
        for i in buildFrames(binaryInputString, dataWordFrameSize):
            tmp = i+crcSize*"0" # [max degree of polynomial] times 0
            crc = divisonCRC(tmp, divisor)[:crcSize]
            output += i+crc.zfill(crcSize)
        return output

    @staticmethod
    def decode(binaryInputString:str, dataWordFrameSize:int, divisor:str):
        output = ""
        errorFound = False
        crcSize = len(divisor)-1
        for i in buildFrames(binaryInputString, dataWordFrameSize+crcSize):
            remainder = divisonCRC(i, divisor)
            if remainder == 0 or remainder == '0':
                messageData = i[:-crcSize]
                # print(messageData)
                output += messageData
            else:
                errorFound = True
        
        return output, errorFound