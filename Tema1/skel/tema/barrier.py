from threading import Event,Thread, current_thread, Lock, Semaphore

class ReusableBarrierSem():

    def __init__(self, num_threads):
        self.num_threads = num_threads
        self.count_threads1 = self.num_threads
        self.count_threads2 = self.num_threads

        self.counter_lock = Lock()       # protejam decrementarea numarului de threaduri
        self.threads_sem1 = Semaphore(0) # contorizam numarul de threaduri pentru prima etapa
        self.threads_sem2 = Semaphore(0) # contorizam numarul de threaduri pentru a doua etapa

    def wait(self):
        self.phase1()
        self.phase2()

    def phase1(self):
        with self.counter_lock:
            self.count_threads1 -= 1
            if self.count_threads1 == 0:
                for i in range(self.num_threads):
                    self.threads_sem1.release()
            self.count_threads2 = self.num_threads

        self.threads_sem1.acquire()

    def phase2(self):
        with self.counter_lock:
            self.count_threads2 -= 1
            if self.count_threads2 == 0:
                for i in range(self.num_threads):
                    self.threads_sem2.release()
            self.count_threads1 = self.num_threads

        self.threads_sem2.acquire()

# class MyThread(Thread):
#     def __init__(self, tid, barrier):
#         Thread.__init__(self)
#         self.tid = tid
#         self.barrier = barrier

#     def run(self):
#         for i in range(10):
#             barrier.wait()
#             print "I'm Thread " + str(self.tid) + " after barrier, in step " + str(i)
