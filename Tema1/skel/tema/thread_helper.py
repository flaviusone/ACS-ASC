from threading import Event,Thread, current_thread

class Master(Thread):
	def __init__(self, node_id, matrix_size, datastore, nodes):
		Thread.__init__(self, name = "Master")
		self.node_id = node_id
		self.matrix_size = matrix_size
		self.datastore = datastore
		self.nodes = nodes
		self.thread_list = []

	def run(self):
		# print "I am thread %s %d\n" % (str(current_thread()),self.node_id)
		# print self.nodes
		# print "%d %s\n" % (self.matrix_size,str(self.datastore.A_row))

		# main for loop
		for j in range(len(self.nodes)):
			rp = self.find_pivot(j)


	def find_pivot(self,row):
		"""
			Finds pivot
		"""

		return self.datastore.A_row











# class Slave(Thread):
# 	def __init__(self, stuff):
# 		Thread.__init__(self, name = "Slave")

# 	def run(self):
# 		print "Doing nothing"


