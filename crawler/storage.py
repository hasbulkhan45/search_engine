import sqlite3
import json
import time
from typing import Optional, Dict, Any, List


class DocumentStore:
    """
    SQLite-backed persistent store for crawled documents and link graph.
    """

    def __init__(self, db_path: str = "search_engine.db"):
        self.db_path = db_path
        self._init_db()

    def _get_conn(self) -> sqlite3.Connection:
        conn = sqlite3.connect(self.db_path)
        conn.row_factory = sqlite3.Row
        return conn

    def _init_db(self):
        with self._get_conn() as conn:
            conn.execute("""
                CREATE TABLE IF NOT EXISTS documents (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    url TEXT UNIQUE NOT NULL,
                    domain TEXT NOT NULL,
                    title TEXT NOT NULL,
                    body TEXT NOT NULL,
                    meta TEXT,
                    crawled_at REAL NOT NULL,
                    pagerank REAL DEFAULT 1.0
                )
            """)
            conn.execute("""
                CREATE TABLE IF NOT EXISTS links (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    from_url TEXT NOT NULL,
                    to_url TEXT NOT NULL,
                    UNIQUE(from_url, to_url)
                )
            """)
            conn.execute("CREATE INDEX IF NOT EXISTS idx_doc_url ON documents(url)")
            conn.execute("CREATE INDEX IF NOT EXISTS idx_doc_domain ON documents(domain)")
            conn.execute("CREATE INDEX IF NOT EXISTS idx_links_from ON links(from_url)")
            conn.execute("CREATE INDEX IF NOT EXISTS idx_links_to ON links(to_url)")

    def save_document(self, url: str, domain: str, title: str, body: str,
                      meta: Optional[Dict[str, str]] = None,
                      outlinks: Optional[List[str]] = None) -> int:
        """Insert or replace document and record link graph edges."""
        meta_json = json.dumps(meta or {})
        now = time.time()

        with self._get_conn() as conn:
            cursor = conn.cursor()
            cursor.execute("""
                INSERT INTO documents (url, domain, title, body, meta, crawled_at)
                VALUES (?, ?, ?, ?, ?, ?)
                ON CONFLICT(url) DO UPDATE SET
                    title = excluded.title,
                    body = excluded.body,
                    meta = excluded.meta,
                    crawled_at = excluded.crawled_at
            """, (url, domain, title, body, meta_json, now))
            doc_id = cursor.lastrowid or 0

            # If it was an update, get existing id
            if doc_id == 0:
                cur = conn.execute("SELECT id FROM documents WHERE url = ?", (url,))
                row = cur.fetchone()
                doc_id = row["id"] if row else 0

            # Record outlinks
            if outlinks:
                for target in outlinks:
                    conn.execute("""
                        INSERT OR IGNORE INTO links (from_url, to_url)
                        VALUES (?, ?)
                    """, (url, target))

            return doc_id

    def get_document(self, doc_id: int) -> Optional[Dict[str, Any]]:
        with self._get_conn() as conn:
            cur = conn.execute("SELECT * FROM documents WHERE id = ?", (doc_id,))
            row = cur.fetchone()
            if row:
                return dict(row)
            return None

    def get_document_by_url(self, url: str) -> Optional[Dict[str, Any]]:
        with self._get_conn() as conn:
            cur = conn.execute("SELECT * FROM documents WHERE url = ?", (url,))
            row = cur.fetchone()
            if row:
                return dict(row)
            return None

    def count(self) -> int:
        with self._get_conn() as conn:
            cur = conn.execute("SELECT COUNT(*) as cnt FROM documents")
            return cur.fetchone()["cnt"]

    def get_all_documents(self) -> List[Dict[str, Any]]:
        with self._get_conn() as conn:
            cur = conn.execute("SELECT * FROM documents ORDER BY id ASC")
            return [dict(r) for r in cur.fetchall()]

    def get_all_links(self) -> List[Tuple[str, str]]:
        with self._get_conn() as conn:
            cur = conn.execute("SELECT from_url, to_url FROM links")
            return [(r["from_url"], r["to_url"]) for r in cur.fetchall()]

    def update_pagerank(self, url_ranks: Dict[str, float]):
        with self._get_conn() as conn:
            for url, rank in url_ranks.items():
                conn.execute("UPDATE documents SET pagerank = ? WHERE url = ?", (rank, url))
# Performance: PRAGMA journal_mode=WAL;
# Batch: insert_many_documents()
# Graph: count outlinks per document
# Metadata: store http_status
