'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
};

class Module {
    constructor(amount, resolution) {
        const colors = [0xdeff17, 0xfe4600, 0xf9990f, 0x8a27ff];
        this.mainColor = 0x333353;
        this.stars = [];

        for(let i = 0; i < amount; i++) {
            this.stars.push({
                x: Math.floor(Math.random() * (resolution.w - 1)),
                y: Math.floor(Math.random() * (resolution.h - 1)),
                color: colors[Math.floor(Math.random() * 3)],
            });
        }
    }
    
    work(units) {
        
    }

    render(surface, offset, resolution) {
        if(surface !== undefined && offset !== undefined && resolution !== undefined) {
            api.oh.fillRect(surface, this.mainColor);
            const localOffset = offset.multiply(0.4);
            for(const star of this.stars) {
                let x = Math.floor(star.x - localOffset.coords[0]);
                let y = Math.floor(star.y - localOffset.coords[1]);

                if(x >= 0) x = x % resolution.w;
                else x = x % resolution.w + resolution.w;

                if(y >= 0) y = y % resolution.h;
                else y = y % resolution.h + resolution.h;

                api.oh.spm.pixel(surface, x, y, star.color);
            }
        }
    }
}

module.exports = Module;


