'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Controller: require('./controller'),
    Autopilot: require('./autopilot'),
    ModuleHandler: require('./moduleHandler'),
};

class Near {
    constructor() {
        this.objects = [];
        this.i = 0;
        this.j = 0;
        this.radius = 200;
    }

    work(units, target) {
        const unit = units[this.i];

        if(unit !== undefined && unit !== target.parent && this.objects.indexOf(unit) === -1) {
            if(unit.pos.subtract(target.parent.pos).module() <= this.radius) this.objects.push(unit);
        }

        this.i++;
        if(this.i >= units.length) this.i = 0;
    }

    workSelf(target) {
        const object = this.objects[this.j];
        
        if(object !== undefined) {
            if(object.pos.subtract(target.parent.pos).module() > (this.radius + 10)) {
                this.objects.splice(this.j, 1);
            }
        }

        this.j++;
        if(this.j >= this.objects.length) this.j = 0;
    }

    getObjects() {
        return this.objects;
    }
}


module.exports = Near;