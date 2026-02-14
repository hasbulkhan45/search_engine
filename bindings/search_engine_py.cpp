#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "document.h"
#include "tokenizer.h"
#include "index.h"
#include "scorer.h"
#include "bm25.h"
#include "query_parser.h"
#include "snippet.h"

namespace py = pybind11;
using namespace search;

PYBIND11_MODULE(search_engine_native, m) {
    m.doc() = "Native C++ Core Search Engine Python Bindings";

    // Document struct
    py::class_<Document>(m, "Document")
        .def(py::init<>())
        .def(py::init<uint32_t, const std::string&, const std::string&, const std::string&>(),
             py::arg("id"), py::arg("url"), py::arg("title"), py::arg("body"))
        .def_readwrite("id", &Document::id)
        .def_readwrite("url", &Document::url)
        .def_readwrite("title", &Document::title)
        .def_readwrite("body", &Document::body)
        .def_readwrite("domain", &Document::domain)
        .def_readwrite("pagerank", &Document::pagerank)
        .def_readwrite("content_len", &Document::content_len)
        .def("full_text", &Document::full_text)
        .def("is_valid", &Document::is_valid)
        .def("set_meta", &Document::set_meta)
        .def("get_meta", &Document::get_meta, py::arg("key"), py::arg("default") = "");

    // Tokenizer
    py::class_<Tokenizer>(m, "Tokenizer")
        .def(py::init<>())
        .def("tokenize", &Tokenizer::tokenize, py::arg("text"))
        .def("stem", &Tokenizer::stem, py::arg("word"))
        .def("is_stopword", &Tokenizer::is_stopword, py::arg("word"));

    // Posting
    py::class_<Posting>(m, "Posting")
        .def_readonly("doc_id", &Posting::doc_id)
        .def_readonly("freq", &Posting::freq)
        .def_readonly("positions", &Posting::positions);

    // SearchResult
    py::class_<SearchResult>(m, "SearchResult")
        .def_readonly("doc_id", &SearchResult::doc_id)
        .def_readonly("score", &SearchResult::score);

    // Index
    py::class_<Index>(m, "Index")
        .def(py::init<>())
        .def("add_document", &Index::add_document, py::arg("doc"), py::arg("tokens"))
        .def("remove_document", &Index::remove_document, py::arg("doc_id"))
        .def("clear", &Index::clear)
        .def("doc_count", &Index::doc_count)
        .def("vocab_size", &Index::vocab_size)
        .def("avg_doc_len", &Index::avg_doc_len)
        .def("get_doc", &Index::get_doc, py::return_value_policy::reference)
        .def("intersect", &Index::intersect, py::arg("terms"))
        .def("unite", &Index::unite, py::arg("terms"))
        .def("phrase_search", &Index::phrase_search, py::arg("terms"));

    // TFIDFScorer
    py::class_<TFIDFScorer>(m, "TFIDFScorer")
        .def(py::init<const Index&>(), py::arg("index"))
        .def("score", &TFIDFScorer::score, py::arg("terms"))
        .def("idf", &TFIDFScorer::idf, py::arg("term"));

    // BM25Scorer
    py::class_<BM25Scorer>(m, "BM25Scorer")
        .def(py::init<const Index&>(), py::arg("index"))
        .def("score", &BM25Scorer::score, py::arg("terms"))
        .def("idf", &BM25Scorer::idf, py::arg("term"));

    // QueryParser
    py::class_<ParsedQuery>(m, "ParsedQuery")
        .def_readonly("terms", &ParsedQuery::terms)
        .def_readonly("phrase", &ParsedQuery::phrase)
        .def_readonly("is_phrase", &ParsedQuery::is_phrase);

    py::class_<QueryParser>(m, "QueryParser")
        .def(py::init<bool>(), py::arg("default_and") = true)
        .def("parse", &QueryParser::parse, py::arg("query_str"));

    // Snippet
    py::class_<Snippet>(m, "Snippet")
        .def_readonly("text", &Snippet::text)
        .def_readonly("has_match", &Snippet::has_match);

    py::class_<SnippetGenerator>(m, "SnippetGenerator")
        .def(py::init<size_t>(), py::arg("max_snippet_length") = 160)
        .def("generate", &SnippetGenerator::generate, py::arg("document_text"), py::arg("query_terms"));
}
