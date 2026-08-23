from collections import Counter
import re

class SpellingCorrector:
    """
    Damerau-Levenshtein / Peter Norvig style statistical spelling corrector.
    """
    def __init__(self):
        self.word_counts = Counter()
        self.total_words = 0

    def train(self, text: str):
        words = re.findall(r'[a-z]+', text.lower())
        self.word_counts.update(words)
        self.total_words = sum(self.word_counts.values())

    def p(self, word: str) -> float:
        if self.total_words == 0:
            return 0.0
        return self.word_counts[word] / self.total_words

    def correction(self, word: str) -> str:
        word = word.lower()
        candidates = (self._known([word]) or 
                      self._known(self._edits1(word)) or 
                      self._known(self._edits2(word)) or 
                      [word])
        return max(candidates, key=self.p)

    def _known(self, words):
        return set(w for w in words if w in self.word_counts)

    def _edits1(self, word: str):
        letters = 'abcdefghijklmnopqrstuvwxyz'
        splits = [(word[:i], word[i:]) for i in range(len(word) + 1)]
        deletes = [L + R[1:] for L, R in splits if R]
        transposes = [L + R[1] + R[0] + R[2:] for L, R in splits if len(R) > 1]
        replaces = [L + c + R[1:] for L, R in splits if R for c in letters]
        inserts = [L + c + R for L, R in splits for c in letters]
        return set(deletes + transposes + replaces + inserts)

    def _edits2(self, word: str):
        return set(e2 for e1 in self._edits1(word) for e2 in self._edits1(e1))
# Distance: keyboard layout typo weighting
