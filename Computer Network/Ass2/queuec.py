from ctypes import sizeof


class QueueC:
    def __init__(self):
        self.data = []
        self.front = 0
        self.rear = 0
        self.size = 0
    
    def enqueue(self, data):
        self.data.append(data)
        self.rear += 1
        self.size += 1
    
    def dequeue(self):
        if self.size == 0:
            return None
        else:
            self.front += 1
            self.size -= 1
            return self.data[self.front-1]
    
    def isEmpty(self):
        return self.size == 0

    def peek(self):
        if self.size == 0:
            return None
        else:
            return self.data[self.front]

    def getFrameBySeqNo(self, seqNo, windowSize):
        atSeqNo = self.front/windowSize
        if atSeqNo == seqNo:
            return self.data[self.front]
        for (i, v) in enumerate(self.data[self.front:]):
            seqNo = (self.front+i)%windowSize
            if seqNo == seqNo:
                return self.data[self.front+i]
        return None