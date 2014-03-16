#a) Folosind clasa Thread, creati 10 threaduri care afiseaza un mesaj de forma:
#    Hello, I'm thread id_thread
from threading import Thread,current_thread

def functie_thread(i):
	print "Hello, I'm thread %d \n" % i

def task_a():
	thread_list = []
	for i in range(10):
		thread = Thread(target = functie_thread, args = (i,))
		thread.start()
		thread_list.append(thread)
	for i in range(len(thread_list)):
		thread_list[i].join()

# print "Task a"
# task_a()

#b) Modificati codul anterior astfel incat thread-urile sa primeasca un 
#   index si un mesaj date ca parametru sub forma de dictionar
#   *hint: exemplu in lab1 http://cs.curs.pub.ro/wiki/asc/asc:lab1:index#functii

def functie_thread2(**dictionar):
	idul = current_thread().name.split('-')[1]
	idul = str(int(idul)-1)
	print "Thread %d: %s\n" % (int(idul),dictionar[idul])

def task_b():
	dict = {}

	thread_list = []
	for i in range(10):
		dict[str(i)] = "Hello, I'm thread " + str(i)
	for i in range(10):
		thread = Thread(target = functie_thread2, kwargs = (dict))
		thread.start()
		thread_list.append(thread)
	for i in range(len(thread_list)):
		thread_list[i].join()

print "Task b"
task_b()


#c) Modificati codul anterior astfel incat thread-urile sa afiseze si numele
#   thread-ului (campul 'name')

def functie_thread3(**dictionar):
	idul = current_thread().name.split('-')[1]
	idul = str(int(idul)-1)
	print "%s : %s\n" % (current_thread().name,dictionar[idul])


def task_c():
	dict = {}

	thread_list = []
	for i in range(10):
		dict[str(i)] = "Hello, I'm thread " + str(i)
	for i in range(10):
		thread = Thread(target = functie_thread3, kwargs = (dict))
		thread.start()
		thread_list.append(thread)
	for i in range(len(thread_list)):
		thread_list[i].join()

# print "Task c"
# task_c()

#d) Modificati codul anterior astfel incat thread-urile sa primeasca index-ul
#   drept nume al thread-ului, afisati-l ca la punctul b)

