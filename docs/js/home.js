(function (root) {
  "use strict";

  var message = "欢迎来到 Blacad 的笔记本";
  var typingTimer;

  function setActivePanel(name) {
    var cards = root.document.querySelectorAll("[data-home-card]");
    var buttons = root.document.querySelectorAll("[data-home-panel]");
    var target = root.document.querySelector('[data-home-card="' + name + '"]');
    var shouldOpen = Boolean(target && target.hidden);

    cards.forEach(function (card) {
      card.hidden = true;
    });
    buttons.forEach(function (button) {
      button.setAttribute("aria-expanded", "false");
    });

    if (!shouldOpen) return;
    target.hidden = false;
    var activeButton = root.document.querySelector('[data-home-panel="' + name + '"]');
    if (activeButton) activeButton.setAttribute("aria-expanded", "true");
  }

  root.toggle_about = function () { setActivePanel("about"); };
  root.toggle_statistics = function () { setActivePanel("statistics"); };
  root.toggle_recommend = function () { setActivePanel("recommend"); };

  function typeWelcome(target) {
    var characters = Array.from(message);
    var index = 0;
    root.clearTimeout(typingTimer);
    target.textContent = "";

    if (root.matchMedia("(prefers-reduced-motion: reduce)").matches) {
      target.textContent = message;
      return;
    }

    function typeNext() {
      target.textContent += characters[index];
      index += 1;
      if (index < characters.length) typingTimer = root.setTimeout(typeNext, 105);
    }

    typeNext();
  }

  function init() {
    var target = root.document.getElementById("home-typed");
    if (!target) return;

    typeWelcome(target);
    root.document.querySelectorAll("[data-home-panel]").forEach(function (button) {
      if (button.dataset.homeBound === "true") return;
      button.dataset.homeBound = "true";
      button.addEventListener("click", function () {
        var method = {
          about: root.toggle_about,
          statistics: root.toggle_statistics,
          recommend: root.toggle_recommend
        }[button.dataset.homePanel];
        if (method) method();
      });
    });
  }

  if (root.document$ && typeof root.document$.subscribe === "function") {
    root.document$.subscribe(init);
  } else if (root.document.readyState === "loading") {
    root.document.addEventListener("DOMContentLoaded", init);
  } else {
    init();
  }
})(window);
