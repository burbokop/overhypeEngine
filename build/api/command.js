'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
};

class Command {
    constructor(key) {
        Object.assign(this, api.res.clone(key));
    }

    work(units) {
        console.log('#null');
    }
}


module.exports = Command;