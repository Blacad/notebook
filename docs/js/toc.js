/* The source code was written by @TonyCrane */
(function (window, document) {
  function register(toc) {
    const currentInView = new Set();
    const headingToMenu = new Map();
    const menus = Array.from(toc.querySelectorAll(".md-nav__list > li > a"));

    for (const menu of menus) {
      const href = menu.getAttribute("href");
      if (!href) continue;
      const heading = document.getElementById(href.trim().slice(1));
      if (heading) headingToMenu.set(heading, menu);
    }

    const headings = Array.from(headingToMenu.keys());
    const observer = new IntersectionObserver(function (entries) {
      for (const entry of entries) {
        if (entry.isIntersecting) currentInView.add(entry.target);
        else currentInView.delete(entry.target);
      }

      let heading;
      if (currentInView.size) {
        heading = Array.from(currentInView).sort(function (first, second) {
          return first.offsetTop - second.offsetTop;
        })[0];
      } else if (headings.length) {
        heading = headings
          .filter(function (item) { return item.offsetTop < window.scrollY; })
          .sort(function (first, second) { return second.offsetTop - first.offsetTop; })[0];
      }

      if (heading && headingToMenu.has(heading)) {
        menus.forEach(function (menu) { menu.classList.remove("is-active"); });
        const menu = headingToMenu.get(heading);
        menu.classList.add("is-active");

        let menuList = menu.parentElement.parentElement.parentElement;
        while (
          menuList.classList.contains("md-nav") &&
          menuList.parentElement.tagName.toLowerCase() === "li"
        ) {
          menuList.parentElement.children[0].classList.add("is-active");
          menuList = menuList.parentElement.parentElement.parentElement;
        }
      }
    }, { threshold: 0 });

    for (const heading of headings) {
      observer.observe(heading);
      const menu = headingToMenu.get(heading);
      menu.setAttribute("data-href", menu.getAttribute("href"));
      menu.setAttribute("href", "javascript:;");
      menu.addEventListener("click", function () {
        if (typeof heading.scrollIntoView === "function") {
          heading.scrollIntoView({ behavior: "smooth" });
        }
        const anchor = menu.getAttribute("data-href");
        if (history.pushState) history.pushState(null, null, anchor);
        else location.hash = anchor;
      });
      heading.style.scrollMargin = "4em";
    }
  }

  if (typeof window.IntersectionObserver === "undefined") return;
  document.querySelectorAll(".md-sidebar--secondary").forEach(register);
})(window, document);
