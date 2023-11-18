<script lang="ts">
    export let placeholder: string = "";
    export let onSearch: (value: string) => Promise<void>;

    let loading: boolean = false;
    let value: string = "";

    let searchHandler = async (event: SubmitEvent) => {
        loading = true;
        await onSearch(value);
        loading = false;
    }
</script>

<form on:submit|preventDefault={searchHandler} class="flex gap-3 w-full">
    <input id="searchbar" type="text" disabled={loading} bind:value {placeholder} class="block w-full rounded-md border-0 py-1.5 px-3 text-gray-900 ring-1 ring-inset ring-gray-300 placeholder:text-gray-400 focus:ring-2 focus:ring-inset focus:ring-indigo-600 flex-grow">
    <button type="submit" disabled={loading} class="bg-indigo-600 hover:bg-indigo-900 text-white py-1.5 px-3 rounded-md">
        <i class="fas fa-circle-notch fa-spin {loading ? "block" : "hidden"}" /> 
        <i class="fas fa-search inline {loading ? "hidden" : "block"}" />
    </button>
</form>