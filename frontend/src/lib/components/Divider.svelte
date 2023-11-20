<script lang="ts">
	import { scale } from 'svelte/transition';
    let extraClasses: string = "";
	export { extraClasses as class };
</script>

<hr class="solid my-4 {extraClasses}">