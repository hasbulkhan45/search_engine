#include "stemmer.h"
#include <cstring>

namespace search {

bool Stemmer::ends_with(const std::string& s, const std::string& suf) {
    return s.size() >= suf.size() &&
           s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}

bool Stemmer::is_consonant(const std::string& s, size_t i) {
    char c = s[i];
    switch (c) {
        case 'a': case 'e': case 'i': case 'o': case 'u': return false;
        case 'y': return (i == 0) || !is_consonant(s, i - 1);
        default:  return true;
    }
}

int Stemmer::measure(const std::string& s) {
    int n = 0;
    size_t i = 0, len = s.size();
    // Skip leading consonants
    while (i < len && is_consonant(s, i)) ++i;
    while (true) {
        // Skip vowels
        while (i < len && !is_consonant(s, i)) ++i;
        if (i >= len) break;
        // Skip consonants
        while (i < len && is_consonant(s, i)) ++i;
        ++n;
    }
    return n;
}

bool Stemmer::has_vowel(const std::string& s) {
    for (size_t i = 0; i < s.size(); ++i)
        if (!is_consonant(s, i)) return true;
    return false;
}

bool Stemmer::double_cons(const std::string& s) {
    size_t n = s.size();
    return n >= 2 && is_consonant(s, n-1) && s[n-1] == s[n-2];
}

bool Stemmer::cvc(const std::string& s) {
    size_t n = s.size();
    if (n < 3) return false;
    char last = s[n-1];
    return is_consonant(s, n-1) &&
           !is_consonant(s, n-2) &&
           is_consonant(s, n-3) &&
           last != 'w' && last != 'x' && last != 'y';
}

std::string Stemmer::step1a(const std::string& s) {
    if (ends_with(s,"sses")) return s.substr(0, s.size()-2);
    if (ends_with(s,"ies"))  return s.substr(0, s.size()-2);
    if (ends_with(s,"ss"))   return s;
    if (ends_with(s,"s"))    return s.substr(0, s.size()-1);
    return s;
}

std::string Stemmer::step1b(const std::string& s) {
    if (ends_with(s,"eed")) {
        auto base = s.substr(0, s.size()-3);
        return measure(base) > 0 ? base + "ee" : s;
    }
    auto try_suffix = [&](const std::string& suf) -> std::string {
        if (!ends_with(s, suf)) return "";
        auto base = s.substr(0, s.size() - suf.size());
        if (!has_vowel(base)) return "";
        if (ends_with(base,"at")||ends_with(base,"bl")||ends_with(base,"iz"))
            return base + "e";
        if (double_cons(base) && base.back()!='l' && base.back()!='s' && base.back()!='z')
            return base.substr(0, base.size()-1);
        if (measure(base)==1 && cvc(base)) return base + "e";
        return base;
    };
    auto r = try_suffix("ed");
    if (!r.empty()) return r;
    r = try_suffix("ing");
    if (!r.empty()) return r;
    return s;
}

std::string Stemmer::step1c(const std::string& s) {
    if (ends_with(s,"y") && has_vowel(s.substr(0, s.size()-1)))
        return s.substr(0, s.size()-1) + "i";
    return s;
}

std::string Stemmer::step2(const std::string& s) {
    struct R { const char* from; const char* to; };
    static const R rules[] = {
        {"ational","ate"},{"tional","tion"},{"enci","ence"},{"anci","ance"},
        {"izer","ize"},{"bli","ble"},{"alli","al"},{"entli","ent"},
        {"eli","e"},{"ousli","ous"},{"ization","ize"},{"ation","ate"},
        {"ator","ate"},{"alism","al"},{"iveness","ive"},{"fulness","ful"},
        {"ousness","ous"},{"aliti","al"},{"iviti","ive"},{"biliti","ble"},
        {nullptr, nullptr}
    };
    for (auto* r = rules; r->from; ++r) {
        if (ends_with(s, r->from)) {
            auto base = s.substr(0, s.size() - std::strlen(r->from));
            if (measure(base) > 0) return base + r->to;
        }
    }
    return s;
}

std::string Stemmer::step3(const std::string& s) {
    struct R { const char* from; const char* to; };
    static const R rules[] = {
        {"icate","ic"},{"ative",""},{"alize","al"},
        {"iciti","ic"},{"ical","ic"},{"ful",""},{"ness",""},
        {nullptr,nullptr}
    };
    for (auto* r = rules; r->from; ++r) {
        if (ends_with(s, r->from)) {
            auto base = s.substr(0, s.size() - std::strlen(r->from));
            if (measure(base) > 0) return base + r->to;
        }
    }
    return s;
}

std::string Stemmer::step4(const std::string& s) {
    static const char* suffixes[] = {
        "al","ance","ence","er","ic","able","ible","ant","ement",
        "ment","ent","ou","ism","ate","iti","ous","ive","ize",nullptr
    };
    for (auto** sf = suffixes; *sf; ++sf) {
        if (ends_with(s, *sf)) {
            auto base = s.substr(0, s.size() - std::strlen(*sf));
            if (measure(base) > 1) return base;
        }
    }
    // Special: "ion" after s/t
    if (ends_with(s,"ion")) {
        auto base = s.substr(0, s.size()-3);
        if (measure(base)>1 && !base.empty() && (base.back()=='s'||base.back()=='t'))
            return base;
    }
    return s;
}

std::string Stemmer::step5(const std::string& s) {
    if (ends_with(s,"e")) {
        auto base = s.substr(0, s.size()-1);
        if (measure(base)>1 || (measure(base)==1 && !cvc(base)))
            return base;
    }
    if (ends_with(s,"ll") && measure(s.substr(0,s.size()-1))>1)
        return s.substr(0, s.size()-1);
    return s;
}

std::string Stemmer::stem(const std::string& word) {
    if (word.size() <= 2) return word;
    std::string s = word;
    s = step1a(s);
    s = step1b(s);
    s = step1c(s);
    s = step2(s);
    s = step3(s);
    s = step4(s);
    s = step5(s);
    return s;
}

std::string porter_stem(const std::string& word) {
    return Stemmer::stem(word);
}

} // namespace search
