<script lang="ts">
	import Card from './Card.svelte';
    import { currSong, type Song } from '../types/Song';

    export let song: Song;
    
    let used: boolean = false;

    currSong.subscribe((newSong: Song | null) => {
        if (!newSong) return;
        used = newSong === song;
    })
</script>

<Card class="flex justify-between items-center gap-4 w-full">
    <div class="flex-grow-0">
        <h1 class="text-lg font-semibold">{song.getName()}</h1>
        <h3 class="text-sm">{song.getCaption()}</h3>
    </div>
    <button disabled={used} on:click={() => {song.setAsCurrentSong()}} class="{used ? "bg-indigo-400" : "bg-indigo-600 hover:bg-indigo-900"}  text-white py-1.5 px-3 w-32 rounded-md">
        {used ? "Current Song" : "Use Song"}
    </button>
</Card>