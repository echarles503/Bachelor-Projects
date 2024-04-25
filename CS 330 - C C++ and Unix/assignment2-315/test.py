def dfs(adjList, visited, cur, key, cur_length, ans):
    # Check if we have reached our destination
    if cur == key:
        if ans[0] == cur_length:
            ans[1] += 1

        elif ans[0] < cur_length:
            ans[0] = cur_length
            ans[1] = 1
        return
    
    for i in range(len(adjList[cur])):
        if not visited[adjList[cur][i][0]]:
            visited[adjList[cur][i][0]] = True
            dfs(adjList, visited, visited[adjList[cur][i][0]], key, cur_length+adjList[cur][i][1], ans)
            visited[adjList[cur][i][0]] = False


class Node:
    def __init__(self, val):
        self.visited = False
        self.id = val

    def name(self):
        return self.id

    def updateVisit(self):
        self.visited = not(self.visited)

    def getVisit(self):
        return self.visited

class Graph:
    def __init__(self, V: int, edges: list):

        # Initialize nested list to represent an adjacency list
        self.vertex = V
        self.adjList = []

        # Add edges to the directed graph
        for (src, dest, weight) in edges:
            self.adjList[src] = [dest, weight]
            
    
    def test(self):
        print(self.adjList)
