import { writable } from "svelte/store";

export const currSong = writable<Song>();

export interface Song {
    link: string;

    getName(): string;
    getCaption(): string;
    setAsCurrentSong(): any;
}

export class NGSong implements Song {
    name: string;
    author: string;
    link: string;

    constructor(name: string, author: string, id: number) {
        this.name = name;
        this.author = author;
        this.link = `/audio/${id}`;
    }

    getName() {
        return this.name;
    }

    getCaption() {
        return this.author;
    }

    setAsCurrentSong() {
        (<any> window).Module.audio = new Audio(this.link);
        currSong.set(this);
    }
}

export interface RAWNONGSong {
    name?: string;
    songName?: string;
    songID?: string;
    downloadUrl?: string;
}

export class NONGSong implements Song {
    name: string;
    caption: string;
    link: string

    constructor(song: RAWNONGSong) {
        this.name = song.songName ?? "Unknown";
        this.caption = song.name ?? "";
        this.link = song.downloadUrl?.replace(/download=true/g, "") ?? `/audio/${song.songID}`;
    }

    getName() {
        return this.name ?? "Unknown";
    }

    getCaption() {
        return this.caption ?? "000000";
    }

    setAsCurrentSong() {
        let audio = new Audio();
        audio.crossOrigin = "anonymous";
        audio.src = this.link;

        (<any> window).Module.audio = audio;
        currSong.set(this);
    }
}