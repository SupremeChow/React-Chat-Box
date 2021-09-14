const mongoose = require('mongoose')
const express = require('express')
const app = express()
const config = require('../utils/config') //configuration, such as addresses, ports, keys, etc that other files can access





//TODO move to env varaibles





//In the future could expand this to allow for keywords beyond blacklist, such as commands or macros and such
//for now, we'll keep it simple with entries being a string, and a Boolean value isBanned
//Since we are pretending that the Tries is used as a chat filter


const getAllBlockedWords = async () => {
	
	

	const url = config.MONGODB_URI
	
	
	mongoose.connect(url)
	.then(result=>{
		console.log('grabber connected to MongoDB')
	})
	.catch(error => {
		console.error('error connecting to MongoDB from grabber', error.message)
	})
	
	
	const wordSchema = new mongoose.Schema({
		word: 
		{
			type: String,
			required: true
		},
		isBlocked:
		{
			type: Boolean,
			required: true
		},	
	})


	wordSchema.set('toJSON',{
		transform: (document, returnedObject) => {
			returnedObject.id = returnedObject._id.toString()
			delete returnedObject._id
			delete returnedObject.__v
		}
	})
	
	const Word = mongoose.model('Word', wordSchema)
	
	
	return await Word.find({}) 
	
}

module.exports = {getAllBlockedWords}
	
	