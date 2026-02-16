from bs4 import BeautifulSoup
from urllib.parse import urljoin, urlparse
from typing import List, Tuple, Dict, Any
import re


class HTMLParser:
    """
    Extracts text, title, links, and meta descriptions from raw HTML.
    Strips scripts, styles, and extra whitespace.
    """

    def __init__(self):
        # Tags to discard completely
        self.discard_tags = {"script", "style", "noscript", "svg", "header", "footer", "nav"}

    def parse(self, html_content: str, base_url: str) -> Dict[str, Any]:
        """
        Parses HTML string and returns:
        - title: document title
        - body: cleaned text content
        - links: list of absolute outgoing URLs
        - meta: dict of meta tags (description, keywords, author)
        """
        soup = BeautifulSoup(html_content, "html.parser")

        # Extract title
        title = ""
        if soup.title and soup.title.string:
            title = soup.title.string.strip()

        # Extract meta tags
        meta: Dict[str, str] = {}
        for tag in soup.find_all("meta"):
            name = tag.get("name") or tag.get("property")
            content = tag.get("content")
            if name and content:
                meta[name.lower()] = content.strip()

        # If title is empty, check meta og:title or h1
        if not title:
            title = meta.get("og:title", "")
        if not title:
            h1 = soup.find("h1")
            if h1:
                title = h1.get_text().strip()

        # Extract links before removing nav/footer
        links: List[str] = []
        for a_tag in soup.find_all("a", href=True):
            href = a_tag["href"].strip()
            if not href or href.startswith(("#", "javascript:", "mailto:", "tel:")):
                continue
            abs_url = urljoin(base_url, href)
            parsed = urlparse(abs_url)
            if parsed.scheme in ("http", "https"):
                links.append(abs_url)

        # De-duplicate links while preserving order
        unique_links = list(dict.fromkeys(links))

        # Remove non-content tags
        for tag in soup.find_all(self.discard_tags):
            tag.decompose()

        # Extract clean text
        text = soup.get_text(separator=" ", strip=True)
        # Collapse multiple whitespace characters
        text = re.sub(r"\s+", " ", text).strip()

        return {
            "title": title,
            "body": text,
            "links": unique_links,
            "meta": meta,
        }
