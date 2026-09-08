---
counter: false
comments: false
home: true
hide:
  - navigation
  - toc
  - footer
title: "Blacad 的笔记本"
---

<div class="simple-home">
  <h1 class="simple-home__welcome" aria-label="欢迎来到 Blacad 的笔记本">
    <span id="home-typed" aria-hidden="true"></span>
  </h1>

  <div class="simple-home__actions" aria-label="首页信息">
    <button type="button" data-home-panel="about" aria-controls="home-about" aria-expanded="false">关于站点</button>
    <button type="button" data-home-panel="statistics" aria-controls="home-statistics" aria-expanded="false">站点统计</button>
    <button type="button" data-home-panel="recommend" aria-controls="home-recommend" aria-expanded="false">推荐阅读</button>
    <button type="button" onclick="window.location.href='board.md'">留言板</button><button onclick="window.location.href='/page2'">
  </div>

  <div class="simple-home__panels" aria-live="polite">
    <section class="simple-home__card" id="home-about" data-home-card="about" hidden>
      <h2>关于站点</h2>
      <p>这里是 Blacad 的个人笔记本，用来整理感兴趣的计算机科学课程、学习记录</p>
    </section>

    <section class="simple-home__card" id="home-statistics" data-home-card="statistics" hidden>
      <h2>站点统计</h2>
      <ul>
        <li>页面总数：<strong>{{pages}}</strong></li>
        <li>总字数：<strong>{{words}}</strong></li>
        <li>代码行数：<strong>{{codes}}</strong></li>
      </ul>
    </section>

    <section class="simple-home__card" id="home-recommend" data-home-card="recommend" hidden>
      <h2>推荐阅读</h2>
      <ul class="simple-home__links">
        <li><a href="CS/CS61A/说明.md">CS61A：程序构造</a></li>
        <li><a href="CS/CS70/说明.md">CS70：离散数学与概率论</a></li>
        <li><a href="CS/CS188/说明.md">CS188：人工智能</a></li>
        <li><a href="CS/宏毅22/说明.md">李宏毅机器学习 2022</a></li>
      </ul>
    </section>
  </div>
</div>
