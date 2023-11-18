<script lang="ts">
	import LoadingIndicator from './lib/LoadingIndicator.svelte';
  import Renderer from "./lib/Renderer.svelte";
  import Menu from "./lib/Menu.svelte";
  import PlayPauseButton from './lib/PlayPauseButton.svelte';
  import Github from './lib/Github.svelte';

  let initialized: boolean = false;
</script>

{#if initialized}
<Menu />
<PlayPauseButton />
{/if}

<Github />
<LoadingIndicator bind:initialized />
<Renderer />