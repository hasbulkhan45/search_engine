#pragma once
#include <string>
#include <vector>
#include <unordered_set>

namespace search {

struct Snippet {
    std::string text;
    bool has_match = false;
};

/**
 * Generates search result snippets around query match terms with <em></em> highlighting.
 */
class SnippetGenerator {
public:
    explicit SnippetGenerator(size_t max_snippet_length = 160)
        : max_len_(max_snippet_length) {}

    Snippet generate(const std::string& document_text,
                     const std::vector<std::string>& query_terms) const;

private:
    size_t max_len_;

    static std::string to_lower(const std::string& s);
};

} // namespace search
