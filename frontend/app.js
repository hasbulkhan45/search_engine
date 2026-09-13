document.addEventListener("DOMContentLoaded", () => {
    const searchForm = document.getElementById("searchForm");
    const searchInput = document.getElementById("searchInput");
    const clearBtn = document.getElementById("clearBtn");
    const heroSection = document.getElementById("heroSection");
    const resultsSection = document.getElementById("resultsSection");
    const resultsList = document.getElementById("resultsList");
    const resultsStats = document.getElementById("resultsStats");
    const rankerSelect = document.getElementById("rankerSelect");
    const autocompleteDropdown = document.getElementById("autocompleteDropdown");
    const pagination = document.getElementById("pagination");
    const prevPageBtn = document.getElementById("prevPageBtn");
    const nextPageBtn = document.getElementById("nextPageBtn");
    const pageIndicator = document.getElementById("pageIndicator");
    const themeToggle = document.getElementById("themeToggle");

    let currentQuery = "";
    let currentPage = 0;
    const pageSize = 10;
    let totalHits = 0;
    let debounceTimer = null;

    // Theme toggle
    themeToggle.addEventListener("click", () => {
        document.body.classList.toggle("light-theme");
        document.body.classList.toggle("dark-theme");
        themeToggle.textContent = document.body.classList.contains("light-theme") ? "☀️" : "🌙";
    });

    // Input handlers
    searchInput.addEventListener("input", (e) => {
        const val = e.target.value.trim();
        clearBtn.classList.toggle("hidden", val.length === 0);

        clearTimeout(debounceTimer);
        if (val.length >= 2) {
            debounceTimer = setTimeout(() => fetchSuggestions(val), 200);
        } else {
            autocompleteDropdown.classList.add("hidden");
        }
    });

    clearBtn.addEventListener("click", () => {
        searchInput.value = "";
        clearBtn.classList.add("hidden");
        autocompleteDropdown.classList.add("hidden");
        searchInput.focus();
    });

    // Tag buttons
    document.querySelectorAll(".tag-btn").forEach(btn => {
        btn.addEventListener("click", () => {
            const q = btn.dataset.query;
            searchInput.value = q;
            clearBtn.classList.remove("hidden");
            executeSearch(q, 0);
        });
    });

    // Form submit
    searchForm.addEventListener("submit", (e) => {
        e.preventDefault();
        const q = searchInput.value.trim();
        if (q) {
            autocompleteDropdown.classList.add("hidden");
            executeSearch(q, 0);
        }
    });

    rankerSelect.addEventListener("change", () => {
        if (currentQuery) {
            executeSearch(currentQuery, currentPage);
        }
    });

    // Pagination
    prevPageBtn.addEventListener("click", () => {
        if (currentPage > 0) executeSearch(currentQuery, currentPage - 1);
    });

    nextPageBtn.addEventListener("click", () => {
        if ((currentPage + 1) * pageSize < totalHits) executeSearch(currentQuery, currentPage + 1);
    });

    async function fetchSuggestions(prefix) {
        try {
            const resp = await fetch(`/api/v1/search?q=${encodeURIComponent(prefix)}&limit=5`);
            if (!resp.ok) return;
            const data = await resp.json();
            if (data.hits && data.hits.length > 0) {
                autocompleteDropdown.innerHTML = data.hits.map(h => `
                    <div class="autocomplete-item" data-query="${h.title}">
                        <span>🔍</span>
                        <span>${h.title}</span>
                    </div>
                `).join("");
                autocompleteDropdown.classList.remove("hidden");

                document.querySelectorAll(".autocomplete-item").forEach(item => {
                    item.addEventListener("click", () => {
                        searchInput.value = item.dataset.query;
                        autocompleteDropdown.classList.add("hidden");
                        executeSearch(item.dataset.query, 0);
                    });
                });
            } else {
                autocompleteDropdown.classList.add("hidden");
            }
        } catch (err) {
            autocompleteDropdown.classList.add("hidden");
        }
    }

    async function executeSearch(query, page) {
        currentQuery = query;
        currentPage = page;
        const ranker = rankerSelect.value;
        const offset = page * pageSize;

        try {
            resultsStats.textContent = "Searching...";
            const resp = await fetch(`/api/v1/search?q=${encodeURIComponent(query)}&limit=${pageSize}&offset=${offset}&ranker=${ranker}`);
            if (!resp.ok) {
                resultsStats.textContent = "Error executing search.";
                return;
            }

            const data = await resp.json();
            totalHits = data.total_hits;

            resultsStats.textContent = `About ${totalHits} results (${data.took_ms} ms)`;
            renderHits(data.hits);

            resultsSection.classList.remove("hidden");
            pagination.classList.toggle("hidden", totalHits <= pageSize);

            prevPageBtn.disabled = currentPage === 0;
            nextPageBtn.disabled = (currentPage + 1) * pageSize >= totalHits;
            pageIndicator.textContent = `Page ${currentPage + 1}`;
        } catch (err) {
            resultsStats.textContent = "Network error connecting to search API.";
        }
    }

    function renderHits(hits) {
        if (!hits || hits.length === 0) {
            resultsList.innerHTML = `<div class="result-card"><p>No documents matched your query.</p></div>`;
            return;
        }

        resultsList.innerHTML = hits.map(h => `
            <article class="result-card">
                <div class="result-domain">${h.domain || "web"}</div>
                <h2 class="result-title"><a href="${h.url}" target="_blank" rel="noopener">${h.title}</a></h2>
                <p class="result-snippet">${h.snippet}</p>
                <div class="result-footer">
                    <span>Relevance Score: ${h.score}</span>
                    <a href="${h.url}" target="_blank" rel="noopener">Visit page &rarr;</a>
                </div>
            </article>
        `).join("");
    }
});
