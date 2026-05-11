import pytest
import os
import tempfile
from crawler.frontier import URLFrontier
from crawler.parser import HTMLParser
from crawler.storage import DocumentStore


def test_url_frontier_normalization():
    frontier = URLFrontier(max_depth=3)
    assert frontier.normalize_url("HTTP://EXAMPLE.COM/path/#frag") == "http://example.com/path"
    assert frontier.normalize_url("https://example.com:443/a/b/") == "https://example.com:443/a/b"

    assert frontier.add_url("https://example.com/page1", depth=0) is True
    # Duplicate should be rejected
    assert frontier.add_url("https://example.com/page1", depth=1) is False
    assert frontier.total_seen() == 1
    assert frontier.size() == 1


def test_html_parser_extraction():
    parser = HTMLParser()
    sample_html = """
    <!DOCTYPE html>
    <html>
    <head>
        <title>Test Page Title</title>
        <meta name="description" content="This is a test description.">
    </head>
    <body>
        <script>var x = 10;</script>
        <h1>Main Header</h1>
        <p>This is the first paragraph with some valuable text.</p>
        <a href="/about">About Us</a>
        <a href="https://external.org/info">External Info</a>
    </body>
    </html>
    """
    res = parser.parse(sample_html, base_url="https://mysite.com/blog/")
    assert res["title"] == "Test Page Title"
    assert res["meta"].get("description") == "This is a test description."
    assert "var x = 10" not in res["body"]
    assert "valuable text" in res["body"]
    assert "https://mysite.com/about" in res["links"]
    assert "https://external.org/info" in res["links"]


def test_document_store_sqlite():
    with tempfile.NamedTemporaryFile(suffix=".db", delete=False) as tmp:
        db_path = tmp.name

    try:
        store = DocumentStore(db_path=db_path)
        doc_id = store.save_document(
            url="https://docstore.io/1",
            domain="docstore.io",
            title="Doc 1",
            body="Storage layer test body",
            meta={"tag": "unit_test"},
            outlinks=["https://docstore.io/2"]
        )
        assert doc_id > 0
        assert store.count() == 1

        fetched = store.get_document(doc_id)
        assert fetched is not None
        assert fetched["title"] == "Doc 1"
        assert fetched["domain"] == "docstore.io"

        links = store.get_all_links()
        assert len(links) == 1
        assert links[0] == ("https://docstore.io/1", "https://docstore.io/2")
    finally:
        if os.path.exists(db_path):
            os.remove(db_path)
# Test: canonical link tag precedence
