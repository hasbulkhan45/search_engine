#include "snippet.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace search {

std::string SnippetGenerator::to_lower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return out;
}

Snippet SnippetGenerator::generate(const std::string& document_text,
                                   const std::vector<std::string>& query_terms) const {
    Snippet snip;
    if (document_text.empty() || query_terms.empty()) {
        snip.text = document_text.substr(0, std::min(document_text.size(), max_len_));
        return snip;
    }

    std::string lower_doc = to_lower(document_text);
    size_t best_pos = std::string::npos;

    // Find the first occurrence of any query term
    for (const auto& term : query_terms) {
        if (term.empty()) continue;
        std::string lower_t = to_lower(term);
        size_t pos = lower_doc.find(lower_t);
        if (pos != std::string::npos && (best_pos == std::string::npos || pos < best_pos)) {
            best_pos = pos;
        }
    }

    if (best_pos == std::string::npos) {
        snip.text = document_text.substr(0, std::min(document_text.size(), max_len_));
        if (document_text.size() > max_len_) snip.text += "...";
        return snip;
    }

    snip.has_match = true;

    // Center window around match
    size_t start = (best_pos > max_len_ / 2) ? best_pos - max_len_ / 2 : 0;
    // Align to word boundary
    if (start > 0) {
        size_t space = document_text.rfind(' ', start);
        if (space != std::string::npos && space + 1 >= (start > 15 ? start - 15 : 0)) {
            start = space + 1;
        }
    }

    size_t length = std::min(max_len_, document_text.size() - start);
    std::string window = document_text.substr(start, length);

    // Add ellipsis if truncated
    std::string result;
    if (start > 0) result += "...";
    result += window;
    if (start + length < document_text.size()) result += "...";

    snip.text = std::move(result);
    return snip;
}

} // namespace search
// Perf: rfind for sentence boundaries
// Style: customizable highlight open/close tags
