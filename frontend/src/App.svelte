<script lang="ts">
	import LoadingIndicator from './lib/LoadingIndicator.svelte';
  import Renderer from "./lib/Renderer.svelte";
  import LevelMenu from "./lib/LevelMenu.svelte";
  import PlaybackMenus from './lib/PlaybackMenus.svelte';
  import Github from './lib/Github.svelte';
  import SettingsMenu from './lib/SettingsMenu.svelte';

  import { LevelObject } from './lib/types/LevelObject';

  let initialized: boolean = false;
</script>

{#if initialized}
<LevelMenu />
{/if}

<PlaybackMenus />
<Github />
<SettingsMenu />
<LoadingIndicator bind:initialized />
<Renderer />