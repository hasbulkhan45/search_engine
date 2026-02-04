#pragma once
#include <string>

namespace search {

/**
 * Porter Stemming Algorithm (1980)
 * Reduces words to their morphological root.
 * e.g. "running" -> "run", "happiness" -> "happi"
 */
std::string porter_stem(const std::string& word);

class Stemmer {
public:
    static std::string stem(const std::string& word);

private:
    static bool is_consonant(const std::string& s, size_t i);
    static int  measure(const std::string& s);
    static bool has_vowel(const std::string& s);
    static bool double_cons(const std::string& s);
    static bool cvc(const std::string& s);
    static bool ends_with(const std::string& s, const std::string& suf);

    static std::string step1a(const std::string& s);
    static std::string step1b(const std::string& s);
    static std::string step1c(const std::string& s);
    static std::string step2(const std::string& s);
    static std::string step3(const std::string& s);
    static std::string step4(const std::string& s);
    static std::string step5(const std::string& s);
};

} // namespace search
