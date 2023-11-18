<script lang="ts">
    import Card from './components/Card.svelte';
    import { onMount } from 'svelte';

    let playing: boolean = false;
    let inLevel: boolean = false;

    let play = () => {
        (<any>window).Module._play();
    }

    let pause = () =>  {
        (<any>window).Module._pause();
    }    

    onMount(async () => {
        (<any>window).Module = {
            ...(<any>window).Module,
            enteredLevel: () => {
                inLevel = true;
            },
            exitedLevel: () => {
                inLevel = false;
            },
            startedPlaying: () => {
                playing = true;
            },
            stoppedPlaying: () => {
                playing = false;
            }
        };
    });
</script>

{#if inLevel}
    <Card on:click={playing ? pause : play} class="fixed top-4 right-4 cursor-pointer h-12 w-12 flex justify-center content-center">
        <i class="fas {playing ? 'fa-pause' : 'fa-play'}" />
    </Card>
{/if}