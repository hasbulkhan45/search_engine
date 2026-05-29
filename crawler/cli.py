import argparse
import sys
from .frontier import URLFrontier
from .crawler import WebCrawler

def main():
    parser = argparse.ArgumentParser(description="Antigravity Web Crawler CLI")
    parser.add_argument("--seed", nargs="+", required=True, help="Seed URLs to crawl")
    parser.add_argument("--max-pages", type=int, default=20, help="Maximum pages to fetch")
    parser.add_argument("--delay", type=float, default=0.5, help="Politeness delay in seconds")
    args = parser.parse_args()

    frontier = URLFrontier(politeness_delay=args.delay)
    for url in args.seed:
        frontier.add_url(url, depth=0)

    crawler = WebCrawler(frontier=frontier)
    print(f"Starting crawl with {len(args.seed)} seed URLs...")
    crawled = crawler.run(max_pages=args.max_pages, on_page_crawled=lambda p: print(f"Crawled: {p['url']} - {p['title']}"))
    print(f"Crawl finished. Successfully indexed {crawled} pages.")

if __name__ == "__main__":
    main()
# CLI: --user-agent option
