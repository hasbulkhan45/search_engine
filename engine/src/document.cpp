#include "document.h"
#include <sstream>
#include <algorithm>

namespace search {

Document::Document(uint32_t id,
                   const std::string& url,
                   const std::string& title,
                   const std::string& body)
    : id(id), url(url), title(title), body(body),
      crawled_at(std::time(nullptr)), content_len(body.size())
{
    // Extract domain from URL
    auto start = url.find("://");
    if (start != std::string::npos) {
        start += 3;
        auto end = url.find('/', start);
        domain = url.substr(start, end - start);
    }
}

std::string Document::full_text() const {
    // Title weighted x2 for scoring purposes
    std::ostringstream oss;
    oss << title << ' ' << title << ' ' << body;
    return oss.str();
}

bool Document::is_valid() const {
    return id > 0 && !url.empty() && !body.empty();
}

void Document::set_meta(const std::string& key, const std::string& val) {
    meta[key] = val;
}

std::string Document::get_meta(const std::string& key,
                                const std::string& def) const {
    auto it = meta.find(key);
    return it != meta.end() ? it->second : def;
}

} // namespace search
// Helper: parse href anchors
// Validation: require non-empty text body
