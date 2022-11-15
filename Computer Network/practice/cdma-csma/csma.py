import threading
import time
import random


frameTime = 3
interFrameTime = 1

# Total frames currently sent
totalFrames = 0

'''
The log function calulate utilization

When the channel is locked , it assumes someone is using 
else count it as free channel
'''
def log(lock, totalFramesWillBeSent):
    global totalFrames
    tmp = 0
    used = 0
    while totalFrames < totalFramesWillBeSent:
        if lock.locked():
            used += 1
        tmp += 1
    print(f"Channel utilization -> {used/tmp}")

'''
Here all the simulations happen

each object  of CSMA is an node
'''
class CSMA(threading.Thread):
    def __init__(self,lock:threading.Lock, index):
        super().__init__()
        self.lock=lock
        self.index=index
    
    def run(self):
        global numFrames
        global totalFrames
        count=1

        while count <= numFrames:
            print(f"[Attempting] Frame {count} | Node {self.index}")

        
            # Changes  for different modes
            # 1-persistent => Continously try to send the packet

            while self.lock.locked():
                pass

            # non-perstent => Wait for random time before trying to send
            '''
            while self.lock.locked():
                time.sleep(random.randint(2,5))
            '''

            # for p-persistent => it will pick a random probability and if the preferred probability is greater  than actual probaility
            #  then can send data
            # else wait or a random time for next try
            '''
            probability = 1/(no of nodes)
            decision = random.random()
            while decision > probability:
                time.sleep(random.randint(2,6))
                while self.lock.locked():
                    pass
                decision = random.random()
            '''

            # Acquire lock
            self.lock.acquire()
            # Sleep for frame time -- simulation that the frame sending on the log
            time.sleep(frameTime)
            totalFrames += 1
            print(f"[Successful] Frame {count} | Node {self.index}")
            # Release lock
            self.lock.release()
            # Sleep for interframe time
            time.sleep(interFrameTime)
            count+=1

if __name__=='__main__':
    numberNodes = int(input("Enter number of nodes: "))
    numFrames = int(input("Enter number of frames will be sent by frame : "))
    lock = threading.Lock() # Lock object
    nodes = [CSMA(lock,i+1) for i in range(0,numberNodes)] # List of nodes
    # Create log thread
    logThread = threading.Thread(target=log, args=[lock, numberNodes*numFrames])
    logThread.start()
    # Start all threads
    for node in nodes:
        node.start()    
    # Wait for all threads to finish
    for node in nodes:
        node.join()
    # Wait for log thread to finish
    logThread.join()