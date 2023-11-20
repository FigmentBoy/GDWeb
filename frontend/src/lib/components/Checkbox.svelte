<script lang="ts">
    export let checked: boolean = false;
    export let disabled: boolean = false;
    export let label: string = "";

    export let onChecked: (value: boolean) => void = () => {};
</script>

<div class="flex items-center">
    <input id="checkbox" type="checkbox" on:change={() => {onChecked(!checked)}} bind:checked {disabled} class="w-4 h-4 text-indigo-600 border-gray-300 focus:ring-indigo-600 checked:bg-indigo-600 focus:ring-2 focus:ring-inset rounded-md">
    <label for="checkbox" class="ms-2 text-sm font-medium text-black">{label}</label>
</div>