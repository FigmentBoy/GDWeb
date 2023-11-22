import { writable } from "svelte/store";

export const currSong = writable<Song>();

export interface Song {
    link: string;

    getName(): string;
    getCaption(): string;
    setAsCurrentSong(): any;
}

const officialNames = [
    "Stereo Madness",
    "Back On Track",
    "Polargeist",
    "Dry Out",
    "Base After Base",
    "Cant Let Go",
    "Jumper",
    "Time Machine",
    "Cycles",
    "xStep",
    "Clutterfunk",
    "Theory of Everything",
    "Electroman Adventures",
    "Clubstep",
    "Electrodynamix",
    "Hexagon Force",
    "Blast Processing",
    "Theory of Everything 2",
    "Geometrical Dominator",
    "Deadlocked",
    "Fingerdash",
];

const officialAuthors = [
    "ForeverBound",
    "DJVI",
    "Step",
    "DJVI",
    "DJVI",
    "DJVI",
    "Waterflame",
    "Waterflame",
    "DJVI",
    "DJVI",
    "Waterflame",
    "DJ-Nate",
    "Waterflame",
    "DJ-Nate",
    "DJ-Nate",
    "Waterflame",
    "Waterflame",
    "DJ-Nate",
    "Waterflame",
    "F-777",
    "MDK",
];

export class OfficialSong implements Song {
    name: string;
    author: string;
    link: string;

    constructor(id: number) {
        this.name = officialNames[id];
        this.author = officialAuthors[id];
        this.link = `/officialSongs/${id}.mp3`;
    }

    getName() {
        return this.name;
    }

    getCaption() {
        return `By ${this.author} (Official)`;
    }

    setAsCurrentSong() {
        (<any>window).Module._pause();

        (<any> window).Module.audio = new Audio(this.link);
        (<any> window).Module.audio.load();
        currSong.set(this);
    }
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
        return `By ${this.author} (Newgrounds)`;
    }

    setAsCurrentSong() {
        (<any>window).Module._pause();

        (<any> window).Module.audio = new Audio(this.link);
        (<any> window).Module.audio.load();
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
        (<any>window).Module._pause();

        let audio = new Audio();
        audio.crossOrigin = "anonymous";
        audio.src = this.link;

        (<any> window).Module.audio = audio;
        (<any> window).Module.audio.load();
        currSong.set(this);
    }
}