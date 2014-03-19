import sys
import random
from threading import Event,Thread, current_thread, Lock, Semaphore, Condition
from time import sleep
from barrier import ReusableBarrierSem

class Master(Thread):
	def __init__(self, node_id, matrix_size, datastore, nodes):
		Thread.__init__(self, name = str(node_id))
		self.node_id = node_id
		self.matrix_size = matrix_size
		self.datastore = datastore
		self.nodes = nodes
		self.thread_list = []
		self.self_node = self.nodes[self.node_id]
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
		# self.datastore.get_A(self.nodes[self.node_id],self.node_id)
		for j in range(self.matrix_size):
			rp = self.find_pivot(j)
			self.swap_rows(rp,j)

	def swap_rows(self,rp,j):
		"""
			Row j swapps with row rp
		"""
		if self.node_id == j:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
			#Trimit
			self.nodes[rp].thread.incomming_buffer = self.outgoing_buffer

		if self.node_id == rp:
			# Creez buffer pt trimitere
			for i in range(self.matrix_size):
				self.outgoing_buffer[i] = self.datastore.get_A(self.self_node,i)
			# Trimit
			self.nodes[j].thread.incomming_buffer = self.outgoing_buffer
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

		if self.node_id != row:
			# Trimit element de pe coloana row la nodul row
			self.send_data_pivot(self.nodes[row],self.datastore.get_A(self.self_node,row))

		# Astept sa primesc toate elementele de la celelalte noduri
		self.barrier.wait()

		if self.node_id == row:
			# Pun si valoarea mea in vector
			self.valori_pivot[self.node_id] = self.datastore.get_A(self.self_node,row)

			# Compute local max
			maxim = -1
			index = -1
			for k in range(len(self.valori_pivot)):
				if abs(self.valori_pivot[k]) > maxim:
					maxim = self.valori_pivot[k]
					index = k
			print "Am calculat maxim %d pe row %d" % (maxim,index)
			# print maxim

		return index


	def send_data_pivot(self,destination_node,data):
		destination_node.thread.valori_pivot[self.node_id] = data
		# .append((data,self.node_id))










# class Slave(Thread):
# 	def __init__(self, stuff):
# 		Thread.__init__(self, name = "Slave")

# 	def run(self):
# 		print "Doing nothing"


