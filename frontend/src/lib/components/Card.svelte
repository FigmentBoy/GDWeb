<script lang="ts">
	import { scale } from 'svelte/transition';
    let extraClasses: string = "";
	export { extraClasses as class };
</script>

<div transition:scale on:click class="bg-white p-4 text-black shadow-2xl rounded-lg {extraClasses}">
    <slot></slot>
</div>