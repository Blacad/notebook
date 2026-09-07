document.addEventListener("DOMContentLoaded", function () {
  function processLinks() {
    const links = document.querySelectorAll("a");
    links.forEach(function (link) {
      const href = link.getAttribute("href");
      if (!href || href.trim().length === 0) return;

      try {
        const linkUrl = new URL(href, window.location.href);
        const baseUrl = new URL(window.location.href);
        const cleanLinkUrl = new URL(
          linkUrl.origin + linkUrl.pathname + linkUrl.search,
          baseUrl.origin
        );

        if (cleanLinkUrl.origin === baseUrl.origin) {
          link.target = "_self";
          link.style.textDecoration = "none";
        } else {
          link.target = "_blank";
          link.classList.add("cross-origin");
        }
      } catch (error) {
        link.target = "_blank";
        link.classList.add("cross-origin");
      }
    });
  }

  processLinks();
});
