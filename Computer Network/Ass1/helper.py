def StringToBinary(string):
    return ''.join(format(ord(x), '08b') for x in string)

def BinaryToString(binary):
    return ''.join(chr(int(binary[i:i+8], 2)) for i in range(0, len(binary), 8))

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

def complementOfBinaryString(input):
    input = list(input)
    for i in range(len(input)):
        if input[i] == '0':
            input[i] = '1'
        else:
            input[i] = '0'
    return ''.join(input)