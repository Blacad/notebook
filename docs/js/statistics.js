(function (root, factory) {
  const api = factory(root);

  if (typeof module === "object" && module.exports) module.exports = api;

  if (root && root.document) {
    const start = function () { return api.init(root.document); };
    if (root.document$ && typeof root.document$.subscribe === "function") {
      root.document$.subscribe(start);
    } else if (root.document.readyState === "loading") {
      root.document.addEventListener("DOMContentLoaded", start);
    } else {
      start();
    }
  }
})(typeof window !== "undefined" ? window : undefined, function (root) {
  const defaults = {
    pageStatistics: true,
    pageReadTime: true,
    pageImages: true,
    wordsPerMinute: 300,
    codeLinesPerMinute: 80,
    cacheKey: "zensical-statistics-v1"
  };

  function countWords(text) {
    const chinese = text.match(/[\u4e00-\u9fa5]/g) || [];
    const english = text.match(/[A-Za-z0-9]+/g) || [];
    return chinese.length + english.length;
  }

  function countCodeLines(code) {
    const normalized = String(code || "").replace(/\r\n?/g, "\n").replace(/\n$/, "");
    return normalized ? normalized.split("\n").length : 0;
  }

  function textWithoutCode(element) {
    if (!element || !element.childNodes) {
      return element && element.textContent ? element.textContent : "";
    }

    const skipTags = new Set(["CODE", "PRE", "SCRIPT", "STYLE", "TEMPLATE"]);
    let text = "";

    function walk(node) {
      if (node.nodeType === 3) {
        text += " " + node.nodeValue;
        return;
      }
      if (node.nodeType !== 1) return;
      if (skipTags.has(node.tagName) || node.classList.contains("statistics")) return;
      for (const child of node.childNodes) walk(child);
    }

    walk(element);
    return text;
  }

  function getArticleStats(article, options) {
    const config = Object.assign({}, defaults, options || {});
    const codeLines = Array.from(article.querySelectorAll("pre code"))
      .reduce(function (total, code) {
        return total + countCodeLines(code.textContent);
      }, 0);
    const words = countWords(textWithoutCode(article));
    const images = article.querySelectorAll("img").length;
    const readTime = Math.round(
      words / config.wordsPerMinute + codeLines / config.codeLinesPerMinute
    );
    return { words: words, codeLines: codeLines, images: images, readTime: readTime };
  }

  function getStatisticsContent(article) {
    return article.querySelector(".statistics-content") || article;
  }

  function replacePlaceholders(text, stats) {
    return String(text)
      .replace(/\{\{\s*pages\s*\}\}/gi, String(stats.pages))
      .replace(/\{\{\s*words\s*\}\}/gi, String(stats.words))
      .replace(/\{\{\s*codes\s*\}\}/gi, String(stats.codes))
      .replace(/\{\{\s*images\s*\}\}/gi, String(stats.images));
  }

  function normalizePageUrl(url) {
    return new URL(url, "https://example.invalid/").pathname;
  }

  function replaceArticlePlaceholders(article, stats) {
    if (!article || !root || !root.document) return;
    const walker = root.document.createTreeWalker(article, NodeFilter.SHOW_TEXT);
    const nodes = [];
    while (walker.nextNode()) {
      if (/\{\{\s*(pages|words|codes|images)\s*\}\}/i.test(walker.currentNode.nodeValue)) {
        nodes.push(walker.currentNode);
      }
    }
    for (const node of nodes) node.nodeValue = replacePlaceholders(node.nodeValue, stats);
  }

  function isCounterEnabled(article) {
    const marker = article && article.querySelector(".statistics-config[data-counter]");
    return Boolean(marker && marker.dataset.counter === "true");
  }

  function fillStatistic(container, name, value) {
    const target = container.querySelector('[data-stat="' + name + '"]');
    if (target) target.textContent = value;
  }

  function toggleSection(container, name, enabled) {
    const section = container.querySelector('[data-stat-section="' + name + '"]');
    if (section) section.hidden = !enabled;
  }

  function renderPageStatistics(article, stats, options) {
    const config = Object.assign({}, defaults, options || {});
    const existing = article.querySelector(".statistics");
    if (!config.pageStatistics || !isCounterEnabled(article)) {
      if (existing) existing.remove();
      return;
    }

    const content = getStatisticsContent(article);
    const heading = content.querySelector("h1");
    if (!heading) return;
    const template = article.querySelector(".statistics-template");
    const container = existing || (
      template
        ? template.content.firstElementChild.cloneNode(true)
        : root.document.createElement("p")
    );
    container.className = "statistics";
    fillStatistic(container, "words", stats.words);
    fillStatistic(container, "codeLines", stats.codeLines);
    fillStatistic(container, "readTime", stats.readTime);
    toggleSection(container, "readTime", config.pageReadTime);
    if (!existing) heading.insertAdjacentElement("afterend", container);
  }

  function pageUrlsFromSearchIndex(searchIndex) {
    const urls = new Set();
    for (const item of searchIndex.items || []) {
      if (item.location) urls.add(item.location.split("#")[0]);
    }
    return Array.from(urls);
  }

  async function fetchPageStats(url, options) {
    const response = await fetch(url, { credentials: "same-origin" });
    if (!response.ok) throw new Error("Unable to fetch " + url);
    const html = await response.text();
    const document = new DOMParser().parseFromString(html, "text/html");
    const article = document.querySelector("article.md-content__inner");
    if (!article) return { words: 0, codeLines: 0, images: 0, readTime: 0 };
    return getArticleStats(getStatisticsContent(article), options);
  }

  function getSessionItem(key) {
    try {
      return root.sessionStorage && root.sessionStorage.getItem(key);
    } catch (error) {
      return null;
    }
  }

  function setSessionItem(key, value) {
    try {
      if (root.sessionStorage) root.sessionStorage.setItem(key, value);
    } catch (error) {}
  }

  async function getGlobalStats(options) {
    const config = Object.assign({}, defaults, options || {});
    const base = root.__md_scope || new URL(".", root.location.href);
    const storageKey = config.cacheKey + ":" + normalizePageUrl(base.href);
    const cached = getSessionItem(storageKey);
    if (cached) return JSON.parse(cached);

    const searchUrl = new URL("search.json", base);
    const response = await fetch(searchUrl, { credentials: "same-origin" });
    if (!response.ok) throw new Error("Unable to fetch search index");
    const searchIndex = await response.json();
    const pageUrls = pageUrlsFromSearchIndex(searchIndex).map(function (location) {
      return new URL(location, base).href;
    });
    const results = await Promise.allSettled(
      pageUrls.map(function (url) { return fetchPageStats(url, config); })
    );
    const stats = { pages: 0, words: 0, codes: 0, images: 0 };
    for (const result of results) {
      if (result.status !== "fulfilled") continue;
      stats.pages += 1;
      stats.words += result.value.words;
      stats.codes += result.value.codeLines;
      stats.images += result.value.images;
    }
    setSessionItem(storageKey, JSON.stringify(stats));
    return stats;
  }

  function init(document) {
    const article = document.querySelector("article.md-content__inner");
    if (!article || article.dataset.statistics === "false") return;
    const config = Object.assign({}, defaults, root.zensicalStatistics || {});
    const pageStats = getArticleStats(getStatisticsContent(article), config);
    renderPageStatistics(article, pageStats, config);
    getGlobalStats(config)
      .then(function (stats) { return replaceArticlePlaceholders(article, stats); })
      .catch(function () {});
  }

  return {
    countCodeLines: countCodeLines,
    countWords: countWords,
    fillStatistic: fillStatistic,
    getArticleStats: getArticleStats,
    getStatisticsContent: getStatisticsContent,
    isCounterEnabled: isCounterEnabled,
    normalizePageUrl: normalizePageUrl,
    pageUrlsFromSearchIndex: pageUrlsFromSearchIndex,
    replacePlaceholders: replacePlaceholders,
    init: init
  };
});
