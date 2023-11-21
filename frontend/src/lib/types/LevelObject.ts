import { type Song } from "./Song";

export class LevelObject {
    name: String = "Level";
    description: String = "Description";
    id: String = "0000000";

    authorID: String = "";
    authorName: String = "Unknown";

    songID: number = 0;
    song: Song | null = null;

    levelString: String = "";

    difficulty: String = "unrated";
    rating: String = "";

    demon: boolean = false;
    demonDifficulty: String = "hard";

    epic: boolean = false;
    feature: boolean = false;

    constructor(levelString: String) {
        const components = levelString.split(":");

        for (let i = 0; i < components.length; i += 2) {
            const key = components[i];
            const value = components[i + 1];

            switch (key) {
                case "1":
                    this.id = value;
                    break;
                case "2":
                    this.name = value;
                    break;
                case "3":
                    this.description = atob(value.replace(/_/g, "/").replace(/-/g, "+"));
                    break;
                case "4":
                    this.levelString = value;
                    break;
                case "6":
                    this.authorID = value;
                    break;
                case "9":
                    switch(value) {
                        case "10":
                            this.difficulty = "easy";
                            break;
                        case "20":
                            this.difficulty = "normal";
                            break;
                        case "30":
                            this.difficulty = "hard";
                            break;
                        case "40":
                            this.difficulty = "harder";
                            break;
                        case "50":
                            this.difficulty = "insane";
                            break;
                    }
                    break;
                case "12":
                    this.songID = parseInt(value);
                    break;
                case "17":
                    this.demon = value == "1";
                    break;
                case "19":
                    this.feature = value != "0";
                    break;
                case "25":
                    if (value == "1") {
                        this.difficulty = "auto";
                    }
                    break;
                case "35":
                    this.songID = parseInt(value);
                    break;
                case "42":
                    this.epic = value == "1";
                    break;
                case "43":
                    switch (value) {
                        case "3":
                            this.demonDifficulty = "easy";
                            break;
                        case "4":
                            this.demonDifficulty = "medium";
                            break;
                        case "0":
                            this.demonDifficulty = "hard";
                            break;
                        case "5":
                            this.demonDifficulty = "insane";
                            break;
                        case "6":
                            this.demonDifficulty = "extreme";
                            break;
                    }
                    break;
            }
        }
    }

    getImageUrl = () => {
        let out: String;

        if (this.demon) {
            out = `demon-${this.demonDifficulty}`;
        } else {
            out = this.difficulty;
        }

        if (this.epic) {
            out += "-epic";
        } else if (this.feature) {
            out += "-featured";
        }

        return `/assets/difficulties/${out}.png`;
    }
}