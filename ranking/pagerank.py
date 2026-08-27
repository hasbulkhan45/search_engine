from typing import Dict, List, Tuple
from collections import defaultdict

class PageRank:
    """
    Power-iteration PageRank algorithm on directed web link graph.
    """
    def __init__(self, damping_factor: float = 0.85, max_iterations: int = 50, tolerance: float = 1e-6):
        self.d = damping_factor
        self.max_iter = max_iterations
        self.tol = tolerance

    def compute(self, edges: List[Tuple[str, str]], all_nodes: List[str] = None) -> Dict[str, float]:
        nodes = set(all_nodes or [])
        out_edges = defaultdict(list)
        in_edges = defaultdict(list)

        for src, dst in edges:
            nodes.add(src)
            nodes.add(dst)
            out_edges[src].append(dst)
            in_edges[dst].append(src)

        N = len(nodes)
        if N == 0:
            return {}

        rank = {n: 1.0 / N for n in nodes}

        for _ in range(self.max_iter):
            new_rank = {}
            # Dangled nodes sum
            dangle_sum = sum(rank[n] for n in nodes if len(out_edges[n]) == 0)

            diff = 0.0
            for n in nodes:
                inbound_sum = sum(rank[src] / len(out_edges[src]) for src in in_edges[n])
                new_val = (1.0 - self.d) / N + self.d * (inbound_sum + dangle_sum / N)
                diff += abs(new_val - rank[n])
                new_rank[n] = new_val

            rank = new_rank
            if diff < self.tol:
                break

        return rank
# Config: configurable damping factor
# Matrix: export sparse adjacency format
# Telemetry: log iteration count and delta
