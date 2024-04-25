import sys

class Pair:
    def __init__(self, first: int, second: int):
        self.v = first
        self.w = second

    def __repr__(self) -> str:
        return f"({self.first}, {self.second})"

class Node:
    def __init__(self, val):
        self.val = val
        self.edges = []
        self.visited = False

    def addEdge(self, dest, weight):
        p = Pair(dest, weight)
        self.edges.append(p)

    def __repr__(self):
        return f"{self.val}"


def longest_path(adj, dist, paths):
    # O(VE) :/
    for node in adj: 
        for edge in node.edges:
            # Index aliases
            u = node.val
            v = edge.v.val
            w = edge.w
            # Optimize iteration time
            if dist[v] > dist[u] + w:
                    continue
            # if distance of child node == distance of parent + child weight:
            # increment child paths by # of parent paths
            if dist[v] == dist[u] + w:
                paths[v] += paths[u]
            # if distance of child node < distance of parent + child weight:
            # increment child dist by (parent dist + w)
            else:
                dist[v] = dist[u] + w
                paths[v] = paths[u]

def main():
    # Read from stdin
    V, E = [int(x) for x in next(sys.stdin).split()]
    edges = [[int(x) for x in line.split()] for line in sys.stdin]
    
    # Initialize adjList, dist/paths arrays
    adj, dist, paths = [], [0] * (V), [0] * (V)
    # Base case
    paths[0] = 1

    # Sort nodes by decreasing order
    for i in range(V):
        adj.append(Node(i))

    # Initialize adjacencies for each node
    for (u, v, w) in edges:
        adj[u-1].addEdge(adj[v-1], w)

    # Execute & Print
    longest_path(adj, dist, paths)
    print("longest path: ", dist[V-1], "\nnumber of longest paths:", paths[V-1])

if __name__ == "__main__":
    main()