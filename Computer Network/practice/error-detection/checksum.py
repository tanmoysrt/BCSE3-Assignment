def addBinaryStringUsingOnesComplement(a:str, b:str):
    max_len = max(len(a), len(b))

    a = a.zfill(max_len)
    b = b.zfill(max_len)

    result = ""
    carry = 0

    for i in range(max_len-1, -1, -1):
        r = carry
        r += 1 if a[i] == '1' else 0
        r += 1 if b[i] == '1' else 0

        # if r is 1 that means it has no carry and result 1 ,
        # but, if r is 2 that means in binary it would be actually 10 , that means r will be  0 and carry will be 1

        result = ('1' if r%2 == 1 else '0') + result
        carry = 0 if r<2 else 1

    if carry != 0:
        result = addBinaryStringUsingOnesComplement(result, '1')
    return result.zfill(max_len)

def complement(a:str):
    result = ''
    for i in a:
        result = result + ('0' if i == '1' else '1')
    return result

def calculateChecksum(frames:list):
    result = frames[0]

    for frame in frames[1:]:
        result = addBinaryStringUsingOnesComplement(result, frame)

    result = complement(result)
    return result

print(complement('10101010'))
frames = ["11001", "01001", "11100", "00110", "10111"]
res = calculateChecksum(frames)
print(res)

frames.append(res)
res = calculateChecksum(frames)
print(res)