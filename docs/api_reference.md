# REST API Reference

Base URL: `http://localhost:8000`

### 1. Search Query
`GET /api/v1/search`

Parameters:
- `q` (string, required): Query string
- `limit` (int, default=10): Results per page
- `offset` (int, default=0): Pagination offset
- `ranker` (string, default="bm25"): `bm25` or `tfidf`

### 2. Index Statistics
`GET /api/v1/index/stats`

Response:
```json
{
  "doc_count": 10542,
  "vocab_size": 48210,
  "avg_doc_len": 184.5
}
```

### 3. Add Document
`POST /api/v1/index/document`

Header: `X-API-Key: your-api-key`

Body:
```json
{
  "url": "https://example.com",
  "title": "Example Title",
  "body": "Full body text content..."
}
```
