#include "query_parser.h"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace search {

std::string QueryParser::extract_phrase(const std::string& q,
                                        size_t start, size_t& end) const {
    size_t closing = q.find('"', start + 1);
    if (closing == std::string::npos) {
        end = q.size();
        return q.substr(start + 1);
    }
    end = closing + 1;
    return q.substr(start + 1, closing - start - 1);
}

std::vector<std::string> QueryParser::tokenize_query(const std::string& q) const {
    std::vector<std::string> tokens;
    std::string current;
    for (char c : q) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

ParsedQuery QueryParser::parse(const std::string& query_str) const {
    ParsedQuery pq;
    size_t i = 0;
    size_t n = query_str.size();

    // Check for quoted phrase query first
    size_t first_quote = query_str.find('"');
    if (first_quote != std::string::npos) {
        size_t end = 0;
        std::string phrase = extract_phrase(query_str, first_quote, end);
        pq.is_phrase = true;
        pq.phrase = phrase;
        pq.type = ParsedQuery::Type::PHRASE;
        pq.terms = tokenize_query(phrase);
        return pq;
    }

    // Standard keyword tokenization
    auto raw_tokens = tokenize_query(query_str);
    bool has_or = false;
    for (const auto& token : raw_tokens) {
        if (token == "or") {
            has_or = true;
            continue;
        }
        if (token == "and") {
            continue;
        }
        if (!token.empty() && token[0] == '-') {
            // Negative term filter
            continue;
        }
        pq.terms.push_back(token);
    }

    if (has_or) {
        pq.type = ParsedQuery::Type::OR;
    } else {
        pq.type = default_and_ ? ParsedQuery::Type::AND : ParsedQuery::Type::OR;
    }

    return pq;
}

} // namespace search
// Syntax: warn on unmatched quotes
