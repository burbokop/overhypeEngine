'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Module: require('./module'),
};

class Docker {
    constructor(parent) {
        this.parent = parent;
    }

    work(units) {
        if(this.targetUnit) this.dockImmediately();

        if(this.protector) {
            const direction = this.protector.pos.subtract(this.parent.pos).add(this.targetPort.offset);
            this.parent.acc = new api.oh.math.Vector();
            if(direction.module() > 2) {
                this.parent.vel = this.parent.vel.relativisticAddition(direction.normalized(), direction.module() / 200);
            } else {
                this.parent.vel = new api.oh.math.Vector();
            }

            //this.parent.angle = 0;
        }
    }

    dockImmediately() {
        if(this.targetUnit.pos.subtract(this.parent.pos).add(this.targetPort.offset).module() <= 30) {
            this.protector = this.targetUnit;
            this.targetUnit = undefined;

            this.targetPort.docked = this.parent;
            this.targetPort.lock();

            this.parent.lockAngle(0);

            const sound = api.res.sound('docking');
            if(this.soundChannel !== undefined && typeof sound === 'number') api.oh.sound.play(sound, this.soundChannel, 0);
            console.log('docked', this.soundChannel);
        }
    }

    dock(unit) {
        if(this.protector === undefined) {
            const dockPort = unit.moduleHandler.getFreeType('dock');
            if(dockPort && (unit.controller === undefined || unit.controller.dockRequest())) {
                this.targetUnit = unit;
                this.targetPort = dockPort;
                console.log('ready to dock');
            }
        }
    }

    undock() {
        this.parent.unlockAngle();
        this.protector = undefined;

        this.targetPort.docked = this.parent;
        this.targetPort.unlock();

        const sound = api.res.sound('undocking');
        if(this.soundChannel !== undefined && typeof sound === 'number') api.oh.sound.play(sound, this.soundChannel, 0);
        console.log('undocked', this.soundChannel);
    }
}

module.exports = Docker;