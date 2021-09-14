//This .js file is in charge of exporting/binding the addon as a module for index.js

//May not be needed, could just do all this in index, will test that for now

const triesAddon = require('./build/Release/triesAddon')

module.exports = triesAddon