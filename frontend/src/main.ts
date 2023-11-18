import '@fortawesome/fontawesome-free/css/all.min.css'
import "./app.postcss";

import App from "./App.svelte";

const app = new App({
  target: <Element>document.getElementById("app"),
});

export default app;
