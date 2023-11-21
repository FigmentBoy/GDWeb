<script lang="ts">
	import Divider from './components/Divider.svelte';
	import Card from './components/Card.svelte';
    import Checkbox from './components/Checkbox.svelte';
    import { onMount } from 'svelte';

    let menuOpen: boolean = false;

    let openMenu = () => {
        menuOpen = true;
    }

    let closeMenu = () => {
        menuOpen = false;
    }

    let onShowTriggers = (value: boolean) => {
        const win: any = <any>window;
        win.Module._showTriggers(value);
        localStorage.setItem("showTriggers", value.toString());
    }

    let showTriggers: boolean = false;

    onMount(() => {
        let showTriggersString = localStorage.getItem("showTriggers");
        showTriggers = showTriggersString ? JSON.parse(showTriggersString) === true : false;

        const win: any = <any>window;
        let interval = setInterval(() => {
            if (win.Module.calledRun) {
                clearInterval(interval);
                onShowTriggers(showTriggers);
            }
        }, 100);
    });
</script>

{#if menuOpen}
    <Card class="fixed bottom-4 right-4 max-w-[20rem] max-h-[40rem] w-full flex flex-col justify-start">
        <div class="flex justify-between items-center">
            <h1 class="text-2xl font-bold">Settings</h1>

            <button class="w-8 h-8" on:click={closeMenu}>
                <i class="fas fa-times translate-x-1/2" />
            </button>
        </div>
        <Divider />
        <Checkbox label="Show Triggers" onChecked={onShowTriggers} bind:checked={showTriggers} />
    </Card>
{:else}
    <Card on:click={openMenu} class="fixed bottom-4 right-4 cursor-pointer h-12 w-12 flex justify-center items-center">
        <i class="fas fa-cog fa-lg" />
    </Card>
{/if}