import sys
import random
from threading import Event,Thread, current_thread, Lock, Semaphore
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
		self.barrier = ReusableBarrierSem(matrix_size)
		random.seed(0)

		# Aici primesc date de la celalalte noduri
		# Initializez cu 0
		self.valori_pivot = []
		for i in range(self.matrix_size):
			self.valori_pivot.append(0)

	def run(self):
		# main for loop
		# self.datastore.get_A(self.nodes[self.node_id],self.node_id)
		for j in range(len(self.nodes)):
			rp = self.find_pivot(j)


	def find_pivot(self,row):
		"""
			Finds pivot
		"""
		if self.node_id != row:
			# Send element from column row to Node row
			self.send_data(self.nodes[row],self.datastore.get_A(self.nodes[self.node_id],row))
			sleep(random.randint(1,3))
		self.barrier.wait()
		print "Thread printing stuff"
		if self.node_id == row:
			# Compute local max
			maxim = -1
			for k in self.valori_pivot:
				if k > maxim:
					maxim = k



		return self.datastore.A_row

	def send_data(self,destination_node,data):
		destination_node.thread.valori_pivot[self.node_id] = data










# class Slave(Thread):
# 	def __init__(self, stuff):
# 		Thread.__init__(self, name = "Slave")

# 	def run(self):
# 		print "Doing nothing"


