<script lang="ts">
    import Card from './components/Card.svelte';
    import { onMount } from 'svelte';

    import { type Song, NGSong, NONGSong, type RAWNONGSong } from './types/Song';

    import { LevelObject } from './types/LevelObject';
    import Modal from './components/Modal.svelte';
    import Divider from './components/Divider.svelte';
    import SongContainer from './components/Song.svelte';

    import { currLevel } from './CurrentLevel';


    let songs: Song[] = [];


    currLevel.subscribe((newLevel: LevelObject | null) => {
        if (!newLevel) return;

        newLevel.song?.setAsCurrentSong();
        fetch(`https://api.songfilehub.com/songs?songID=${newLevel.songID}`)
            .then(async response => await response.json() as Promise<RAWNONGSong[]>)
            .then(response => response.map(song => new NONGSong(song)))
            .then(response => {
                songs = [newLevel!.song!, ...response]
                console.log(songs)
                document.getElementById("songlist")?.scrollTo(0, 0);
            })
    });

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

    let nongMenuOpen: boolean = false;

    let openMenu = () => {
        nongMenuOpen = true;
    }

    let closeMenu = () => {
        nongMenuOpen = false;
    }
</script>

{#if inLevel}
    <div class = "fixed top-4 right-4 flex gap-4">
        <Card on:click={openMenu} class="cursor-pointer h-12 w-12 flex justify-center items-center">
            <i class="fas fa-music fa-lg" />
        </Card>
        <Card on:click={playing ? pause : play} class="cursor-pointer h-12 w-12 flex justify-center items-center">
            <i class="fas {playing ? 'fa-pause' : 'fa-play'} fa-lg" />
        </Card>
    </div>
{/if}

{#if nongMenuOpen}
    <Modal class="{songs.length == 0 ? '' : 'pb-0'}">
        <div class="flex justify-between items-center w-full">
            <div>
                <h1 class="text-2xl font-bold">NONG Settings{#if songs.length == 0}<span class="pl-2"><i class="fas fa-circle-notch fa-spin text-gray-400" /></span>{/if}</h1>
                <h2 class="text-lg font-semibold text-gray-500">Powered by <a href="https://songfilehub.com/" target="_blank" class="text-indigo-600">Song File Hub</a></h2>
            </div>
            <button class="w-8 h-8" on:click={closeMenu}>
                <i class="fas fa-times translate-x-1/2" />
            </button>
        </div>
        
        {#if songs.length != 0}
            <Divider class="mb-0" />
            <div class="songlist overflow-y-auto overflow-x-visible flex flex-col gap-2 p-4" id="songlist">
                {#each songs as song}
                    <SongContainer {song} />
                {/each}
            </div>
        {/if}
        
    </Modal>
{/if}

<style>
    .songlist {
        width: calc(100% + 2rem);
        margin-left: -1rem;
        margin-right: -1rem;
    }

    .songlist::-webkit-scrollbar {
        width: 0;
    }
</style>