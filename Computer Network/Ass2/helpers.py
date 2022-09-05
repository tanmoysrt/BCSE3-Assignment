from cmath import sqrt


def ReadNoOfZerosAndOnes(input):
    noOfZeros = 0
    noOfOnes = 0

    for i in input:
        if i == '0':
            noOfZeros += 1
        elif i == '1':
            noOfOnes += 1
    
    return noOfZeros, noOfOnes

def buildFrames(input, frameSize):
    output = []
    for i in range(0, len(input), frameSize):
        output.append(input[i:i+frameSize])
    return output

def encodeData(data, divisor="10011"):
    x = len(str(int(divisor)))-1
    tmp = data + "0"*x
    crc = divisonCRC(tmp, divisor)
    return data + crc

def decodeData(data, divisor="10011"):
    x = len(str(int(divisor)))-1
    res = divisonCRC(str(data), divisor)
    if int(res) == 0:
        return (True, data[:-x])
    return (False, "")

def generateACK(rn, with_parity=True, for_selective_repeat=False, isNak=False):
    # Convert rn to binary string of 8 length
    ack = str(bin(rn))[2:]
    ack = ack.zfill(4)

    if for_selective_repeat:
        ack = ('0' if isNak else '1') + ack

    if with_parity:
        ack = encodeData(ack, divisor="10011")
    print("res", ack)
    return ack


    

# XOR List
def xor_list(a, b):

	# initialize result
	result = []

	# Traverse all bits, if bits are
	# same, then XOR is 0, else 1
	for i in range(1, len(a)):
		if a[i] == b[i]:
			result.append('0')
		else:
			result.append('1')

	return ''.join(result)

# Divide function for CRC
def divisonCRC(input:str, divisor:str):        
    pick = len(divisor)
    tmp = input[0 : pick]
    
    while pick < len(input):
        if tmp[0] == '1':
            tmp = xor_list(tmp, divisor) + input[pick]
        else:
            tmp = xor_list(tmp, '0'*pick) + input[pick]
        
        pick += 1

    if tmp[0] == '1':
        tmp = xor_list(tmp, divisor)
    else:
        tmp = xor_list(tmp, '0'*pick)

    x = len(str(int(divisor)))-1
    return str(int(tmp)).zfill(x)


    

def generateSequenceNumbers(windowSize):
    x = int(sqrt(windowSize).real)
    nmax = int('1'*x, 2)
    sequenceNumbers = [i for i in range(nmax+1)]
    return sequenceNumbers