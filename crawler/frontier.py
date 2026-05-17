from collections import deque
from urllib.parse import urlparse, urldefrag
import time
from typing import Optional, Set, Tuple


class URLFrontier:
    """
    URL Frontier manages the crawl queue, tracking visited URLs,
    imforcing crawl depth, and respecting per-domain politeness delays.
    """

    def __init__(self, politeness_delay: float = 0.5, max_depth: int = 5):
        self.politeness_delay = politeness_delay
        self.max_depth = max_depth
        self.queue: deque[Tuple[str, int]] = deque()
        self.seen_urls: Set[str] = set()
        self.last_domain_access: dict[str, float] = {}

    @staticmethod
    def normalize_url(url: str) -> str:
        """Strip fragments and normalize scheme and trailing slash."""
        clean_url, _ = urldefrag(url)
        clean_url = clean_url.strip()
        parsed = urlparse(clean_url)
        scheme = parsed.scheme.lower()
        netloc = parsed.netloc.lower()
        path = parsed.path
        if not path:
            path = "/"
        elif len(path) > 1 and path.endswith("/"):
            path = path[:-1]
        query = f"?{parsed.query}" if parsed.query else ""
        return f"{scheme}://{netloc}{path}{query}"

    def add_url(self, url: str, depth: int = 0) -> bool:
        """Add a URL to the frontier if not already visited or queued."""
        if depth > self.max_depth:
            return False

        norm_url = self.normalize_url(url)
        parsed = urlparse(norm_url)
        if parsed.scheme not in ("http", "https"):
            return False
        if not parsed.netloc:
            return False

        if norm_url in self.seen_urls:
            return False

        self.seen_urls.add(norm_url)
        self.queue.append((norm_url, depth))
        return True

    def get_next(self) -> Optional[Tuple[str, int]]:
        """
        Pops the next URL ready for crawling while respecting
        domain politeness delay.
        """
        if not self.queue:
            return None

        # Check queue elements for one whose domain politeness is satisfied
        now = time.time()
        for _ in range(len(self.queue)):
            url, depth = self.queue.popleft()
            domain = urlparse(url).netloc
            last_access = self.last_domain_access.get(domain, 0.0)

            if now - last_access >= self.politeness_delay:
                self.last_domain_access[domain] = now
                return url, depth
            else:
                # Put back at tail
                self.queue.append((url, depth))

        # If all domains are in politeness cooldown, pop the head and update time
        url, depth = self.queue.popleft()
        domain = urlparse(url).netloc
        self.last_domain_access[domain] = time.time()
        return url, depth

    def size(self) -> int:
        return len(self.queue)

    def total_seen(self) -> int:
        return len(self.seen_urls)
# Filter: ignore non-http schemes
# Politeness: token bucket per netloc
# Normalizer: handle //example.com/path
