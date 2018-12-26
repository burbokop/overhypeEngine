'use strict';


const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Command: require('./command'),
};

class Autopilot {
    constructor(parent) {
        this.commands = [];
        this.active;
        this.parent = parent;
    }

    work(units) {
        if(this.active && this.active.work) this.active.work(units);
    }

    addCommand(command) {
        if(typeof command === 'string') {
            command = new api.Command(command);
        } else if (typeof command === 'object') {

        } else return;
        if(command !== undefined) this.commands.push(command);
    }

    makeActive(i) {
        const tmp = this.commands[i];
        if(tmp !== undefined) this.active = tmp;
    }
}

module.exports = Autopilot;