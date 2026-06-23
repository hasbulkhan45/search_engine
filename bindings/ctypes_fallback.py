import ctypes
import os
import sys

class NativeCEngine:
    """
    ctypes fallback wrapper for systems without pybind11.
    """
    def __init__(self, lib_path: str = None):
        if lib_path is None:
            lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "build", "libsearch_engine.so"))
        try:
            self.lib = ctypes.CDLL(lib_path)
            self._setup_signatures()
            self._handle = self.lib.search_engine_index_create()
        except Exception:
            self.lib = None
            self._handle = None

    def _setup_signatures(self):
        self.lib.search_engine_index_create.restype = ctypes.c_void_p
        self.lib.search_engine_index_free.argtypes = [ctypes.c_void_p]
        self.lib.search_engine_index_doc_count.argtypes = [ctypes.c_void_p]
        self.lib.search_engine_index_doc_count.restype = ctypes.c_size_t

    def doc_count(self) -> int:
        if not self._handle:
            return 0
        return self.lib.search_engine_index_doc_count(self._handle)

    def __del__(self):
        if self.lib and self._handle:
            self.lib.search_engine_index_free(self._handle)
# Helper: paginate_results()
