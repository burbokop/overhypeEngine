'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Unit: require('./unit'),
    Docker: require('./docker'),
};

class Ship extends api.Unit {
    constructor(key, name, x, y) {
        super(key, name, x, y);
        this.docker = new api.Docker(this);
        this.vel = new api.oh.math.Vector(0, 0);
        this.acc = new api.oh.math.Vector(0, 0);

        this.angle = 0;
        this.dstangle = 0;
        this.angleLocked = false;
    }
    
    work(units) {
        super.work(units);
        this.docker.work(units);
        
        if(this.angleLocked) {
            const av = this.angleVelocity ? this.angleVelocity : 0.01;
            if(Math.abs(this.angle - this.dstangle) > av * 5) {
                if(this.angle > this.dstangle) this.angle -= av;
                else this.angle += av;
            }
        }
        
        this.vel = this.vel.relativisticAddition(this.acc, this.maxVel);
        this.pos = this.pos.add(this.vel);
    }

    lockAngle(angle) {
        this.dstangle = angle;
        this.angleLocked = true;
    }

    unlockAngle() {
        this.angleLocked = false;
    }
}


module.exports = Ship;