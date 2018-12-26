const api = {
    oh: require('overhype'),
    res: require('./res'),
    Unit: require('./api/unit'),
    Ship: require('./api/ship'),
    Controller: require('./api/controller'),
    Near: require('./api/near'),
    Background :require('./api/background'),
}


const loadGame = () => {
    const units = [];
    const player = new api.Controller(undefined, 'player');




    //PLAYER SHIP
    const playerShip = new api.Ship('sh1', 'vova-ps', 100, 100);
    playerShip.setController(player);
    playerShip.moduleHandler.add('pistol');
    playerShip.moduleHandler.add('engine');
    player.parent.docker.soundChannel = api.oh.sound.channel();
    units.push(playerShip);

    //CAMERA
    const camera = new api.Ship(undefined, 'camera', 150, 150);
    camera.setController(new api.Controller(undefined, 'camera', playerShip.controller));
    units.push(camera);

    //ENEMY
    const enemy = new api.Ship('sh2', 'vova-sh2-en', 200, 200)
    enemy.setController(new api.Controller('npc'));
    enemy.moduleHandler.add('plasma-launcher');
    units.push(enemy);

    //SOME SHIP
    units.push(new api.Ship('sh3', 'vova-sh3', 300, 300));

    //STATION
    const station = new api.Unit('st1', 'station', 100, 300);
    station.moduleHandler.add('dock-1');
    units.push(station);








    return { units, player, camera };
}

module.exports = { loadGame };