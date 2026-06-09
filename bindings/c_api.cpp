#include "c_api.h"
#include "index.h"
#include "tokenizer.h"

extern "C" {

CSearchEngineIndex search_engine_index_create() {
    return new search::Index();
}

void search_engine_index_free(CSearchEngineIndex idx) {
    delete static_cast<search::Index*>(idx);
}

size_t search_engine_index_doc_count(CSearchEngineIndex idx) {
    if (!idx) return 0;
    return static_cast<search::Index*>(idx)->doc_count();
}

void search_engine_index_add_doc(CSearchEngineIndex idx, uint32_t doc_id, const char* url, const char* title, const char* body) {
    if (!idx) return;
    auto* index = static_cast<search::Index*>(idx);
    search::Document doc(doc_id, url ? url : "", title ? title : "", body ? body : "");
    search::Tokenizer tok;
    index->add_document(doc, tok.tokenize(doc.full_text()));
}

}
