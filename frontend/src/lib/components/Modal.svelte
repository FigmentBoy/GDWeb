<script lang="ts">
    import Card from './Card.svelte';
    import { fade } from 'svelte/transition';

    let extraClasses: string = "";
    export { extraClasses as class };
</script>

<div transition:fade class="fixed z-50 top-0 left-0 w-full h-full backdrop-blur-md flex justify-center items-center">
    <Card class="flex flex-col justify-start w-full max-w-[50rem] max-h-[30rem] {extraClasses}">
        <slot></slot>
    </Card>
</div>
