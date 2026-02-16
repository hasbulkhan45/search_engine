#pragma once
#include <string>
#include <unordered_set>

namespace search {

class StopWords {
public:
    static bool is_stop_word(const std::string& term);
    static const std::unordered_set<std::string>& get_default_set();
};

} // namespace search
