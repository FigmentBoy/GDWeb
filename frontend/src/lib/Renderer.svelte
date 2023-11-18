<script lang="ts">
    import { onMount } from 'svelte';

    let canv: HTMLCanvasElement;

    onMount(() => {
        (<any>window).Module = {
            ...(<any>window).Module,
            canvas: canv
        };
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