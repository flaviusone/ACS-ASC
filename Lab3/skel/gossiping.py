"""
Implementati o propagare ciclica de tip gossiping folosind bariere. 
  * Se considera N noduri (obiecte de tip Thread), cu indecsi 0...N-1
  * Fiecare nod tine o valoare generata random 
  * Calculati valoarea minima folosind urmatorul procedeu:
     * nodurile ruleaza in cicluri
     * intr-un ciclu, fiecare nod comunica cu un subset de alte noduri pentru a
       obtine valoarea acestora si a o compara cu a sa
       * ca subset considerati random 3 noduri din lista de noduri primita in
        constructor si obtineti valoarea acestora (metoda get_value)
     * dupa terminarea unui ciclu, fiecare nod va avea ca valoare minimul 
       obtinut in ciclul anterior
     * la finalul iteratiei toate nodurile vor contine valoarea minima 
  * Folositi una din barierele reentrante din modulul barrier
  * Pentru a simplifica implementarea, e folosit un numar fix de cicluri, 
    negarantand astfel convergenta tutoror nodurilor la acelasi minim
"""
import sys
import random
from threading import enumerate, Event, Thread, Lock, Semaphore
from time import sleep

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


random.seed(0) # genereaza tot timpul aceeasi secventa pseudo-random

class Node(Thread):
    #TODO Node trebuie sa fie Thread

    def __init__(self, node_id, other_nodes, num_iter):
        #TODO nodurile trebuie sa foloseasca un obiect bariera

        Thread.__init__(self)
        self.node_id = node_id
        self.other_nodes = other_nodes
        self.num_iter = num_iter
        self.value = random.randint(1,1000)
        print "Init thread %d with val %d" % (self.node_id,self.value)
         
    def run(self):
      barrier.wait()
      for i in xrange(num_iter):
        list = []
        list.append(self.other_nodes[random.randint(1,len(self.other_nodes)-1)].get_value())
        list.append(self.other_nodes[random.randint(1,len(self.other_nodes)-1)].get_value())
        list.append(self.other_nodes[random.randint(1,len(self.other_nodes)-1)].get_value())
        self.value = min(list)
        print "Valoare minima thead %d dupa %d iteratii = %d\n" % (self.node_id,i,self.value)  
        barrier.wait()
      # print "Valoare minima finala %d\n" % self.value  
         
    def get_value(self):
        return self.value

if __name__ == "__main__":
    if len(sys.argv) == 2:
        num_threads = int(sys.argv[1])
    else: 
        print "Usage: python "+sys.argv[0]+" num_nodes"
        sys.exit(0)

    barrier = ReusableBarrierSem(num_threads)

    num_iter = 10  # numar iteratii/cicluri algoritm
    thread_list=[]
    #TODO creare si pornire noduri
    for i in xrange(num_threads):
      nod = Node(i,thread_list,num_iter)
      nod.start()
      thread_list.append(nod)
    for nod in thread_list:
      nod.join()

    #TODO asteptati terminarea executiei nodurilor
