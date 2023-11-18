import { vitePreprocess } from "@sveltejs/vite-plugin-svelte";
import tailwind from "tailwindcss";
import autoprefixer from "autoprefixer";

export default {
  // Consult https://svelte.dev/docs#compile-time-svelte-preprocess
  // for more information about preprocessors
  preprocess: [
    vitePreprocess({
      postcss: {
        plugins: [tailwind, autoprefixer],
      },
    }),
  ],
};
