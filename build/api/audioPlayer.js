const api = {
    oh: require('overhype'),
    res: require('./../res'),
};

class AudioPlayer {
    constructor() {
        this.modules = [];
        this.signal = 0;
    }

    work(units) {
        for(const module of this.modules) {
            if(module.audio && module.audio.channel) {   
                if(module.audio.signal === 1) {
                    module.audio.signal = 2;
                    if(module.audio.start !== undefined && typeof module.audio.start === 'number') {
                        api.oh.sound.play(module.audio.start, module.audio.channel, 0);
                        api.oh.sound.volume(module.audio.channel, module.audio.channel.volume);
                    }
                } else if(module.audio.signal === 2) {
                    if(!api.oh.sound.playing(module.audio.channel)) {
                        if(module.audio.loop !== undefined && typeof module.audio.loop === 'number') {
                            api.oh.sound.play(module.audio.loop, module.audio.channel, -1);
                            module.audio.signal = 3;
                        } else {
                            module.audio.signal = 0;
                        }
                    }
                } else if(module.audio.signal === 4) {
                    if(module.audio.stop !== undefined && typeof module.audio.stop === 'number') {
                        api.oh.sound.play(module.audio.stop, module.audio.channel, 0);
                    }
                    module.audio.signal = 0;
                } else if(module.audio.signal === 0) {
                    if(!api.oh.sound.playing(module.audio.channel)) {
                        this.modules.splice(this.modules.indexOf(module), 1);
                    }
                }
            }
        }
    }

    play(module, volume) {
        if(module.audio && module.audio.channel) {   
            module.audio.channel.volume = volume !== undefined ? volume : 128;
            module.audio.signal = 1;
            this.modules.push(module);
        }
    }

    stop(module) {
        module.audio.signal = 4;
    }
}


module.exports = AudioPlayer;