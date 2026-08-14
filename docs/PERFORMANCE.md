# Performance Notes
[2026-03-05] Indexing: 85,001 docs/sec | Query: <15µs avg
[2026-03-19] BM25 query on 10K docs: avg 8µs latency
[2026-03-25] Compression: delta enc reduces 10M postings from 80MB to 28MB
[2026-03-30] Avg BM25 score calc: 95ns per doc per term
[2026-04-12] Crawler: ~5 pages/sec with 1s politeness delay
[2026-04-28] API: median search response time 3.4ms on local
[2026-05-07] PageRank on 10K nodes: converges in 47 iterations, 12ms
[2026-08-14] E2E benchmark: 85K docs/sec index, <15µs query, 3.4ms API
