def multiplyTuples(t1, t2):
	tup = []
	for i in range(len(t1)):
		tup.append(t1[i] * t2[i])
	return tup


def multiplyScalar(t1, x):
	tup = []
	for i in range(len(t1)):
		tup.append(t1[i] * x)
	return tup


def addTuples(t1, t2):
	tup = []
	for i in range(len(t1)):
		tup.append(t1[i]+t2[i])
	return tup


def generateWalshTable(n):
    print('Generating Walsh table ...')
    p = 1
    prevtable = [[1]]
    while p < n:
        p *= 2
        curtable = []
        for i in range(p):
            tup = []
            for j in range(p):
                tup.append(0)
            curtable.append(tup)
        for i in range(0, p//2):
            for j in range(0, p//2):
                curtable[i][j] = prevtable[i][j]
                curtable[i+p//2][j] = prevtable[i][j]
                curtable[i][j+p//2] = prevtable[i][j]
                curtable[i+p//2][j+p//2] = -1*prevtable[i][j]
        prevtable = curtable

    return prevtable

def displayWalshTable(walshtable):
    p = len(walshtable)
    for i in range(p):
        for j in range(p):
            if walshtable[i][j] == 1:
                print(end=' ')
            print(walshtable[i][j],end=' ')
        print()