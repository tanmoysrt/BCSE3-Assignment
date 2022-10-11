import random
import threading
import time


frameTime = 3
interFrameTime = 1

# Total frames currently sent
totalFrames = 0


def channel(lock, totalFramesWillBeSent):
    global totalFrames
    tmp = 0
    used = 0
    while totalFrames < totalFramesWillBeSent:
        if lock.locked():
            used += 1
        tmp += 1
    print(f"Channel utilization -> {used/tmp}")

class CSMA(threading.Thread):
    def __init__(self,lock, index):
        super().__init__()
        self.lock=lock
        self.index=index
    
    def run(self):
        global numFrames
        global totalFrames
        global backOffTime
        global probability

        count=1

        while count <= numFrames:
            print(f"[Attempting] Frame {count} | Node {self.index}")
            # If locked wait for milliseconds
            while self.lock.locked():
                pass

            decision = random.random()
            while decision>probability:
                print(f"[Waiting] Fram {count} | Node {self.index} | Backoff Time {backOffTime} | Decision {decision}")
                time.sleep(backOffTime)
                while self.lock.locked():
                    pass
                decision=random.random()
                
            # Acquire lock
            self.lock.acquire()
            # Sleep for frame time -- simulation that the frame sending on the channel
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
    backOffTime = int(input("Enter backoff time: "))
    probability = 1/numberNodes

    lock = threading.Lock() # Lock object
    nodes = [CSMA(lock,i+1) for i in range(0,numberNodes)] # List of nodes
    # Create channel thread
    channelThread = threading.Thread(target=channel, args=[lock, numberNodes*numFrames])
    channelThread.start()
    # Start all threads
    for node in nodes:
        node.start()    
    # Wait for all threads to finish
    for node in nodes:
        node.join()
    # Wait for channel thread to finish
    channelThread.join()