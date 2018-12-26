'use strict';

const api = {
    fs: require('fs')
}

const prefix = __dirname + '/api';


#

const newClass = (className) => {
    const content = `    
    'use strict';

    const api = {
        oh: require('overhype'),
        res: require('./../res'),
    };
    
    class ${className} {
        constructor( /*args*/ ) {
            this.private = {};
            
        }
    
    }
    
    module.exports = ${className};`

    api.fs.writeFileSync(prefix + '/' + className + '.js', content, 'utf8');
}


if(process.argv.length > 2) {
    const className = process.argv[2];
    newClass(className);
    console.log(className + ' created');
} else {
    console.log('Missing argument');
    console.log('  Usege: node ./new.js [class name]');
}
