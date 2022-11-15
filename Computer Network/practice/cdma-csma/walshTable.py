def generateWalshTable(n:int):
    prevtable = [[1]]
    p = 1

    while p < n:
        p *= 2
        # First create the table of PxP dimension and fill with 0 
        curtable = []
        for i in range(p):
            row = [ 0 for _ in range(p)]
            curtable.append(row)

        # Fill appropiate values
        for rowI in range(0, p//2):
            for colI in range(0, p//2):
                curtable[rowI][colI] = prevtable[rowI][colI]
                curtable[rowI + p//2][colI] = prevtable[rowI][colI]
                curtable[rowI][colI + p//2] = prevtable[rowI][colI]
                curtable[rowI + p//2][colI + p//2] = -1 * prevtable[rowI][colI]
        
        # Update table
        prevtable = curtable
    return prevtable

print(generateWalshTable(4))