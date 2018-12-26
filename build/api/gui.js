'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
};

class GUI {
    constructor() {
        this.active = 0;
        this.current = undefined;
        this.stack = [];
    }

    render(surface, resolution, font) {
        const cell1Width = resolution.w / 12;
        const cell1Height = resolution.w / 12;

        const cell2Width = resolution.w / 6;
        const cell2Height = resolution.w / 6;

        const padding = 20;
        const margin = 20;

        if(this.current) {
            if(api.oh.event.pressed()) {
                if(api.oh.event.code(81)) {
                    this.active++;
                    if(this.active >= this.current.content.length) this.active = 0;
                } else if(api.oh.event.code(82)) {
                    this.active--;
                    if(this.active < 0) this.active = this.current.content.length - 1;
                } else if(api.oh.event.code(40)) {
                    const element = this.current.content[this.active];
                    if(this.current.onEnter) this.current.onEnter(element);
                    
                    if(element && element.content) {
                        this.stack.push(this.current);
                        this.current = element;
                        this.active = 0;
                    }
                    if(element.onEnterSelf) element.onEnterSelf();
                } else if(api.oh.event.code(42)) {
                    this.back();
                } 
            }

            api.oh.spm.blendedText(surface, typeof this.current.title === 'function' ? this.current.title() : this.current.title, padding, padding, 0xC97878);
            api.oh.spm.line(surface, 0, padding + margin * 1.5, cell1Width, padding + margin * 1.5, 0xC97878);

            for(let i = 0; i < this.current.content.length; i++) {
                
                const element = this.current.content[i];
                
                const x = padding;
                const y = padding + margin * (i + 2);
                const color = i === this.active ? 0xD2BB5F : 0xC97878;

                if(this.current.toGUI !== undefined) {
                    api.oh.spm.blendedText(surface, this.current.toGUI(element), x, y, color);
                } else if(typeof element === 'object' && element.title) {
                    if(element.title) {
                        if(typeof element.title === 'function') {
                            api.oh.spm.blendedText(surface, element.title(), x, y, color);
                        } else {
                            api.oh.spm.blendedText(surface, element.title, x, y, color);
                        }
                    }
                } else if(typeof element === 'string') {
                    api.oh.spm.blendedText(surface, element, x, y, color);
                } else {
                    api.oh.spm.blendedText(surface, element.toString(), x, y, color);
                }
            }
        }
    }

    setList(list) {
        this.current = list;
        this.list = list;
    }

    back() {
        if(this.stack.length > 0) {
            this.current = this.stack.pop();
            this.active = 0;
        }
    }
}


module.exports = GUI;