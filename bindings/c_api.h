#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* CSearchEngineIndex;

CSearchEngineIndex search_engine_index_create();
void search_engine_index_free(CSearchEngineIndex idx);
size_t search_engine_index_doc_count(CSearchEngineIndex idx);
void search_engine_index_add_doc(CSearchEngineIndex idx, uint32_t doc_id, const char* url, const char* title, const char* body);

#ifdef __cplusplus
}
#endif
