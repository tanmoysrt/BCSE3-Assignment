def xor_list(a:str, b:str):
    result  = ''
    for i in range(1, len(a)):
        if a[i] == b[i]:
            result = result + "0"
        else:
            result = result + "1"
    return result


def divisionCRC(input:str, divisor:str):
    if len(input)< len(divisor) : return input

    pick = len(divisor)
    tmp = input[0:pick]

    while pick < len(input):
        if tmp[0] == '1':
            tmp = xor_list(tmp, divisor) + input[pick]
        else:
            tmp = xor_list(tmp, "0"*pick) + input[pick]

        pick = pick + 1

    if tmp[0] == "1":
        tmp = xor_list(tmp, divisor)
    else:
        tmp = xor_list(tmp, "0"*pick)

    return tmp


print(xor_list("1010", "0101"))
print(divisionCRC("1101011110110000", "11001"))