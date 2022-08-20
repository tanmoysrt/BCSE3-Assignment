from functools import cmp_to_key
from typing import List, Tuple

def StringToBinary(string):
    return ''.join(format(ord(x), '08b') for x in string)

def BinaryToString(binary):
    return ''.join(chr(int(binary[i:i+8], 2)) for i in range(0, len(binary), 8))

def XOR(a, b):
    if a == b:
        return '0'
    return '1'

def XOR_List(a, b):
    return str(int(''.join([XOR(a[i], b[i]) for i in range(len(a))])))

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

def addBinaryString(a, b):
    max_len = max(len(a), len(b))
    a = a.zfill(max_len)
    b = b.zfill(max_len)
    result = ''
    carry = 0
    
    # Traverse the string
    for i in range(max_len - 1, -1, -1):
        r = carry
        r += 1 if a[i] == '1' else 0
        r += 1 if b[i] == '1' else 0
        result = ('1' if r % 2 == 1 else '0') + result
    
        # Compute the carry.
        carry = 0 if r < 2 else 1
    
    if carry != 0:
        result = '1' + result
    
    return result.zfill(max_len)

def addBinaryStringUsingOnesComplement(a, b):
    max_len = max(len(a), len(b))
    a = a.zfill(max_len)
    b = b.zfill(max_len)
    result = ''
    carry = 0
    
    # Traverse the string
    for i in range(max_len - 1, -1, -1):
        r = carry
        r += 1 if a[i] == '1' else 0
        r += 1 if b[i] == '1' else 0
        result = ('1' if r % 2 == 1 else '0') + result
    
        # Compute the carry.
        carry = 0 if r < 2 else 1
    
    if carry != 0:
        result = addBinaryStringUsingOnesComplement(result, '1')
    
    return result.zfill(max_len)

def sumOfListOfBinaryString(l):
    result = l[0]
    for i in range(1, len(l)):
        result = addBinaryString(result, l[i])
    return result

def complementOfBinaryString(input:str):
    input = list(input)
    for i in range(len(input)):
        if input[i] == '0':
            input[i] = '1'
        else:
            input[i] = '0'
    return ''.join(input)

# Return sorted list of (power, coefficient)
# Valid polynomial format
# x^4 + x^2 + x^1 + 1
# return polynomials in form of list of (power, coefficient) in descending order of power
def polynomialStringParser(input:str):
    # Raise exception for invalid input
    if input == '' :
        raise Exception("Invalid input")
    if input.find('x') != -1 and input.find('x^') == -1:
        raise Exception("Invalid input")

    # Main logic
    data = []

    # Iterate over list of (Splitted at the '+').removeSpaces
    for d in [j.strip() for j in input.split('+')]:
        if len(d) == 1:
            data.append((0, int(d)))
        else:
            tmp = d.split('x^')
            if len(tmp) == 1:
                data.append((int(tmp[0]), 1))
            elif len(tmp) == 2:
                if tmp[0] == '':
                    data.append((int(tmp[1]), 1))
                else:
                    data.append((int(tmp[1]), int(tmp[0])))

    # Sort the list by degree
    data = sorted(data, key=cmp_to_key(lambda item1, item2: item2[0] - item1[0]))

    # Fill blank degress with 0 coefficients
    final_data = []

    i=0
    while i < len(data)-1:
        final_data.append(data[i])
        a = data[i][0]
        b = data[i+1][0]

        if a-b > 1:
            for j in range(b+1, a)[::-1]:
                final_data.append((j, 0))
        i+=1
    final_data.append(data[i])
    return final_data

# Accept polynomials in form of list of (power, coefficient) in descending order of power
def generateDivisor(input:List):
    divisor = ''
    for i in input:
        divisor += ("1" if i[1] != 0 else "0")
    return divisor

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
    input = str(int(input))
    divisor = str(int(divisor))

    # If input is longer than divisor, return input 
    if len(input) < len(divisor):
        return input
        
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

    return str(int(tmp))




# print(generateDivisor(polynomialStringParser("x^3 + x^4 + 5")))
# print(divisonCRC("010010000000", "11001"))
# print(divisonCRC("001", "11001"))
# print(XOR_List("11000", "11001"))