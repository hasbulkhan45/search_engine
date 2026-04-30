import urllib.robotparser
from urllib.parse import urlparse
import requests
from typing import Dict, Optional


class RobotsChecker:
    """
    Parses and caches robots.txt files per domain to ensure compliance.
    """

    def __init__(self, user_agent: str = "SearchEngineBot/1.0", timeout: float = 3.0):
        self.user_agent = user_agent
        self.timeout = timeout
        self.cache: Dict[str, urllib.robotparser.RobotFileParser] = {}

    def _fetch_robots_txt(self, domain: str, scheme: str = "https") -> urllib.robotparser.RobotFileParser:
        rp = urllib.robotparser.RobotFileParser()
        robots_url = f"{scheme}://{domain}/robots.txt"
        try:
            resp = requests.get(
                robots_url,
                headers={"User-Agent": self.user_agent},
                timeout=self.timeout
            )
            if resp.status_code == 200:
                rp.parse(resp.text.splitlines())
            else:
                # 404 or other: assume all allowed
                rp.allow_all = True
        except Exception:
            # On network timeout or connection error, allow crawling politely
            rp.allow_all = True

        self.cache[domain] = rp
        return rp

    def can_fetch(self, url: str) -> bool:
        """Check if the crawler is allowed to fetch the given URL."""
        parsed = urlparse(url)
        domain = parsed.netloc
        if not domain:
            return False

        rp = self.cache.get(domain)
        if rp is None:
            rp = self._fetch_robots_txt(domain, parsed.scheme or "https")

        try:
            return rp.can_fetch(self.user_agent, url)
        except Exception:
            return True
# Directive: Crawl-Delay parsing
# Feature: Sitemap link discovery
