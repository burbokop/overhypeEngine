'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
};


let playerExist = false;


class Controller {
    constructor(parent, type, target) {
        this.parent = parent;
        this.type = type;
        this.target = target;
        
        if (type === 'player') {
            if(playerExist) this.type = 'not-player';
            playerExist = true;
        }
    }

    work() {
        if(this.type === 'camera') {
            this.onCamera();
        } else if (this.type === 'player') {
            this.onPlayerControl();         
        } else {
            
        }
    }

    onCamera() {
        const direction = this.target.parent.pos.subtract(this.parent.pos);
        if(direction.module() > 2) {
            this.parent.acc = direction.multiply(this.target.parent.vel.module());
            this.parent.maxVel = direction.module() / 200;
        } else {
            this.parent.acc = new api.oh.math.Vector();
            this.parent.vel = new api.oh.math.Vector();
        }
    }

    onPlayerControl() {
        if(api.oh.event.code(4)) {
            this.parent.angle -= 0.01;
        } else if(api.oh.event.code(7)) {
            this.parent.angle += 0.01;
        }

        const weapon = this.parent.moduleHandler.getFreeType('weapon');
        const engine = this.parent.moduleHandler.getFreeType('engine');
        
        if(weapon) {
            weapon.fire = api.oh.event.code(44);
        }

        if(engine) {
            if(api.oh.event.code(26)) {
                if(!this.pressed26) {
                    this.pressed26 = true;
                    this.parent.audioPlayer.play(engine, 24);
                }
                
                this.parent.acc = api.oh.math.Vector.createByAngle(0.01, this.parent.angle);
            } else {
                if(this.pressed26) {
                    this.pressed26 = false;
                    this.parent.audioPlayer.stop(engine);
                }

                this.parent.acc = this.parent.vel.module() > 0.015 ? api.oh.math.Vector.createByAngle(-0.01, this.parent.vel.angle()) : new api.oh.math.Vector();
            }
        }
    }

    dockRequest() {
        return true;
    }
}


module.exports = Controller;