import React, { useState, useEffect, useRef } from 'react'
import './App.css';
import axios from 'axios'
import wordChecker from './services/wordChecker'
import _ from 'lodash'



//*** Styles ***  

const chatBoxStyle = 
{
	margin: 'auto',
	borderRadius: '5px',
	borderStyle: 'solid',
	borderColor: 'blue',
	backgroundColor: 'grey',
	
	marginTop: '10px',
	marginRight: '10px',
	marginBottom: '10px',
	marginLeft: '10px',
	
	paddingTop: '10px',
	paddingRight: '12px',
	paddingBottom: '10px',
	paddingLeft: '2px',
	
	width: '720px',
	height: '260px',
	overflow: 'scroll',
	
	display: 'flex',
	flexDirection: 'column-reverse'
}
const chatBoxTextStyle = 
{
	color: 'white'
}
const textAlignLeft =
{
	
	align: 'left',
	textAlign: 'left',
	marginRight: '120px',
	paddingRight: '20px',
	marginLeft: '20px',
	marginTop: '50px',
	marginBottom: '50px',
	wordWrap: 'break-word'
}
const textAlignRight =
{
	align: 'right',
	textAlign: 'right',
	marginRight: '20px',
	paddingRight: '20px',
	marginLeft: '120px',
	marginTop: '50px',
	marginBottom: '50px',
	wordWrap: 'break-word'
	
}
const hiddenListBulletStyle =
{
	listStyleType: 'none'
}



//*** Seperate Modules ***  

const AlertMessage = ({isGood, message}) => {
	const alertStyle = {
		color: (isGood) ? 'green' : 'red',
		fontSize: 35,
		borderRadius: '5px',
		borderStyle: 'solid',
		borderColor: (isGood) ? 'green' : 'red',
		backgroundColor: 'grey',
		
		marginTop: '10px',
		marginRight: '10px',
		marginBottom: '10px',
		marginLeft: '10px',
		
		paddingTop: '10px',
		paddingRight: '12px',
		paddingBottom: '10px',
		paddingLeft: '2px',
	}
	const divSpacerStyle = {
		marginTop: '20px',
		marginRight: '10px',
		marginBottom: '20px',
		marginLeft: '10px'
	}
	
	if(message === null)
	{
		return(
			null
		)
	}
	
	return(
		<div style={divSpacerStyle}>
			<span style = {alertStyle}> {message} </span>
		</div>
	)
}



const ChatBox = ({chatWords}) => {
	
	return(
		<div style = {chatBoxStyle}>
			<ul style = {hiddenListBulletStyle}>
			{
			
			
				chatWords.map( (aWord, index) => {
					if(aWord.ownMessage)
					{
						return(
							<li key = {index} style = {textAlignLeft}>
								<p style = {chatBoxTextStyle}>{aWord.word}</p>
							</li>
						)
					}
					return(
						<li key = {index} style = {textAlignRight}>
							<p style = {chatBoxTextStyle}>{aWord.word}</p>
						</li>
					)
					
				})
			}
			</ul>
		</div>
	)
}






function App() {

	
	const [newWord, setNewWord] = useState('')
	const [currentWords, setCurrentWords] = useState([])
	const [alertMessage, setAlertMessage] = useState(null)
	const [alertTypeGood, setAlertTypeGood] = useState(true)
	
	
	//TODO move to a env vavriable if deployed
	const CHAT_SERVER_ADDRESS = 'ws://localhost:3002'
	
	
	//For Axios testing pruposes
	//const [testSwapId, setTestSwapId] = useState(false)
	
	
	
	

	
//***    WebSocket Creation and usage  ****

	const chatConnection = useRef(null)
	
	useEffect(() => {
		chatConnection.current = new WebSocket(CHAT_SERVER_ADDRESS)
		
		
		
		//*** On Connection to Server ***  
		
		chatConnection.current.addEventListener('open',(event) =>{
			console.log('has connected')
		})  
		
		
		
		//*** On Receiving a Message From Server ***  
		
		chatConnection.current.addEventListener('message', (event) => {
			const message = JSON.parse(event.data)
			//console.log('Client received message: ', message)
			
			//Handle message interpretation here...
			if(message.error)
			{
				console.log('The sent data has an error...')
				console.log(message.error)
				
				//if(filtred word) { trigger message box of for user}
				if(message.error === 'Filtered Word Detected')
				{
					const deniedWords = message.wordsChecked.filter((wordObject) => {
						return wordObject.result
					})
					.map((wordObject) => wordObject.word)
							
					const deniedString = deniedWords.toString()
					
					//Create Temporary Error Message For Denied Words
					setAlertMessage(`The words ${ deniedString } are on the filter list, and are not allowed`)
					setAlertTypeGood(false)
					
					setTimeout(() => {
						setAlertMessage(null)	
					}, 5000)
				}
			}
			else
			{
				//Concat to word box
				setCurrentWords((c) => c.concat(message))
								
				if(message.ownMessage)
				{
					setNewWord('')
				}
				
			}
		}) 
			
			
			
		//*** On Closing Connection to Server ***  
		chatConnection.current.addEventListener('close', (event) => { 
			console.log('connection closed') 
		})
			
		
		//*** On Error ***  
		chatConnection.current.addEventListener('error', (event) => {
			console.log(event.data)
		})
			
			
	}, [])
		
	
	//*** Handles the total number of chat submissions in chat box ***  
	useEffect(() => {
  
		if(currentWords.length > 7)
		{
			//trim to last 7
			const shrunkText = _.takeRight(currentWords, 7)
			setCurrentWords(shrunkText)
		}
	
	}, [currentWords])
	
	
	//*** Submit Word event ***  
	const submitWordWebsocket = (event) =>{
		event.preventDefault()
		
		//Use an object to pass word, since in future could have a sentence that can be parsed into array of words
		const wordObject = {
			word: newWord,
		}
		
		//Submit the word, let server handle parsing and determining if valid, the message listener should determine how to handle any response
		chatConnection.current.send(JSON.stringify(wordObject))

	}
	
	
	
	
	
	
//************** Axios Chat Submission  ***  ********
//********** (DO NOT USE!!! FOR TESTING ONLY!) **************


	/*
	const submitWordAxios = (event) =>{
		event.preventDefault()
		
		console.log('word submitted...')
		
		//call service word to handle axios and calling backend for response
		
		//Use an object to pass word, since in future could have a sentence that can be parsed into array of words
		const wordObject = {
			word: newWord,
			clientId: (testSwapId) ? otherId : clientUserId
      
		}
		
		wordChecker.checkWord(wordObject)
			.then(result => {
				console.log('the REsult: ', result)
				if(!result.error)
				{
					console.log('The sent data has an error...')
				}
				else if(result.isWordResult === true)
				{
					console.log('Reutrned True, pretending this is a censor word...')
					//make error message appear or something
				}
				else
				{
					console.log('Reutrned false, pretending valid word...')
					//Concat to word box
					setCurrentWords(currentWords.concat(wordObject))
					
					setTestSwapId(!testSwapId) //For testing purpose of pretending different users
					
							
					setNewWord('')
					
				}
			})

	}
	*/
	
	
	
	//*** updating Input Box ***  
	const newWordChange = (event) =>{
	
    setNewWord(event.target.value)
	}
	
	
	
	
	//*** The Rendered Page ***  
	return (
		<div>	
			<ChatBox chatWords = {currentWords}/>
			
		
			<form onSubmit={submitWordWebsocket}>
				<input
				  value={newWord}
				  onChange={newWordChange}
				/>
			</form>  
			
			<AlertMessage message = {alertMessage} isGood = {alertTypeGood}/>
			
		</div>
	);
}

export default App;
