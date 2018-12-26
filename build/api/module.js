    
'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Projectile: require('./projectile'),
    Animation: require('./animation'),
};

class Module {
    constructor(key, parent) {
        this.props = {};
        Object.assign(this, api.res.clone(key));
        if(this.rate) this.timer = 60000 / this.rate;
        if(this.timer) this.timer = new api.oh.math.Timer(this.timer);
        this.parent = parent;
        if(this.audio) this.audio.channel = api.oh.sound.channel();
    
        if(this.type === 'weapon' && this.projectile !== undefined) {
            this.fire = false;
        }
    }
    
    work(units) {
        if(this.type === 'weapon' && this.projectile !== undefined) {
            
            //if(this.timer && this.timer.count(api.oh.event.code(44))) {
            if(this.timer && this.timer.count(this.fire)) {
                const projectile = new api.Projectile(this.projectile, this.parent);
                projectile.vel = api.oh.math.Vector.createByAngle(1, this.parent.angle);
                units.push(projectile);
                this.parent.audioPlayer.play(this, 14);
            }
        } else if(this.type === 'dock') {
     
        } else if(this.type === 'engine') {

        }
    }

    render(surface, offset) {

        if(this.animation) {
            const localPos = this.parent.pos.subtract(offset).add(this.offset);
        
            this.animation.render(surface, localPos);
        }
        else if(this.sprite) {
            const localPos = this.parent.pos.subtract(offset).add(this.offset);
            const x = localPos.x;
            const y = localPos.y;
            api.oh.spm.rotatedSurface(
                this.sprite,
                surface,
                { x, y },
                this.angle ? (this.angle * 180 / -Math.PI) : 0,
                this.zoom !== undefined ? this.zoom : 1
            );
        }
    }

    unlock() {
        this.free = true;
    }

    lock() {
        this.free = false;
    }
}

module.exports = Module;