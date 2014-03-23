import sys
import random
from threading import Event,Thread, current_thread, Lock, Semaphore, Condition
from time import sleep
from barrier import ReusableBarrierSem

class Listner(Thread):
	def __init__(self, node_id, matrix_size, datastore, nodes, thread_list):
		Thread.__init__(self, name = str(node_id))
		self.node_id = node_id
		self.matrix_size = matrix_size
		self.datastore = datastore
		self.nodes = nodes
		self.thread_list = thread_list
		self.self_node = self.nodes[self.node_id]
		self.destination_node = None
		self.element = None
		self.event_listner = Event()
		self.semafor = Semaphore(value = 1)
		self.exit = 0

	def run(self):
		while(True):
			# Astept sa primesc o cerere de la alt nod
			self.event_listner.wait()
			# Check if stop thread
			if self.exit==1:
				break
			# Scot element din datastore si il trimit
			# print "%s\n" % self.datastore.A_row
			if self.element == -1:
				self.tmp = self.datastore.get_b(self.self_node)
			else:
				self.tmp = self.datastore.get_A(self.self_node,self.element)
			self.destination_node.thread.payload_bay = self.tmp
			# print "AM PUS BOSS %s\n" % str(self.tmp)
			# Notific nodul destinatie ca am trimis
			self.destination_node.thread.event_listner.set()
			# Clear si reiau bucla
			self.event_listner.clear();
			# Deblochez semafor
			self.semafor.release()


	def set(self,destination_node,element):
		self.semafor.acquire()
		self.destination_node = destination_node
		# print "Setting element %f" % element
		self.element = element

		self.event_listner.set()
		# print "Sunt %s si am primit cerere pentru elementul %d de la %s\n" % (self.node_id, element,str(destination_node))


class Master(Thread):
	def __init__(self, node_id, matrix_size, datastore, nodes,thread_list):
		Thread.__init__(self, name = str(node_id))
		self.node_id = node_id
		self.matrix_size = matrix_size
		self.datastore = datastore
		self.nodes = nodes
		self.thread_list = thread_list
		self.self_node = self.nodes[self.node_id]
		self.listner_thread = self.thread_list[0]

		# Folosit la obtinut elemente de la alte noduri prin thread listener
		self.event_listner = Event()

		# Folosit pentru a bloca functia get_X
		self.job_done = Event()

		# Aici livreaza elemente de alte noduri
		self.payload_bay = None

		# Soluti finala
		self.solution = None

		#########################
		# Creez o bariera locala
		if self.node_id == 0:
			# initializez bariera
			self.barrier = ReusableBarrierSem(self.matrix_size)
		else:
			# setez bariera comuna
			self.barrier = self.nodes[0].thread.barrier
		#########################

		#########################
		# in valor_pivot primesc date de la celalalte noduri despre pivot
		# in incomming_buffer primesc de la alt nod un rand intreg
		# in outgoing_buffer creez pachetul pe care il trimit la alt nod
		# ATENTIE bufferul se creeaza DE FIECARE DATA prin INTEROGARI REPETATE
		self.valori_pivot = [0] * self.matrix_size
		self.incomming_buffer = [0] * self.matrix_size
		self.outgoing_buffer = [0] * self.matrix_size

		#########################

	def run(self):

		# main for loop
		for j in range(self.matrix_size):
			rp = self.find_pivot(j)
			self.barrier.wait()
			self.swap_rows(rp,j)

			if self.node_id > j:

				# Cer elementul j de la nodul curent
				# (am voie sa fac cache la asta conform cerintei pg3 example)
				aux2 = self.datastore.get_A(self.self_node,j)

				# Cer elementul j de la nodul j
				# (am voie sa fac cache la asta conform cerintei pg3 example)
				self.nodes[j].listner.set(self.self_node,j)
				# Astept sa il primesc
				self.event_listner.wait()
				self.event_listner.clear()
				aux3 = self.payload_bay

				for i in range(j+1,self.matrix_size):
					element = self.datastore.get_A(self.self_node,i)

					# Cer elementul i de la nodul j
					self.nodes[j].listner.set(self.self_node,i)
					# Astept sa il primesc
					self.event_listner.wait()
					self.event_listner.clear()
					aux1 = self.payload_bay

					element -= aux1 * (aux2 / aux3)
					self.datastore.put_A(self.self_node,i,element)
				############COMPUTE B#############
				element = self.datastore.get_b(self.self_node)
				# Cer elementul b de la nodul j
				self.nodes[j].listner.set(self.self_node,-1)

				# Astept sa il primesc
				self.event_listner.wait()
				self.event_listner.clear()
				aux1 = self.payload_bay

				element -= aux1 * (aux2 / aux3)
				self.datastore.put_b(self.self_node,element)
				############COMPUTE B#############
				self.datastore.put_A(self.self_node,j,0)

		# backward
		for i in range(self.matrix_size-1,-1,-1):
			if self.node_id == i:
				self.solution = self.get_b() / self.get_A(i)
			self.barrier.wait()
			if self.node_id < i:
				self.put_b(self.get_b() - self.nodes[i].thread.solution * self.get_A(i))
				self.put_A(i,0)

		self.job_done.set()




	def swap_rows(self,rp,j):
		"""
			Face swap la randul rp cu randul j
			@type row: Integer
			@type j: Integer
			@param rp: Randul cu care se face swap (pivotul nou)
			@param j: Randul cu care se face swap
		"""
		thread_list = []
		if self.node_id == j:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				t = Worker(self.self_node,i)
				self.datastore.register_thread(self.self_node,t)
				thread_list.append(t)
				t.start()
			for i in thread_list:
				i.join()
			# for i in range(self.matrix_size):
			# 	self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
			# Trimit row
			self.nodes[rp].thread.incomming_buffer = self.outgoing_buffer
			# Trimit b
			self.nodes[rp].thread.payload_bay = self.datastore.get_b(self.self_node)

		if self.node_id == rp:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				t = Worker(self.self_node,i)
				self.datastore.register_thread(self.self_node,t)
				thread_list.append(t)
				t.start()
			for i in thread_list:
				i.join()
			# for i in range(self.matrix_size):
			# 	self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
			# Trimit row
			self.nodes[j].thread.incomming_buffer = self.outgoing_buffer
			# Trimit b
			self.nodes[j].thread.payload_bay = self.datastore.get_b(self.self_node)

		self.barrier.wait()

		# Updatez linia
		if self.node_id == j:
			for i in range(self.matrix_size):
				self.datastore.put_A(self.self_node,i,self.incomming_buffer[i])
			self.datastore.put_b(self.self_node,self.payload_bay)

		if self.node_id == rp:
			for i in range(self.matrix_size):
				self.datastore.put_A(self.self_node,i,self.incomming_buffer[i])
			self.datastore.put_b(self.self_node,self.payload_bay)

		self.barrier.wait()
		return True

	def find_pivot(self,row):
		"""
			Finds pivot
			@type row: Integer
			@param row: randul pe care se cauta elementul de pe pozitia row

			@return: index-ul rowului care are pivotul
		"""
		index = -1

		if (self.node_id != row) and (self.node_id > row):
			# Trimit element de pe coloana row la nodul row
			self.send_data_pivot(self.nodes[row],self.datastore.get_A(self.self_node,row))

		# Astept sa primesc toate elementele de la celelalte noduri
		self.barrier.wait()

		if self.node_id == row:

			# Pun si valoarea mea in vector
			self.valori_pivot[self.node_id] = self.datastore.get_A(self.self_node,row)

			# Calculez maxim local
			maxim = -1
			for k in range(len(self.valori_pivot)):
				if abs(self.valori_pivot[k]) > maxim:
					maxim = self.valori_pivot[k]
					index = k
			if maxim == 0:
				print "Maxim 0 matrice nesingulara"

			# Broadcast la toata lumea cu indicele
			for i in range(self.matrix_size):
				self.nodes[i].thread.payload_bay = index

		# Astept sa puna latoata lume noul index
		self.barrier.wait()

		# Updatez indexul cu ce am primit de la row
		if (self.node_id != row):
			index = self.payload_bay

		return index


	def send_data_pivot(self,destination_node,data):
		"""
			Pune datale din data in vectorul valori_pivot din nodul destinatie
		"""
		destination_node.thread.valori_pivot[self.node_id] = data

	def get_b(self):
		"""
			Wrapper peste get_b
		"""
		return self.datastore.get_b(self.self_node)

	def get_A(self,column):
		"""
			Wrapper peste get_A
		"""
		return self.datastore.get_A(self.self_node,column)

	def put_b(self,b):
		"""
			Wrapper peste put_b
		"""
		self.datastore.put_b(self.self_node,b)

	def put_A(self,column,element):
		"""
			Wrapper peste put_A
		"""
		self.datastore.put_A(self.self_node,column,element)


class Worker(Thread):
	def __init__(self, node, element):
		Thread.__init__(self, name = str(node))
		self.node = node
		self.element = element

	def run(self):
		self.node.thread.outgoing_buffer[self.element] = self.node.datastore.get_A(self.node,self.element)
