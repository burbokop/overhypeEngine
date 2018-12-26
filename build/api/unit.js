'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Controller: require('./controller'),
    Autopilot: require('./autopilot'),
    ModuleHandler: require('./moduleHandler'),
    AudioPlayer: require('./audioPlayer'),
};

class Unit {
    constructor(key, name, x, y) {
        Object.assign(this, api.res.clone(key));

        this.name = name;
        
        this.pos = new api.oh.math.Vector(x, y);
        this.angle = 0;

        this.controller = undefined;
        this.autopilot = new api.Autopilot(this);
        this.moduleHandler = new api.ModuleHandler(this);
        this.audioPlayer = new api.AudioPlayer(this);
        if(this.animation) this.animator = new api.Animator(this);
    }

    work(units) {
        if(this.controller) this.controller.work(units);
        if(this.autopilot) this.autopilot.work(units);
        if(this.moduleHandler) this.moduleHandler.work(units);
        if(this.audioPlayer) this.audioPlayer.work(units);
    }

    render(surface, offset) {
        if(surface !== undefined && this.pos) {
            if(this.animation) this.animation.render(surface, offset, offsetLocal);
            else if (this.hasSprite) {
                const pos = {}
                pos.x = this.pos.coords[0] - offset.coords[0];
                pos.y = this.pos.coords[1] - offset.coords[1];
                
                api.oh.spm.rotatedSurface(
                    this.sprite,
                    surface,
                    pos,
                    this.angle ? (this.angle * 180 / -Math.PI) : 0,
                    this.zoom !== undefined ? this.zoom : 1
                );
            }
            this.moduleHandler.render(surface, offset);
        }
    }

    setController(controller) {
        this.controller = controller;
        this.controller.parent = this;
    }
}


module.exports = Unit;