#!/usr/bin/env python3
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from crawler.storage import DocumentStore

SAMPLE_DATA = [
    {
        "url": "https://en.wikipedia.org/wiki/Search_engine",
        "domain": "en.wikipedia.org",
        "title": "Search engine (computing) - Wikipedia",
        "body": "A search engine is an information retrieval software system designed to help find information stored on a computer system or the World Wide Web. Search results are generally presented in a list of results, often referred to as search engine results pages (SERPs)."
    },
    {
        "url": "https://en.wikipedia.org/wiki/Inverted_index",
        "domain": "en.wikipedia.org",
        "title": "Inverted index - Wikipedia",
        "body": "In computer science, an inverted index is a database index storing a mapping from content, such as words or numbers, to its locations in a document or a set of documents. The purpose of an inverted index is to allow very fast full-text searches."
    },
    {
        "url": "https://en.wikipedia.org/wiki/Okapi_BM25",
        "domain": "en.wikipedia.org",
        "title": "Okapi BM25 - Wikipedia",
        "body": "In information retrieval, Okapi BM25 is a ranking function used by search engines to estimate the relevance of documents to a given search query. It is based on the probabilistic retrieval framework developed in the 1970s and 1980s by Stephen E. Robertson, Karen Spärck Jones, and others."
    },
    {
        "url": "https://en.wikipedia.org/wiki/PageRank",
        "domain": "en.wikipedia.org",
        "title": "PageRank - Wikipedia",
        "body": "PageRank is an algorithm used by Google Search to rank web pages in their search engine results. It is named after Larry Page, one of the founders of Google. PageRank is a way of measuring the importance of website pages."
    },
    {
        "url": "https://docs.python.org/3/library/asyncio.html",
        "domain": "docs.python.org",
        "title": "asyncio - Asynchronous I/O - Python Documentation",
        "body": "asyncio is a library to write concurrent code using the async/await syntax. asyncio is used as a foundation for multiple Python asynchronous frameworks that provide high-performance network and web-servers, database connection libraries, and distributed task queues."
    }
]

def seed():
    store = DocumentStore("search_engine.db")
    print(f"Seeding {len(SAMPLE_DATA)} foundational documents into search_engine.db...")
    for item in SAMPLE_DATA:
        doc_id = store.save_document(
            url=item["url"],
            domain=item["domain"],
            title=item["title"],
            body=item["body"]
        )
        print(f"  Indexed [{doc_id}]: {item['title']}")
    print("Database seeding completed.")

if __name__ == "__main__":
    seed()
