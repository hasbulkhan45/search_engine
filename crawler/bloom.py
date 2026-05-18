import math
import hashlib

class BloomFilter:
    """
    Space-efficient probabilistic data structure for URL duplicate filtering.
    """
    def __init__(self, expected_items: int = 100000, false_positive_rate: float = 0.01):
        self.expected_items = expected_items
        self.fp_rate = false_positive_rate
        self.size = self._optimal_size(expected_items, false_positive_rate)
        self.hash_count = self._optimal_hash_count(self.size, expected_items)
        self.bit_array = bytearray((self.size + 7) // 8)

    @staticmethod
    def _optimal_size(n: int, p: float) -> int:
        m = -(n * math.log(p)) / (math.log(2) ** 2)
        return int(m)

    @staticmethod
    def _optimal_hash_count(m: int, n: int) -> int:
        k = (m / n) * math.log(2)
        return max(1, int(k))

    def _hashes(self, item: str):
        h1 = int(hashlib.sha256(item.encode('utf-8')).hexdigest()[:16], 16)
        h2 = int(hashlib.md5(item.encode('utf-8')).hexdigest()[:16], 16)
        for i in range(self.hash_count):
            yield (h1 + i * h2) % self.size

    def add(self, item: str):
        for bit_index in self._hashes(item):
            byte_index = bit_index // 8
            bit_offset = bit_index % 8
            self.bit_array[byte_index] |= (1 << bit_offset)

    def __contains__(self, item: str) -> bool:
        for bit_index in self._hashes(item):
            byte_index = bit_index // 8
            bit_offset = bit_index % 8
            if not (self.bit_array[byte_index] & (1 << bit_offset)):
                return False
        return True
# Persistence: save/load bit array
