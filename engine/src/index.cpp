#include "index.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace search {

Index::Index() : mu_(std::make_unique<std::mutex>()) {}

Index::Index(Index&& o) noexcept
    : index_(std::move(o.index_)), docs_(std::move(o.docs_)),
      total_tokens_(o.total_tokens_), mu_(std::move(o.mu_)) {
    if (!mu_) mu_ = std::make_unique<std::mutex>();
}

Index& Index::operator=(Index&& o) noexcept {
    if (this != &o) {
        index_ = std::move(o.index_);
        docs_ = std::move(o.docs_);
        total_tokens_ = o.total_tokens_;
        mu_ = std::move(o.mu_);
        if (!mu_) mu_ = std::make_unique<std::mutex>();
    }
    return *this;
}

void Index::add_document(const Document& doc,
                         const std::vector<std::string>& tokens) {
    std::lock_guard<std::mutex> lock(*mu_);
    if (docs_.count(doc.id)) return;  // already indexed
    docs_[doc.id] = doc;
    build_postings(doc.id, tokens);
}

void Index::build_postings(uint32_t doc_id,
                           const std::vector<std::string>& tokens) {
    // Group positions per term
    std::unordered_map<std::string, std::vector<uint32_t>> pos_map;
    for (uint32_t i = 0; i < tokens.size(); ++i)
        pos_map[tokens[i]].push_back(i);

    for (auto& [term, positions] : pos_map) {
        auto& pl = index_[term];
        pl.emplace_back(doc_id, static_cast<uint32_t>(positions.size()));
        pl.back().positions = std::move(positions);
        total_tokens_ += pl.back().freq;
    }
}

void Index::remove_document(uint32_t doc_id) {
    std::lock_guard<std::mutex> lock(*mu_);
    docs_.erase(doc_id);
    for (auto& [term, pl] : index_) {
        pl.erase(std::remove_if(pl.begin(), pl.end(),
                     [doc_id](const Posting& p){ return p.doc_id == doc_id; }),
                 pl.end());
    }
}

void Index::clear() {
    std::lock_guard<std::mutex> lock(*mu_);
    index_.clear();
    docs_.clear();
    total_tokens_ = 0;
}

const PostingList* Index::lookup(const std::string& term) const {
    auto it = index_.find(term);
    return it != index_.end() ? &it->second : nullptr;
}

PostingList Index::intersect(const std::vector<std::string>& terms) const {
    if (terms.empty()) return {};
    auto* pl = lookup(terms[0]);
    if (!pl) return {};
    PostingList result = *pl;

    for (size_t i = 1; i < terms.size() && !result.empty(); ++i) {
        auto* next = lookup(terms[i]);
        if (!next) return {};
        PostingList merged;
        size_t a = 0, b = 0;
        while (a < result.size() && b < next->size()) {
            if (result[a].doc_id == (*next)[b].doc_id) {
                merged.push_back(result[a]);
                ++a; ++b;
            } else if (result[a].doc_id < (*next)[b].doc_id) {
                ++a;
            } else {
                ++b;
            }
        }
        result = std::move(merged);
    }
    return result;
}

PostingList Index::unite(const std::vector<std::string>& terms) const {
    std::unordered_map<uint32_t, Posting*> seen;
    PostingList result;
    for (auto& term : terms) {
        auto* pl = lookup(term);
        if (!pl) continue;
        for (auto& p : *pl) {
            if (!seen.count(p.doc_id)) {
                result.push_back(p);
                seen[p.doc_id] = &result.back();
            }
        }
    }
    return result;
}

double Index::avg_doc_len() const {
    if (docs_.empty()) return 0.0;
    return static_cast<double>(total_tokens_) / docs_.size();
}

const Document* Index::get_doc(uint32_t id) const {
    auto it = docs_.find(id);
    return it != docs_.end() ? &it->second : nullptr;
}

void Index::save(const std::string& path) const {
    std::ofstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open index file: " + path);
    // TODO: binary serialization (Phase 2)
    f << "SEARCH_INDEX_V1\n";
    f << docs_.size() << "\n";
    f << vocab_size() << "\n";
}

void Index::load(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open index file: " + path);
    // TODO: binary deserialization (Phase 2)
}

} // namespace search
