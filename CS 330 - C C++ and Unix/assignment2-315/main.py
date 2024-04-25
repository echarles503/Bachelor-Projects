#!/usr/bin/python
import sys

class Node:
    def __init__(self, val):
        self.val = val
        self.edges = []
        self.visited = False

    def id(self):
        return self.val

    def add_edge(self, dest, weight):
        self.edges.append([dest, weight])

    def get_edges(self):
        return self.edges
    
    def update_visit(self):
        self.visited = not(self.visited)

    def get_visit(self):
        return self.visited

class Graph:
    def __init__(self, V: int, E: list):
        self.verify = {}
        self.adjList = []


        for i in range(V):
            self.verify[i+1] = Node(i+1)
        for (src, dest, weight) in E:
            # Avoid duplicate nodes, use src/dest as a lookup id
            a = self.verify[src]
            b = self.verify[dest]
            a.add_edge(b, weight)

        for i in range(V):
            self.adjList.append(self.verify[i+1])

    def addEdge(self, u, v, w):
        self.graph[u]

def dfs(adjList, current, node_N, res, path):
    if current == node_N:
        if path == res[0]:
            res[1] += 1
        elif path > res[0]:
            res[0] = path
            res[1] = 1
        return
 
    for y in current.get_edges():
        if y[0].get_visit() is False:
            y[0].update_visit()
            path += y[1]
            dfs(adjList, y[0], node_N, res, path)
            path -= y[1]
            y[0].update_visit()


def main():
    with open(sys.argv[1],'r') as f:
        V, E = [int(x) for x in next(f).split()]
        edges = [[int(x) for x in line.split()] for line in f]
    
    adjList = [ ]
    verify = {}
    for i in range(V):
        verify[i+1] = Node(i+1)
    for (src, dest, weight) in edges:
        # Avoid duplicate nodes, use src/dest as a lookup id
        a = verify[src]
        b = verify[dest]
        a.add_edge(b, weight)

    for i in range(V):
        adjList.append(verify[i+1])

    res = [0,0]

    dfs(adjList, verify[1], verify[3], res, 0)
    print("longest path: ", res[0])
    print("number of longest paths: ", res[1])


if __name__ == "__main__":
    main()

    

    






    

