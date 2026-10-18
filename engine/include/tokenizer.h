#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <functional>

namespace search {

/**
 * Tokenizer — converts raw text into normalized tokens.
 * Applies: lowercasing, punctuation stripping, stopword removal, stemming.
 */
class Tokenizer {
public:
    struct Config {
        bool   lowercase        = true;
        bool   strip_punct      = true;
        bool   remove_stopwords = true;
        bool   apply_stemming   = true;
        size_t min_len          = 2;
        size_t max_len          = 64;
        Config() = default;
    };

    using Filter = std::function<bool(const std::string&)>;

    Tokenizer();
    explicit Tokenizer(const Config& cfg);

    /// Main entry point: text → token list
    std::vector<std::string> tokenize(const std::string& text) const;

    /// Stem a single word (Porter algorithm)
    std::string stem(const std::string& word) const;

    /// Check if word is a stopword
    bool is_stopword(const std::string& word) const;

    /// Add a custom filter (return false to drop token)
    void add_filter(Filter f);

    const Config& config() const { return cfg_; }

private:
    Config                        cfg_;
    std::unordered_set<std::string> stopwords_;
    std::vector<Filter>            filters_;

    void load_stopwords();
    std::string normalize(const std::string& tok) const;
    std::vector<std::string> split_whitespace(const std::string& text) const;

    static std::string to_lower(const std::string& s);
    static std::string strip_punctuation(const std::string& s);
};

} // namespace search
