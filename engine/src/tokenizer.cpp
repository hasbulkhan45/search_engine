#include "tokenizer.h"
#include "stemmer.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace search {

Tokenizer::Tokenizer() : Tokenizer(Config{}) {}

Tokenizer::Tokenizer(const Config& cfg) : cfg_(cfg) {
    load_stopwords();
}

void Tokenizer::load_stopwords() {
    static const char* sw[] = {
        "a","an","the","and","or","but","in","on","at","to","for",
        "of","with","by","from","up","about","into","through","during",
        "is","was","are","were","be","been","being","have","has","had",
        "do","does","did","will","would","could","should","may","might",
        "shall","can","it","its","this","that","these","those",
        "i","we","you","he","she","they","me","him","her","us","them",
        "my","our","your","his","their","what","which","who","when",
        "where","why","how","not","no","nor","so","yet","very","too",
        nullptr
    };
    for (int i = 0; sw[i]; ++i) stopwords_.insert(sw[i]);
}

std::string Tokenizer::to_lower(const std::string& s) {
    std::string r(s.size(), '\0');
    std::transform(s.begin(), s.end(), r.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return r;
}

std::string Tokenizer::strip_punctuation(const std::string& s) {
    std::string r;
    r.reserve(s.size());
    for (unsigned char c : s) {
        if (std::isalnum(c)) r += static_cast<char>(c);
        else if (!r.empty() && r.back() != ' ') r += ' ';
    }
    while (!r.empty() && r.back() == ' ') r.pop_back();
    return r;
}

std::string Tokenizer::normalize(const std::string& tok) const {
    std::string t = tok;
    if (cfg_.lowercase)   t = to_lower(t);
    if (cfg_.strip_punct) t = strip_punctuation(t);
    return t;
}

std::vector<std::string> Tokenizer::split_whitespace(const std::string& text) const {
    std::vector<std::string> out;
    std::istringstream iss(text);
    std::string w;
    while (iss >> w) out.push_back(w);
    return out;
}

bool Tokenizer::is_stopword(const std::string& word) const {
    return stopwords_.count(word) > 0;
}

std::string Tokenizer::stem(const std::string& word) const {
    return porter_stem(word);
}

void Tokenizer::add_filter(Filter f) {
    filters_.push_back(std::move(f));
}

std::vector<std::string> Tokenizer::tokenize(const std::string& text) const {
    auto words = split_whitespace(text);
    std::vector<std::string> result;
    result.reserve(words.size());

    for (auto& w : words) {
        std::string t = normalize(w);
        if (t.empty() || t.size() < cfg_.min_len || t.size() > cfg_.max_len) continue;
        if (cfg_.remove_stopwords && is_stopword(t)) continue;

        bool ok = true;
        for (auto& f : filters_) if (!f(t)) { ok = false; break; }
        if (!ok) continue;

        if (cfg_.apply_stemming) t = stem(t);
        if (!t.empty()) result.push_back(std::move(t));
    }
    return result;
}

} // namespace search
