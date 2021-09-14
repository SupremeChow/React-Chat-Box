
const config = require('./utils/config') //configuration, such as addresses, ports, keys, etc that other files can access
const express = require('express')

//For Testing
//const theAddon = require('./binding.js')

//This should get the TriesAddon as an object that we can instatiate
const triesAddon = require('./build/Release/triesAddon')


const http = require('http')
const WebSocket = require('ws')
const cors = require('cors')
const _ = require('lodash')

const app = express()

const server = http.createServer(app)


app.use(express.json())
app.use(cors())




const webSocketServer = new WebSocket.Server({server})
const aTriesObject = new triesAddon.TriesWrapper()


//**** Startup Method for grabbing filter words from database and applying to Tries
const chatFilterDb = require('./services/TriesDBGrabber')

let filteredWords
(async()=>{
	await chatFilterDb.getAllBlockedWords()
		.then((result) =>{
			result.forEach(word =>{
				if(word.isBlocked)
				{
					aTriesObject.AddWord(word.word)
				}

			})
			
			
			//console.log('Async get', result)
			//filteredWords = result
		})
		//These are for testing purpose, remove when confident assignment works]
		/*
		.then(() =>{
			console.log('Second then should assign filterd Words', filteredWords)
			filteredWords.forEach(word =>{
				if(word.isBlocked)
				{
					aTriesObject.AddWord(word.word)
				}

				})
		})
		.then(()=>{
			//sanity Check block
			console.log('Seeing if BOhio is a word in new TriesAddon', aTriesObject.CheckIfWord('Ohio'))
			console.log('Seeing if tiger is a word in new TriesAddon', aTriesObject.CheckIfWord('tiger'))

			console.log('Seeing if bigssss is a word in new TriesAddon', aTriesObject.CheckIfWord('bigssss'))
		})
		*/
})()







//This holds the list of connected clients
const clientList = {}
//TODO need a ID generator to create ID, of now increment a number
let idNum = 0


//******* WebSocket Handling *****************
webSocketServer.on('connection', (ws) => {


	//TODO need a ID generator to create ID, of now increment a number
	
	//Add id field to client connection ws
	ws.id = idNum
	
	//increment id
	idNum++
	
	//Add id and correspoinding socket to client list
	clientList[ws.id] = ws

	//******** Handling Message Received
	ws.on('message', (data) =>{
		
		//Check validity with addon, then stamp with date and (TODO id)
		//send out to everyone to read
		message = JSON.parse(data)
		
		if(!(message.word))
		{
			message.error = 'No Word given'
			ws.send(message)
		}
		else
		{
			
			//Parse into array if sentence
			const wordArray = _.split(message.word, /[^a-zA-Z0-9]/gm)
			
			const wordCheck = wordArray.map((word) => {
				
				//For testing Old Addon, don not use
				//const result = theAddon(word)
				
				
				const result = aTriesObject.CheckIfWord(word)
				if(result)
				{
					message.isWordResult = true
				}
				return ({word, result})
			})
			
			/*
			const isWord = theAddon(message.word)
			
			console.log('Tries determined: ', isWord)
			message.isWordResult = isWord
			*/
			
			
			message.wordsChecked = wordCheck
			
		}
		
		if(message.isWordResult)
		{
			message.error = "Filtered Word Detected"
			//Not a word, send back to user
			ws.send(JSON.stringify(message))
		}
		else
		{
			//stamp with data and broadcast the message to everyone, including source
			message.date = new Date()
			
			message.senderId = ws.id
			
			Object.keys(clientList).forEach( (clientId) =>{
				
				
				if(Number(clientId) === message.senderId)
				{
					message.ownMessage = true
				}
				else
				{
					message.ownMessage = false
				}
				
				clientList[clientId].send(JSON.stringify(message))
				
			})
		}

		
	})
	
	//***********   Handling Client closing connection   
	ws.on('close', () => {
		delete clientList[ws.id]
		delete ws.id
	})
})





//****************   Old Axios Submission Test *********************
//****************  (DO NOT USE!!!! FOR TESTING ONLY!!!) *************


/*


app.post('/service/wordCheck/', (request, response, next) => {
	
	
	//TODO: in future when sending in sentences of words, use lodash _.split(str, delimitor), where delimtor is a regular expression for multiple symbols to deliminate
	
	
	console.log('Server has received request, procesing')
	
	const body = request.body
	
	if(!(body.word))
	{
		response.status(400).json({
			error: 'missing word for checking...'
		})
	}
	
	else
	{
		
		const isWord = theAddon(body.word)

		
		response.json({isWordResult: isWord})
	}
	
	
	console.log('Response set, should have gone throug trie, so should return rquest')
	
})


const errorHandler = (error, request, response, next) =>{
	console.error(error.message)
	
	//Place any specific error handling here
	
	if(error.name === 'CastError')
	{
		return response.status(400).send({error: 'malformated id'})
	}
	else if(error.name === 'ValidationError')
	{
		return (response.status(400).json({error : error.message}))
	}
	
	next(error)
}

app.use(errorHandler)

*/


server.listen(config.PORT, () => {
	console.log(`Server running on port ${config.PORT}`)
})