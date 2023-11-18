<script lang="ts">
    import { onMount } from 'svelte';

    let canv: HTMLCanvasElement;

    onMount(() => {
        let win = (<any>window);
        
        win.Module = {
            ...win.Module,
            canvas: canv
        };

        let interval = setInterval(() => {
            if (win.Module.calledRun) {
                clearInterval(interval);

                window.removeEventListener('keydown', win.GLFW.onKeydown, true)

                win.GLFW.onKeydown = (event: KeyboardEvent) => {
                    win.GLFW.onKeyChanged(event.keyCode, 1);
                }

                window.addEventListener('keydown', win.GLFW.onKeydown, true)
            }
        }, 100);
    });
</script>

<svelte:head>
  <script src="WebEditor.js" />
</svelte:head>

<canvas bind:this={canv} on:contextmenu|preventDefault class="-z-50"></canvas>

<style>
    canvas {
        width: 100%;
        height: 100%;
        position: absolute;
        top: 0;
        left: 0;
    }
</style>