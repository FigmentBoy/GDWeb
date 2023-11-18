<script lang="ts">
	import Card from './Card.svelte';
    import { LevelObject } from '../types/LevelObject';

    export let level: LevelObject;
</script>

<Card on:click class="flex justify-start items-center cursor-pointer hover:shadow-black transition-all">
    <img class="w-12 h-auto" src={level.getImageUrl()} alt={`${level.difficulty} (${level.rating != "" ? level.rating : "Unrated"})`}/>
    <div class="flex flex-col justify-start items-start ml-2">
        <h1 class="text-lg font-bold">{level.name} <span class="text-sm text-gray-500">({level.id})</span></h1>
        <h3 class="text-sm">{level.description}</h3>
    </div>
</Card>