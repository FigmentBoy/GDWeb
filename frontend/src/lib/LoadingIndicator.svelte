<script lang="ts">
    import { onMount } from 'svelte';

    let loadPercent: number;
    let loadStage: number;

    export let initialized: boolean = false;

    onMount(async () => {
        (<any>window).Module = {
            ...(<any>window).Module,
            updateLoading: (percent: number, stage: number) => {
                loadPercent = percent;
                loadStage = stage;
            },
            initialized: () => {
                initialized = true;
            }
        };
    });

    const stages = [
        "Sprites",
        "Level"
    ];
</script>

{#if loadStage >= 0 || loadStage == undefined}
    <div class="absolute left-0 top-0 w-full h-full flex justify-center items-center -z-40">
        {#if loadStage >= 0}
            <h1 class="text-white text-5xl">
                {#if loadPercent != undefined}
                    Loading {stages[loadStage]}: {(loadPercent * 100).toFixed(2)}%
                {:else}
                    Loading Module...
                {/if}
            </h1>
        {:else if loadStage == undefined}
            <h1 class="text-white text-5xl">Initializing...</h1>
        {/if}
    </div>
{/if}