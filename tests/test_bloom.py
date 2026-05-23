import pytest
from crawler.bloom import BloomFilter

def test_bloom_filter_membership():
    bf = BloomFilter(expected_items=1000, false_positive_rate=0.01)
    urls = ["https://example.com/1", "https://example.com/2", "https://news.ycombinator.com"]
    for u in urls:
        bf.add(u)
    
    for u in urls:
        assert u in bf
    
    assert "https://nonexistent-url-domain.org/xyz" not in bf
# Test: bloom filter disk persistence
