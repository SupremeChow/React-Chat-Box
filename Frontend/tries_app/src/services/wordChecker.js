//TODO only temp, use web sockets for more real time tcp action
import axios from 'axios'

//todo, if making static local page, change to const url = '/service/wordCheck'
const url = 'http://localhost:3002/service/wordCheck'

//TODO figure out websockets, it seems better for chat stuff



//Will do basic post as a work around, which should communicate with server to verify against the 'Dictionary'
const checkWord = (word) => {
	
	const request = axios.post(url, word)
	
	return request.then(response => response.data)
}

const wordCheckerService = {checkWord}

export default wordCheckerService