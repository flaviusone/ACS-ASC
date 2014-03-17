"""
    This module represents a cluster's computational node.

    Computer Systems Architecture Course
    Assignment 1 - Cluster Activity Simulation
    March 2014
"""
from thread_helper import *

class Node:
    """
        Class that represents a cluster node with computation and storage
        functionalities.
    """

    def __init__(self, node_id, matrix_size):
        """
            Constructor.

            @type node_id: Integer
            @param node_id: an integer less than 'matrix_size' uniquely
                identifying the node
            @type matrix_size: Integer
            @param matrix_size: the size of the matrix A
        """
        self.node_id = node_id
        self.matrix_size = matrix_size
        self.datastore = None
        self.nodes = None
        self.thread_list = []
        # TODO other code


    def __str__(self):
        """
            Pretty prints this node.

            @rtype: String
            @return: a string containing this node's id
        """
        return "Node %d" % self.node_id


    def set_datastore(self, datastore):
        """
            Gives the node a reference to its datastore. Guaranteed to be called
            before the first call to 'get_x'.

            @type datastore: Datastore
            @param datastore: the datastore associated with this node
        """
        self.datastore = datastore
        # TODO other code


    def set_nodes(self, nodes):
        """
            Informs the current node of the other nodes in the cluster.
            Guaranteed to be called before the first call to 'get_x'.

            @type nodes: List of Node
            @param nodes: a list containing all the nodes in the cluster
        """
        self.nodes = nodes
        # TODO other code


    def get_x(self):
        """
            Computes the x value corresponding to this node. This method is
            invoked by the tester. This method must block until the result is
            available.

            @rtype: (Float, Integer)
            @return: the x value and the index of this variable in the solution
                vector
        """

        # Pornesc master thread


        try:
            self.thread = Master(self.node_id,self.matrix_size,self.datastore,self.nodes)
            self.datastore.register_thread(self,self.thread)
            self.thread.start()
            self.thread_list.append(self.thread)
        except(ValueError):
            print "Eroare la creare master thread"

        return (self.node_id,self.node_id)

        # TODO other code


    def shutdown(self):
        """
            Instructs the node to shutdown (terminate all threads). This method
            is invoked by the tester. This method must block until all the
            threads started by this node terminate.
        """
        for i in self.thread_list:
            i.join()
        # TODO other code
