#pragma once
#include <string_view>
#include <vector>

namespace search {

/**
 * Zero-copy token stream for high-performance string scanning.
 */
class TokenStream {
public:
    explicit TokenStream(std::string_view text);

    bool has_next() const;
    std::string_view next();
    void reset();

private:
    std::string_view text_;
    size_t cursor_ = 0;
};

} // namespace search
