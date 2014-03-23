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
		self.event_listner = Event()
		self.job_done = Event()

		# Aici imi pun elemente de alte noduri
		self.payload_bay = None

		self.solution = None

		random.seed(0)

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
		# in valor_pivot primesc date de la celalalte noduri
		# in incomming_buffer primesc de la alt nod un rand intreg
		# in outgoing_buffer creez pachetul pe care il trimit la alt nod
		self.valori_pivot = []
		self.incomming_buffer = []
		self.outgoing_buffer = []
		# Initializez cu 0
		for i in range(self.matrix_size):
			self.valori_pivot.append(0)
			self.incomming_buffer.append(0)
			self.outgoing_buffer.append(0)
		#########################

	def run(self):

		# main for loop
		for j in range(self.matrix_size):
			rp = self.find_pivot(j)

			self.barrier.wait()
			# for i in range(self.matrix_size):
			# 	if self.node_id == i:
			# 		print "%s %s\n" % (self.datastore.A_row,self.datastore.b_elem)
			# 	self.barrier.wait()

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

				# Cer elementul j de la nodul curent
				aux2 = self.datastore.get_A(self.self_node,j)

				# Cer elementul j de la nodul j
				self.nodes[j].listner.set(self.self_node,j)

				# Astept sa il primesc
				self.event_listner.wait()
				self.event_listner.clear()
				aux3 = self.payload_bay

				element -= aux1 * (aux2 / aux3)
				self.datastore.put_b(self.self_node,element)
				############COMPUTE B#############
				self.datastore.put_A(self.self_node,j,0)


		# self.barrier.wait()
		# for i in range(self.matrix_size):
		# 	if self.node_id == i:
		# 		print "%s %s\n" % (self.datastore.A_row,self.datastore.b_elem)
		# 	self.barrier.wait()


		# backward
		for i in range(self.matrix_size-1,-1,-1):
			if self.node_id == i:
				self.solution = self.get_b() / self.get_A(i)
			self.barrier.wait()
			if self.node_id < i:
				self.put_b(self.get_b() - self.nodes[i].thread.solution * self.get_A(i))
				self.put_A(i,0)

		# print "Nod %s sol:%s" % (self.self_node,self.solution)
		self.job_done.set()



	def get_b(self):
		return self.datastore.get_b(self.self_node)

	def get_A(self,column):
		return self.datastore.get_A(self.self_node,column)

	def put_b(self,b):
		self.datastore.put_b(self.self_node,b)
	def put_A(self,column,element):
		self.datastore.put_A(self.self_node,column,element)


	def swap_rows(self,rp,j):
		"""
			Row j swapps with row rp
		"""
		if self.node_id == j:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
			# Trimit row
			self.nodes[rp].thread.incomming_buffer = self.outgoing_buffer
			# Trimit b
			self.nodes[rp].thread.payload_bay = self.datastore.get_b(self.self_node)

		if self.node_id == rp:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
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

			# Compute local max
			maxim = -1
			for k in range(len(self.valori_pivot)):
				if abs(self.valori_pivot[k]) > maxim:
					maxim = self.valori_pivot[k]
					index = k
			if maxim == 0:
				print "Maxim 0 matrice nesingulara"
				print "Am calculat maxim %d pe row %d" % (maxim,index)
			# print maxim

			# broadcast la toata lumea cu indicele gasit ???
			for i in range(self.matrix_size):
				self.nodes[i].thread.payload_bay = index

		self.barrier.wait()

		if (self.node_id != row):
			index = self.payload_bay
		return index


	def send_data_pivot(self,destination_node,data):
		destination_node.thread.valori_pivot[self.node_id] = data

