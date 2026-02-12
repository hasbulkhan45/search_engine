#pragma once
#include "document.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <mutex>
#include <memory>

namespace search {

/// A single entry in a posting list
struct Posting {
    uint32_t              doc_id;
    uint32_t              freq;       ///< term frequency in document
    std::vector<uint32_t> positions;  ///< byte positions for phrase queries

    Posting(uint32_t doc_id, uint32_t freq)
        : doc_id(doc_id), freq(freq) {}
};

using PostingList   = std::vector<Posting>;
using InvertedIndex = std::unordered_map<std::string, PostingList>;

/**
 * In-memory inverted index.
 * Thread-safe for concurrent reads; single-writer model.
 */
class Index {
public:
    Index();
    Index(Index&& o) noexcept;
    Index& operator=(Index&& o) noexcept;
    Index(const Index&) = delete;
    Index& operator=(const Index&) = delete;

    /// Add a document with its pre-tokenized terms
    void add_document(const Document& doc,
                      const std::vector<std::string>& tokens);

    /// Remove a document by ID (lazy — marks as deleted)
    void remove_document(uint32_t doc_id);

    /// Drop everything
    void clear();

    /// Lookup posting list for a single term (nullptr if not found)
    const PostingList* lookup(const std::string& term) const;

    /// AND-merge posting lists for multiple terms
    PostingList intersect(const std::vector<std::string>& terms) const;

    /// OR-merge posting lists for multiple terms
    PostingList unite(const std::vector<std::string>& terms) const;

    /// Positional phrase search: matches documents where terms appear in exact consecutive order
    PostingList phrase_search(const std::vector<std::string>& terms) const;

    // Stats
    size_t doc_count()   const { return docs_.size(); }
    size_t vocab_size()  const { return index_.size(); }
    double avg_doc_len() const;

    const Document*   get_doc(uint32_t id) const;
    const DocumentMap& documents()         const { return docs_; }

    void save(const std::string& path) const;
    void load(const std::string& path);

private:
    InvertedIndex index_;
    DocumentMap   docs_;
    size_t        total_tokens_ = 0;
    mutable std::unique_ptr<std::mutex> mu_;

    void build_postings(uint32_t doc_id,
                        const std::vector<std::string>& tokens);
};

} // namespace search
