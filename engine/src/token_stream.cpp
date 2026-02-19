#include "token_stream.h"
#include <cctype>

namespace search {

TokenStream::TokenStream(std::string_view text) : text_(text), cursor_(0) {}

bool TokenStream::has_next() const {
    size_t i = cursor_;
    while (i < text_.size() && std::isspace(static_cast<unsigned char>(text_[i]))) {
        ++i;
    }
    return i < text_.size();
}

std::string_view TokenStream::next() {
    while (cursor_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[cursor_]))) {
        ++cursor_;
    }
    if (cursor_ >= text_.size()) {
        return {};
    }

    size_t start = cursor_;
    while (cursor_ < text_.size() && !std::isspace(static_cast<unsigned char>(text_[cursor_]))) {
        ++cursor_;
    }
    return text_.substr(start, cursor_ - start);
}

void TokenStream::reset() {
    cursor_ = 0;
}

} // namespace search
