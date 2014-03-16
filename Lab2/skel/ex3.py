# Implementare producator-consumator

from threading import Thread,Semaphore,current_thread,Lock
from random import randint
import sys
import time


def producator(sem1):
	global buff
	while(1):
		sem1.acquire()
		while(1):
			if(len(buff)<10):
				break
		produs  = randint(1,100)
		print "Thread-ul %s produce element %d \n" % (current_thread().name,produs)
		buff.append(produs)
		print buff
 		sem1.release()
 		time.sleep(1)

def consumator(sem):
	global buff
	while(1):
		sem.acquire()
		if not buff:
			print "Lista e goala nu consum nimic\n"
			sem.release()
		else:	
			consuma = buff.pop()
			print "Threadul %s consuma un element %d \n" % (current_thread().name,consuma)
			sem.release()
		time.sleep(1)


buff = []
semafor1 = Semaphore(value = 3)
semafor2 = Semaphore(value = 1)

for i in range(5):
	thread = Thread(target = producator, args = (semafor1,))
	thread.start()


for i in range(5):
	thread = Thread(target = consumator, args = (semafor2,))
	thread.start()

