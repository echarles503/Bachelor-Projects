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
    
    V, E = [int(x) for x in next(sys.stdin).split()]
    edges = [[int(x) for x in line.split()] for line in sys.stdin]
    
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

    for x in adjList:
        for y in x.get_edges():
            print(y[0].id(), y[1])


    res = [0,0]
    dfs(adjList, verify[1], verify[V], res, 0)
    print("longest path: ", res[0])
    print("number of longest paths: ", res[1])


if __name__ == "__main__":
    main()

    

    






    

