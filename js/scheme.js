

window.toggleScheme = () => {
  const body = document.querySelector("body")
  let scheme = body.getAttribute("data-md-color-scheme")

if (scheme === "slate") {
    scheme = "default"
  } else {
    scheme = "slate"
  }
  body.setAttribute("data-md-color-scheme", scheme)
  updateIconDisplay(scheme)
  var frame = document.querySelector(".giscus-frame")
  var theme = scheme === "slate" ? "transparent_dark" : "light"
  frame.contentWindow.postMessage(
    { giscus: { setConfig: { theme } } },
    "https://giscus.app"
  )
}
// 添加图标更新函数
function updateIconDisplay(scheme) {
  // 获取图标元素
  const lightIcon = document.querySelector('.icon-light');
  const darkIcon = document.querySelector('.icon-dark');
  
  // 根据主题显示对应图标
  if (scheme === "slate") {
    // 深色主题 - 显示太阳图标
    if (lightIcon) lightIcon.style.display = 'block';
    if (darkIcon) darkIcon.style.display = 'none';
  } else {
    // 浅色主题 - 显示月亮图标
    if (lightIcon) lightIcon.style.display = 'none';
    if (darkIcon) darkIcon.style.display = 'block';
  }
}
// 页面加载时初始化图标显示
document.addEventListener('DOMContentLoaded', function() {
  const body = document.querySelector("body");
  const scheme = body.getAttribute("data-md-color-scheme") || "slate";
  updateIconDisplay(scheme);
});