import sys

def itr_split(s, word_set):
    """
    Iterative bottom up approach: Finds all paths of valid output
    """
    n = len(s)
    # Initialization:
    paths = [[] for _ in range(n + 1)]
    # Base case:
    paths[0] = ['']
    for i in range(1, n + 1):
        j = i - 1   # Use j as the trailing ptr
        while j >= 0:
            if s[j:i] in word_set and paths[j] != []:
                for gap in paths[j]:
                    # The ternary statement omits a space if it is the first word in a path
                    paths[i].append(gap + (' ' if gap != '' else '') + s[j:i])
            j -= 1
    return paths[n]

def memo_split(s: str, word_set):
    """
    Memoized recursive approach: Finds all paths of valid output
    """
    memo = {}

    def find(string):
        if string in memo:
            return memo[string]
        else:
            # Base case:
            paths = []
            # Further decompose all words that appear in the dictionary
            if string in word_set:
                paths.append(string)
            
            for i in range(len(string)):
                if string[:i] in word_set:
                    # If a prefix exists in the dictionary, check if the rest of the word can be decomposed
                    # If it can't be decomposed, then it will be empty
                    path = find(string[i:])
                    # For each decomposition, prepend the prefix
                    for p in path:
                        paths.append(string[:i] + " " + "".join(p))
            # Update current path
            memo[string] = paths
            return memo[string]
    return find(s)

def main(input):
    n = int(next(sys.stdin))
    lines = [line.strip() for line in sys.stdin]
    dict = set()
    for item in input:
        dict.add(item)

    for i in range(n):
        print("\nphrase number: ", i+1)
        print(lines[i], "\n")
        print("iterative attempt:")
        itr = itr_split(lines[i], dict)
        if itr == []:
            print("NO, cannot be split\n")
        else:
            print("YES, can be split")
            print(itr[0])

        print("\nmemoized attempt:")
        memo = memo_split(lines[i], dict)
        if memo == []:
            print("NO, cannot be split\n")
        else:
            print("YES, can be split")
            print(memo[0])
    return

if __name__ == "__main__":
    with open("diction10k.txt", "r") as f:
        input = [line.strip() for line in f]
    main(input)
