'use strict';

const api = {
    oh: require('overhype'),
    fs: require('fs'),
    Anim: require('./api/animation'),
}


const prefix = __dirname;
const resources = {};
const sounds = {};

const functionCreater = (key, value) => {
    if (typeof value === "function") {
        return "/Function(" + value.toString() + ")/";
    }
    return value;
}
  
const functionParser = (key, value) => {
    if (typeof value === "string" && value.startsWith("/Function(") && value.endsWith(")/")) {
        value = value.substring(10, value.length - 2);
        return eval("(" + value + ")");
    }
    return value;
}

const getSufix = str => {
    const parts = str.split('.');
    return parts[parts.length - 1].toLowerCase();
}

const addPrefix = (str, ap) => {
    if(str.length === 0) return ap ? ap : prefix;
    if(str[0] === '.') return (ap ? ap : prefix) + str.substr(1);
    return str;
}

const processFile = (file, path, noAudio) => {
    const sufix = getSufix(file);
    if(sufix === 'json') {
        const resource = JSON.parse(api.fs.readFileSync(file, { encoding: 'utf8' }), functionParser);
        if(resource.sprite) {
            resource.sprite = api.oh.loadSurface(addPrefix(resource.sprite, path));
            resource.hasSprite = true;
        } else resource.hasSprite = false;
        
        if(resource.animation && resource.animation.spritesheet) {
            resource.animation = new api.Anim(api.oh.loadSurface(
                addPrefix(resource.animation.spritesheet, path)),
                resource.animation.w,
                resource.animation.h,
                resource.animation.mode,
                resource.animation.interval
            );
        }

        if(resource.audio && !noAudio) {
            for(const sample in resource.audio) {
                resource.audio[sample] = api.oh.sound.load(addPrefix(resource.audio[sample], path));
            }
        }

        if(resource.key) {
            resources[resource.key] = resource;
        }
    } else if ((sufix === 'mp3' || sufix === 'ogg' || sufix === 'wav') && !noAudio) {
        let name = file.split('/');
        name = name[name.length - 1];
        name = name.split('.')[0];
        sounds[name] = api.oh.sound.load(file);
    }
};

//oh.sound.play(, 1);


const loadDir = (path, noAudio) => {
    if(noAudio === undefined) noAudio = false;
    if(typeof path === 'string') {
        if(path[path.length - 1] === '/') path = path.slice(0, -1);
        const items = api.fs.readdirSync(path);
        for (const item of items) {
            const file = path + '/' + item;
            const stats = api.fs.statSync(file);
            if(stats.isDirectory()) {
                loadDir(file, noAudio);
            } else {
                processFile(file, path, noAudio);
            }
        }
    } else {
        console.log(path, 'is not a string');
    }
}

const clone = key => {
    const template = resources[key];
    console.log('--||--||--', template);
    
    //if(template) return JSON.parse(JSON.stringify(template, functionCreater), functionParser);
    if(template) return template;

    else undefined;
};

const sound = key => {
    return sounds[key];
};


module.exports = { loadDir, clone, sound, prefix, functionCreater, functionParser };