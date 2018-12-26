'use strict';

const api = {
    oh: require('overhype'),
    res: require('./res'),
    game: require('./game'),
    GUI: require('./api/gui'),
    Near: require('./api/near'),
    Background :require('./api/background'),
    //wt: require('worker_threads'),
}

api.oh.structure();
api.oh.init();

const window = { title: 'project 17', w: 1000, h: 600 }

const screen = api.oh.createWindow(window);

api.oh.event.start();

console.log('argc:', process.argc, 'argv:', process.argv);

api.res.loadDir(api.res.prefix + "/resources", process.argv[2] === '-no-audio');

const game = api.game.loadGame();


const backgroundSound = api.res.sound('background');
if(backgroundSound !== undefined) {
    const backgroundChannel = api.oh.sound.channel();
    api.oh.sound.play(backgroundSound, backgroundChannel);
    api.oh.sound.volume(backgroundChannel, 16);
}

const near = new api.Near();
const background = new api.Background(100, window);
const font = api.oh.loadFont(api.res.prefix + '/node_modules/overhype/ttf/Caviar_Dreams_Bold.ttf', 16);

const gui = new api.GUI();
gui.setList({
    title: 'main',
    content: [
        {
            title: 'enter for help',
            content: [
                'this is menu',
                'up down arrows =',
                '= navigation',
                'backspace =',
                '= back',
                '------------',
                'wasd for fly',
                'space for fire',
                '------------',
                'this buttons',
                'are not active'
            ]
        },
        {
            title: 'autopilot',
            content: game.player.parent.autopilot.commands,
            toGUI: object => object.title,
            onEnter: object => {
                game.player.parent.autopilot.active = object;
            }
        },
        {
            title: 'modules',
            content: game.player.parent.moduleHandler.modules,
            toGUI: object => object.key + ' ' +
            (object.timer !== undefined ? (new Date().getTime() - object.timer.duration < object.timer.interval ?
            '-'.repeat((new Date().getTime() - object.timer.duration) / object.timer.interval * 8) +
            ' loading' : '-------- ready') : '--|'),
        },
        {
            title: () => game.player.parent.docker.protector !== undefined ? 'undock' : 'dock',
            content: near.getObjects(),
            toGUI: object => object.name,
            onEnter: object => {
                if(game.player.parent.docker.protector === undefined) {
                    game.player.parent.docker.dock(object);
                    gui.back();
                }
            },
            onEnterSelf: () => {
                if(game.player.parent.docker.protector !== undefined) {
                    game.player.parent.docker.undock();
                    gui.back();
                }
            }
        },
        {
            title: 'station',
        }
    ]
});

setInterval(() => {
    if(api.oh.event.mouse().x > 970) {
        api.oh.quit();
        process.exit(0);
    }

    background.work(game.units);
    near.work(game.units, game.player);
    near.workSelf(game.player);
    
    const offset = game.camera.pos.subtract(new api.oh.math.Vector(window.w / 2, window.h / 2));

    background.render(screen, offset, window);

    for(const unit of game.units) {
        if(unit.work) unit.work(game.units);
        unit.render(screen, offset);
    }

    gui.render(screen, window, font);
    
    api.oh.updateWindowSurface();
}, 10);