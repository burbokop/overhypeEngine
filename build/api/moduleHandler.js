'use strict';

const api = {
    oh: require('overhype'),
    res: require('./../res'),
    Module: require('./module'),
};

class ModuleHandler {
    constructor(parent) {
        this.modules = [];
        this.parent = parent;
    }

    work(units) {
        for(const module of this.modules) {
            module.work(units);
        }
    }

    render(surface, offset) {
        for(const module of this.modules) {
            module.render(surface, offset);
        }
    }

    add(key) {
        const module = new api.Module(key, this.parent)
        this.modules.push(module);

        if(module.commands && this.parent.autopilot) {
            for(const command of module.commands) {
                this.parent.autopilot.addCommand(command);
            }
            delete module.commands;
        }

        for(const key in module.props) {
            if(this.parent[key] !== undefined) {
                this.parent[key] += module.props[key];
            } else {
                this.parent[key] = module.props[key];
            }
        }

        module.unlock();
    }

    remove(module) {
        /*
        for(const)

        for(key in module.props) {
            if(this.parent[key] !== undefined) {
                this.parent[key] += module.props[key];
            } else {
                this.parent[key] = module.props[key];
            }
        }
        */
    }

    getFreeType(type) {
        for(const module of this.modules) {
            if(module.type === type && module.free) {
                return module;
            }
        }
        return undefined;
    }
}

module.exports = ModuleHandler;