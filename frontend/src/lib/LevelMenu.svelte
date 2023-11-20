<script lang="ts">
	import Level from './components/Level.svelte';
    import { LevelObject } from './types/LevelObject';

	import Divider from './components/Divider.svelte';
	import SearchBar from './components/SearchBar.svelte';
	import Card from './components/Card.svelte';

    let menuOpen: boolean = false;

    let openMenu = () => {
        menuOpen = true;
    }

    let closeMenu = () => {
        menuOpen = false;
    }

    let onSearch = async (query: string) => {
        const response = await fetch(`/search/${encodeURIComponent(query)}`).then(async response => await response.text());
        levels = response.split("#")[0].split("|").map(level => new LevelObject(level));
        
        const levellist = document.getElementById("levellist");
        if (levellist) levellist.scrollTop = 0;
    }

    let levels: LevelObject[] = [];

    // void loadLevel(const char* name, const char* string, int audioTrack)
    let loadLevel = async (level: LevelObject) => {
        closeMenu();

        const response = await fetch(`/level/${level.id}`).then(async response => await response.text());
        const levelDataObject = new LevelObject(response);

        const win: any = <any>window;
        win.Module._loadLevel(win.stringToNewUTF8(levelDataObject.name), win.stringToNewUTF8(levelDataObject.levelString), levelDataObject.song);
    }
</script>

{#if menuOpen}
    <Card class="fixed top-4 left-4 max-w-[30rem] max-h-[40rem] w-full flex flex-col justify-start {levels.length == 0 ? '' : 'pb-0'}">
        <div class="flex justify-between items-center mb-4">
            <h1 class="text-2xl font-bold">Levels</h1>

            <button class="w-8 h-8" on:click={closeMenu}>
                <i class="fas fa-times" />
            </button>
        </div>
        <SearchBar placeholder={"Level Name or ID"} {onSearch} />
        {#if levels.length != 0}
            <Divider class="mb-0" />
            <div class="levellist overflow-y-auto overflow-x-visible flex flex-col gap-2 p-4" id="levellist">
                {#each levels as level}
                    <Level on:click={()=>{loadLevel(level)}} {level} />
                {/each}
            </div>
        {/if}
    </Card>
{:else}
    <Card on:click={openMenu} class="fixed top-4 left-4 cursor-pointer h-12 w-12 flex justify-center items-center">
        <i class="fas fa-bars fa-lg" />
    </Card>
{/if}

<style>
    .levellist {
        width: calc(100% + 2rem);
        margin-left: -1rem;
        margin-right: -1rem;
    }

    .levellist::-webkit-scrollbar {
        width: 0;
    }
</style>