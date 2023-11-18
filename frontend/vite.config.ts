import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'
import { imagetools } from '@zerodevx/svelte-img/vite'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    imagetools(),
    svelte()
  ],
})
