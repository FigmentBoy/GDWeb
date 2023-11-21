import { LevelObject } from "./types/LevelObject";

import { writable } from 'svelte/store';

export const currLevel = writable<LevelObject>();