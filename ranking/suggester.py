from typing import List, Dict, Tuple

class TrieNode:
    def __init__(self):
        self.children: Dict[str, TrieNode] = {}
        self.is_terminal: bool = False
        self.frequency: int = 0
        self.query: str = ""

class QuerySuggester:
    """
    Prefix Trie query autocomplete suggester ranked by search frequency.
    """
    def __init__(self):
        self.root = TrieNode()

    def insert(self, query: str, frequency: int = 1):
        node = self.root
        q_clean = query.strip().lower()
        for char in q_clean:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.is_terminal = True
        node.frequency += frequency
        node.query = q_clean

    def suggest(self, prefix: str, max_suggestions: int = 5) -> List[str]:
        node = self.root
        p_clean = prefix.strip().lower()
        for char in p_clean:
            if char not in node.children:
                return []
            node = node.children[char]

        results: List[Tuple[int, str]] = []
        self._dfs(node, results)
        results.sort(key=lambda x: x[0], reverse=True)
        return [q for _, q in results[:max_suggestions]]

    def _dfs(self, node: TrieNode, results: List[Tuple[int, str]]):
        if node.is_terminal:
            results.append((node.frequency, node.query))
        for child in node.children.values():
            self._dfs(child, results)
