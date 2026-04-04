# Crawling Notes
[2026-04-01] Crawler uses asyncio + aiohttp for concurrent fetching
[2026-04-02] URL frontier: priority queue with domain bucketing
[2026-04-02] Domain bucket caps at 100 queued URLs per domain
[2026-04-03] Bloom filter: k=7 hash functions, m=10^6 bits
[2026-04-03] False positive rate ~0.8% at 100K URLs
[2026-04-04] robots.txt: cached with 1h TTL per domain
[2026-04-04] Crawl-delay from robots.txt respected as min politeness
