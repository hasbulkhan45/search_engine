#include "serializer.h"
#include "index.h"
#include <stdexcept>
#include <cstring>

namespace search {

void Serializer::write_u32(std::ofstream& f, uint32_t v) {
    f.write(reinterpret_cast<const char*>(&v), sizeof(v));
}
uint32_t Serializer::read_u32(std::ifstream& f) {
    uint32_t v = 0;
    f.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}
void Serializer::write_f64(std::ofstream& f, double v) {
    f.write(reinterpret_cast<const char*>(&v), sizeof(v));
}
double Serializer::read_f64(std::ifstream& f) {
    double v = 0;
    f.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}
void Serializer::write_str(std::ofstream& f, const std::string& s) {
    uint32_t len = static_cast<uint32_t>(s.size());
    write_u32(f, len);
    f.write(s.data(), len);
}
std::string Serializer::read_str(std::ifstream& f) {
    uint32_t len = read_u32(f);
    std::string s(len, '\0');
    f.read(s.data(), len);
    return s;
}

void Serializer::save(const Index& idx, const std::string& path) const {
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) throw std::runtime_error("Cannot write index: " + path);

    // Header
    write_u32(f, MAGIC);
    write_u32(f, VERSION);
    write_u32(f, static_cast<uint32_t>(idx.doc_count()));

    // Documents
    for (auto& [id, doc] : idx.documents()) {
        write_u32(f, doc.id);
        write_str(f, doc.url);
        write_str(f, doc.title);
        write_str(f, doc.body);
        write_str(f, doc.domain);
        write_f64(f, doc.pagerank);
        write_u32(f, static_cast<uint32_t>(doc.content_len));
    }
}

void Serializer::load(Index& idx, const std::string& path) const {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot read index: " + path);

    uint32_t magic = read_u32(f);
    if (magic != MAGIC) throw std::runtime_error("Invalid index file magic");
    uint32_t ver = read_u32(f);
    if (ver != VERSION) throw std::runtime_error("Unsupported index version");

    uint32_t doc_count = read_u32(f);
    for (uint32_t i = 0; i < doc_count; ++i) {
        Document doc;
        doc.id         = read_u32(f);
        doc.url        = read_str(f);
        doc.title      = read_str(f);
        doc.body       = read_str(f);
        doc.domain     = read_str(f);
        doc.pagerank   = read_f64(f);
        doc.content_len= read_u32(f);
        // Re-index tokens on load
        idx.add_document(doc, {});
    }
}

} // namespace search
