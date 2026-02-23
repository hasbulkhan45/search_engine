#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <ctime>
#include <unordered_map>

namespace search {

/**
 * Represents a single crawled/indexed document.
 */
struct Document {
    uint32_t    id            = 0;
    std::string url;
    std::string title;
    std::string body;
    std::string domain;
    std::time_t crawled_at    = 0;
    std::time_t last_modified = 0;
    double      pagerank      = 0.0;
    size_t      content_len   = 0;

    std::vector<std::string>                       outlinks;
    std::unordered_map<std::string, std::string>   meta;

    Document() = default;
    Document(uint32_t id,
             const std::string& url,
             const std::string& title,
             const std::string& body);

    /// Returns title + body concatenated (title boosted x2)
    std::string full_text() const;

    /// Basic sanity check
    bool is_valid() const;

    void        set_meta(const std::string& key, const std::string& val);
    std::string get_meta(const std::string& key,
                         const std::string& def = "") const;
};

using DocumentList = std::vector<Document>;
using DocumentMap  = std::unordered_map<uint32_t, Document>;

} // namespace search

// Added SimHash fingerprint helper
namespace search {
inline uint64_t calculate_doc_fingerprint(const Document& doc) {
    return HashUtil::simhash_64(doc.body);
}
}
// Cache: document length precomputed
