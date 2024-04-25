from cmath import inf
import sys
from queue import PriorityQueue



class Node:
    def __init__(self, val):
        self.val = val
        self.edges = list()
        self.visited = False

    def id(self):
        return self.val

    def addEdge(self, dest, weight):
        self.edges.append([dest, weight])

    def getEdges(self):
        return self.edges
    
    def updateVisit(self):
        self.visited = not(self.visited)

    def getVisit(self):
        return self.visited
    
    def __repr__(self):
        return f"{self.val}"
 
class Graph:

    def __init__(self, V: int, E: list):
        self.vertices = V
        self.nodes = {}
        self.adj = []

        for i in range(V):
            self.nodes[i] = Node(i)

        for (u, v, w) in E:
            self.nodes[u-1].addEdge(self.nodes[v-1], w)

    def printAdj(self):
        for i in range(self.vertices):
            print(self.nodes[i],": ", end='')
            for j in self.nodes[i].getEdges():
                print(j[0], "/ ", end='')
            print()

    def dijkstra(self):
        dist = [0] * self.vertices     # Store max distance to ith node in dist
        paths = [0] * self.vertices    # Initialize path length container
        q = PriorityQueue()            # Initialize p_queue
        q.put([0, self.nodes[0]])      # Add src node to queue
        dist[0] = 0                    # Distance from src to destination = 0
        paths[0] = 1

        while q.qsize() != 0:
            current = q.get()           
            curr = current[1].val
            length = dist[curr]     # Max path length of current node
            print(q.queue)
            for v in current[1].getEdges():
                #print("current:", curr, "neighbor:", v)
                node, child = v[0], v[0].val
                cost = v[1]
                #print(cost)
                if dist[child] > (dist[curr] + cost):
                    continue
                
                if dist[child] == (dist[curr] + cost):
                    paths[child] += paths[curr]
                    #print("updating ==", child, "->", paths[child], "-", paths[curr])
                if dist[child] < (dist[curr] + cost):
                    q.put([-(length+cost), node])
                    #print("adding:", node)
                    dist[child] = (dist[curr] + cost)
                    paths[child] = paths[curr]
                    #print("updating <", child, ":", paths[child])
                    #print("current: ", curr, v)
        return print(paths, dist)
        

def main():
    with open(sys.argv[1],'r') as f:
        V, E = [int(x) for x in next(f).split()]
        edges = [[int(x) for x in line.split()] for line in f]

    graph = Graph(V, edges)
    #graph.printAdj()
    graph.dijkstra()
main()
