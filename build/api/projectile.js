'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
};

class Projectile {
    constructor(key, parent) {
        Object.assign(this, api.res.clone(key));
        console.log(parent);
        this.pos = new api.oh.math.Vector(parent.pos.coords[0], parent.pos.coords[1]);
        this.vel = new api.oh.math.Vector(0, 0);
        this.acc = new api.oh.math.Vector(0, 0);
        this.angle = 0;
        this.name = key;
    }
    
    work(units) {
        this.vel = this.vel.relativisticAddition(this.acc, this.maxVel);
        this.pos = this.pos.add(this.vel);
    }

    render(surface, offset) {
        if (this.hasSprite && surface !== undefined && this.pos) {
            const pos = {}
            pos.x = this.pos.coords[0] - offset.coords[0];
            pos.y = this.pos.coords[1] - offset.coords[1];
            
            this.angle = this.vel.angle();

            api.oh.spm.rotatedSurface(
                this.sprite,
                surface,
                pos,
                this.angle * 180 / -Math.PI,
                this.zoom !== undefined ? this.zoom : 1
            );
        }
    }
}


module.exports = Projectile;