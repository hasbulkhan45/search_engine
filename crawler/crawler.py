import time
import requests
from urllib.parse import urlparse
from typing import List, Optional, Callable
import sys
import os

# Import native C++ engine
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
try:
    import search_engine_native as se
except ImportError:
    se = None

from .frontier import URLFrontier
from .robots import RobotsChecker
from .parser import HTMLParser
from .storage import DocumentStore


class WebCrawler:
    """
    Main WebCrawler engine.
    Orchestrates URL frontier, robots politeness, fetching, HTML parsing,
    storing documents in SQLite, and indexing into C++ native inverted index.
    """

    def __init__(
        self,
        frontier: Optional[URLFrontier] = None,
        robots: Optional[RobotsChecker] = None,
        parser: Optional[HTMLParser] = None,
        storage: Optional[DocumentStore] = None,
        native_index: Optional["se.Index"] = None,
        user_agent: str = "AntigravityBot/1.0",
        timeout: float = 5.0,
    ):
        self.frontier = frontier or URLFrontier()
        self.robots = robots or RobotsChecker(user_agent=user_agent)
        self.parser = parser or HTMLParser()
        self.storage = storage or DocumentStore()
        self.native_index = native_index
        self.tokenizer = se.Tokenizer() if se else None
        self.user_agent = user_agent
        self.timeout = timeout
        self.session = requests.Session()
        self.session.headers.update({"User-Agent": self.user_agent})

    def crawl_page(self, url: str, depth: int) -> Optional[dict]:
        """Fetch, parse, store, and index a single URL."""
        if not self.robots.can_fetch(url):
            return None

        try:
            resp = self.session.get(url, timeout=self.timeout)
            if resp.status_code != 200:
                return None

            content_type = resp.headers.get("Content-Type", "")
            if "text/html" not in content_type and "application/xhtml" not in content_type:
                return None

            parsed_data = self.parser.parse(resp.text, base_url=url)
            domain = urlparse(url).netloc

            # Store in SQLite
            doc_id = self.storage.save_document(
                url=url,
                domain=domain,
                title=parsed_data["title"],
                body=parsed_data["body"],
                meta=parsed_data["meta"],
                outlinks=parsed_data["links"]
            )

            # Index into C++ inverted index if available
            if self.native_index and self.tokenizer and parsed_data["body"]:
                doc = se.Document(doc_id, url, parsed_data["title"], parsed_data["body"])
                tokens = self.tokenizer.tokenize(doc.full_text())
                self.native_index.add_document(doc, tokens)

            # Queue outlinks to frontier
            for outlink in parsed_data["links"]:
                self.frontier.add_url(outlink, depth=depth + 1)

            return {
                "id": doc_id,
                "url": url,
                "title": parsed_data["title"],
                "links_found": len(parsed_data["links"])
            }

        except Exception:
            return None

    def run(self, max_pages: int = 50, on_page_crawled: Optional[Callable[[dict], None]] = None) -> int:
        """Run crawler loop up to max_pages."""
        pages_crawled = 0

        while pages_crawled < max_pages:
            item = self.frontier.get_next()
            if item is None:
                break

            url, depth = item
            result = self.crawl_page(url, depth)
            if result:
                pages_crawled += 1
                if on_page_crawled:
                    on_page_crawled(result)

        return pages_crawled
