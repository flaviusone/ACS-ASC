# Scrieti un program in care mai multe threaduri (numarul lor este dat ca
# argument in linia de comanda) aleg random un element dintr-o lista comuna si
# le adauga intr-o lista rezultat (tot comuna).
# a) in fiecare thread, afisati numele threadului si varianta din acel moment 
#    a listei rezultat
# b) in fiecare thread adunati elementul la o variabila globala. Protejati 
#    accesul la aceasta
# c) faceti thread-urile sa astepte un numar random de secunde pana sa inceapa
#    executia. Hint: modulul time
#   

from threading import Thread,current_thread,Lock
from random import randint 
import sys
import time

def thread_function(lista_sursa,lista_dest,lock):
	time.sleep(randint(0,5))
	# iau un element random din lista
	tmp = lista_sursa[randint(0,len(lista_sursa)-1)]
	# adaug la lista comuna elementul ales
	lista_dest.append(tmp)
	print "Sunt thread %s si adaug elementul %d si lista devine: %s\n" % (current_thread().name,tmp,str(lista_dest))
	lock.acquire()
	print "Sunt thread %s si adaug %d la element %d\n" % (current_thread().name,tmp,element)
	global element
	element = element + tmp
	lock.release()


# initializare liste
lista_comuna = [1,2,3,4,5,6,7,8,9,10]
lista_rezultat = []
nr_threads = sys.argv[1]
thread_list = []
element_lock = Lock()
element = 0

for i in range(len(lista_comuna)):
	thread = Thread(target = thread_function, args = (lista_comuna,lista_rezultat,element_lock))		
	thread.start()
	thread_list.append(thread)

for i in range(len(thread_list)):
		thread_list[i].join()
print "Lista finala" + str(lista_rezultat)
print "Element final " + str(element)
