#pragma once
#include "index.h"
#include "scorer.h"
#include <string>
#include <vector>
#include <variant>

namespace search {

/// Parsed query node types
struct TermNode  { std::string term; };
struct AndNode   { std::vector<struct QueryNode*> children; };
struct OrNode    { std::vector<struct QueryNode*> children; };
struct NotNode   { struct QueryNode* child; };
struct PhraseNode{ std::vector<std::string> terms; };

using QueryVariant = std::variant<TermNode, PhraseNode>;

struct ParsedQuery {
    enum class Type { AND, OR, PHRASE, TERM } type = Type::AND;
    std::vector<std::string> terms;
    std::string phrase;
    bool is_phrase = false;
};

/**
 * Simple recursive descent query parser.
 * Supports: term, "phrase", AND, OR, -NOT
 *
 * Grammar:
 *   query  := expr (WS expr)*
 *   expr   := '-'? ('"' phrase '"' | term)
 */
class QueryParser {
public:
    explicit QueryParser(bool default_and = true)
        : default_and_(default_and) {}

    ParsedQuery parse(const std::string& query_str) const;

private:
    bool default_and_;

    std::vector<std::string> tokenize_query(const std::string& q) const;
    std::string extract_phrase(const std::string& q,
                               size_t start, size_t& end) const;
};

} // namespace search
