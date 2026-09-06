from typing import List, Dict, Any

class MLReranker:
    """
    Feature-based machine learning re-ranker.
    Combines BM25 lexical score, PageRank authority, URL depth, and exact title matches.
    """
    def __init__(self, w_bm25: float = 0.55, w_pagerank: float = 0.25, w_title_exact: float = 0.15, w_url_depth: float = 0.05):
        self.w_bm25 = w_bm25
        self.w_pagerank = w_pagerank
        self.w_title_exact = w_title_exact
        self.w_url_depth = w_url_depth

    def rerank(self, query: str, candidate_hits: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
        if not candidate_hits:
            return []

        q_lower = query.lower()
        scored = []

        max_bm25 = max((h.get("score", 0.0) for h in candidate_hits), default=1.0) or 1.0

        for h in candidate_hits:
            norm_bm25 = h.get("score", 0.0) / max_bm25
            pr = min(1.0, h.get("pagerank", 1.0) * 5.0)
            title = h.get("title", "").lower()
            title_exact = 1.0 if q_lower in title else 0.0
            url = h.get("url", "")
            depth = max(1, url.count("/"))
            depth_score = 1.0 / depth

            final_score = (
                self.w_bm25 * norm_bm25 +
                self.w_pagerank * pr +
                self.w_title_exact * title_exact +
                self.w_url_depth * depth_score
            )

            item = dict(h)
            item["ml_score"] = round(final_score, 4)
            scored.append(item)

        scored.sort(key=lambda x: x["ml_score"], reverse=True)
        return scored
# CTR: integrate click counts into feature vector
# Anchor: inbound anchor text bonus
# Weights: set_weights(bm25, pr, title)
# Reputation: whitelist high-authority domains
# Freshness: exponential time decay
