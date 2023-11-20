<script lang="ts">
	import LoadingIndicator from './lib/LoadingIndicator.svelte';
  import Renderer from "./lib/Renderer.svelte";
  import LevelMenu from "./lib/LevelMenu.svelte";
  import PlayPauseButton from './lib/PlayPauseButton.svelte';
  import Github from './lib/Github.svelte';
  import SettingsMenu from './lib/SettingsMenu.svelte';

  let initialized: boolean = false;
</script>

{#if initialized}
<LevelMenu />
<PlayPauseButton />
{/if}

<Github />
<SettingsMenu />
<LoadingIndicator bind:initialized />
<Renderer />