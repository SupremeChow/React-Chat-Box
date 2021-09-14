#include "TriesNode.h"




/*
	Helper function for removing branches of unused words

		Process:
			pre-condition: should start at node where word was found and set to not a word. Target Char is NULL, which should mean there are no child char to delete

			first: remove the targetChar from the current Node's nextChar
				in the case of the target being NULL (a word terminal, meaning at end of word and not targeting a char, called at initial pruning), do nothing
				other cases, remove from nextChar. targetChar should exit in nextChar since the previously deleted node had already existed and should have been in that map already. This is just clearing the parent node's records

			Second: Check if nextChar is now empty()
				if so:
					if the current Node's char is NULL (Should only be Root), do nothing and return, at end of recursive call
					otherwise, copy current Node's char and parent pointer, delete it, then recursively call pruneBranch (this is an independent leaf branch, so now it is pruned, and we need to have the parent clear record and possibly prune self)
				if NOT:
					do nothing and return, after clearing record in NextChar, there is still a dependency of another possible word from this branch, so end

*/
void TriesNode::pruneBranch(TriesNode* targetNode, char targetChar)
{
	if (targetChar != NULL)
		targetNode->nextChar.erase(targetChar);

	if (targetNode->nextChar.empty() && targetNode->currentChar != NULL)
	{
		char newTargetChar = targetNode->currentChar;
		TriesNode* newParent = targetNode->parent;




		//NOTE Below may be causing issues which may be related to smart pointer. It seems that calling the above map.erase(), while normally removing entry only...
		//Actually does the extra step of deleting since it is a smart pointer. As a result calling delete here, and then having a parent node call erase()
		//inadvertibly makes the smart pointer do a second delete, which throws an error. Guessing this is the built in memory handling that smart pointers are known for,
		//so now need to adjust (by commenting out) to not interfere...

		//delete targetNode; //Should be a leaf branch with no dependencies, so remove




		//recursively call prune at the parent

		pruneBranch(newParent, newTargetChar);
	}
	

}




TriesNode::TriesNode()
{
	currentChar = NULL;
	isWord = false;
	parent = nullptr;
}

TriesNode::TriesNode(string targetWord, TriesNode* newParent) : TriesNode(targetWord, 0, newParent)
{
}


TriesNode::TriesNode(string targetWord, int posInString, TriesNode* newParent)
{
	parent = newParent;
	currentChar = targetWord[posInString];
	isWord = (posInString == targetWord.size() - 1) ? true : false;

	posInString++;
	if (posInString < targetWord.size())
		nextChar.insert( pair<char, unique_ptr<TriesNode>>(targetWord[posInString], make_unique<TriesNode>(targetWord, posInString, this)) );

}

//Delete assumes in good faith that removing this node will not orphan any children or disrupt the Tries. It is simply used to delete the current Node. It is up to functions
//like removeWord() to handle tree like removal of nodes.
TriesNode::~TriesNode()
{
	//TODO check if have do do anything, since using smartPointer for nextChar map, should handle deletion itself if needed
}

bool TriesNode::isAWord()
{
	return isWord;
}


/*

//TODO
// Place here any additional getters for info on words and such, like definitions, info, maybe even a list of potential words from this point (ie if the current word is a prefix "con",
// return list of all child words that can be made. Additionally maybe first n words, or top n popular words, etc)

*/


char TriesNode::getCurrentChar()
{
	return currentChar;
}


//Parent method of insertWord(), inplicitly inserts a word starting at position 0 of word.
bool TriesNode::insertWord(string targetWord)
{
	return insertWord(targetWord, 0);
}



/**
	Recursively call insertWord(), until base case of end of string, or word is already inserted.
	As process procedes, checks if characters are alredy present, and if not, creates new TriesNodes.

	@return boolean: Condition of whether the insertion was needed (False if the word was already inserted, True if word needed to be created or assigned as aWord)
*/
bool TriesNode::insertWord(string targetWord, int posInString)
{
	//base case
	if (posInString == targetWord.size())
	{
		if (isWord == true)
			return false; //Word already exists, return false to say so (may be redundant, but we'll leave for now just in case relevant)
		else
		{
			isWord = true;
			return true;
		}
	}

	//Recursive case, going through each letter

	//if no next characters, or the nextCharacter doesn't exit in current map, start creating new nodes, using constructor
	if (nextChar.empty() || nextChar.count(targetWord[posInString]) == 0)
	{
		nextChar.insert(pair<char, unique_ptr<TriesNode>>(targetWord[posInString], make_unique<TriesNode>(targetWord, posInString, this)));
		return true;
	}
	//Otherwise, increment posInString, and move to next character that the node has
	else if (nextChar.count(targetWord[posInString]) > 0)
	{
		//posInString++;
		return nextChar[targetWord[posInString]]->insertWord(targetWord, posInString + 1);

	}


}


TriesNode* TriesNode::findWord(string targetWord)
{
	return findWord(targetWord, 0);
}






/**
	Recursively call findWord(), trying to find terminal point where at the end of targetWord, and current TriesNode's isWord == true.
	If at the end of targetWord and TriesNode's isWord != true, or not at the end of targetWord and there are no more TriesNodes to search, returns
	a nullptr indicating that the word doesn't exist (useful for deleteWord(), preventing deletion of words that don't exist)

	@return TriesNode*: Pointer to the TriesNode indicating corresponding to the given word. Returns nullptr if word is invalid or doesn't exist
*/
TriesNode* TriesNode::findWord(string targetWord, int posInString)
{


	//REDO
	//Terminal Case, end of targetWord, and current TriesNode is a word
	if ((posInString == targetWord.size() && isWord == true))
		return this;

	//At end of target word and not considered a word, or no more characters to go to in our Tries' map, return nullptr
	else if ((posInString == targetWord.size() && isWord == false) || (posInString < targetWord.size()) && (nextChar.empty() || nextChar.count(targetWord[posInString]) == 0))
		return nullptr;

	//Otherwise, itterate posInString, and recrusively call findWord in the next TriesNode
	else
		return nextChar[targetWord[posInString]]->findWord(targetWord, posInString+1);


	/*
	//Terminal Case, ned of targetWord, and current TriesNode is a word
	if ((posInString == targetWord.size() - 1 && isWord == true))
		return this;

	//At end of target word and not considered a word, or no more characters to go to in our Tries' map, return nullptr
	else if( (posInString == targetWord.size()-1 && isWord == false) || (posInString < targetWord.size() - 1) && (nextChar.empty() || nextChar.count(targetWord[posInString]) == 0 ) )
		return nullptr;
	//Otherwise, itterate posInString, and recrusively call findWord in the next TriesNode
	else 
		return nextChar[targetWord[posInString]]->findWord(targetWord, ++posInString);

	*/
}


//TODO May need to implement parent pointer for TriesNode so that when using findWord(), can work upwards and delete nodes if necessary
bool TriesNode::deleteWord(string targetWord)
{
	auto foundWord = findWord(targetWord);
	if(foundWord == nullptr)
		return false;

	foundWord->isWord = false;

	//if word is valid, and node is a leaf, call helper function pruneBranch() to recursively remove un-used nodes
	pruneBranch(foundWord, NULL); //TODO, check if okay for char should be NULL, indicating end of string

	return true;


}



void TriesNode::printWord(PrintFunctor& printOutput, string currentWord)
{
	if (currentChar != NULL) //don't add if root
	{
		currentWord += currentChar;
	}


	//TODO decide if print first, then thread children, or opposite. Printing first could make this thread wait, thus delay threading
	//However, calling children thread first may start parralellism, but now children may increase potential wait (also, if ordered mattered, could potentially print really out of order)
	if (isWord)
	{
		//
		(printOutput)(currentWord);
	}

	//if child words, create threads of each nextChar to recursively call printWord
	if (nextChar.size() != 0)
	{
		vector<thread> childThreads;




		for (map<char, unique_ptr<TriesNode>>::iterator iter = nextChar.begin(); iter != nextChar.end(); iter++)
		{
			childThreads.push_back(thread(&TriesNode::printWord,iter->second.get(), ref(printOutput), currentWord));
			
		}

		//Join threads
		//NOTE, may cause long hang ups with this thread which may take up clock time. However, detatching threads will free this parent thread, but there is no gauruentee or
		//way of knowing when this recursive call is complete. It's possible that a significantly large tire could be continuing to run in the background and not know when we try to do other things

		for (int i = 0; i < childThreads.size(); i++)
		{
			childThreads[i].join();
		}
	}
	
	



}



//From the calling node, returns a list of all possible words(*) made of the root node and the combination of children
//Makes use of parallell calls to children so that children list can be made simultaneously, then linked together at the end
//*Note, because this function can be initially called from Non-root nodes, the resulting words may actually be sans-prefix.
//i.e. words like preamp, prenup, pretend,..., where the getAllWords is called at the node AFTER "pre" will result in
//amp, nup, tend ...etc. 
list<string>* TriesNode::getAllWords(string currentWord)
{
	list<string>* returnList = new list<string>();
	if (currentChar != NULL)
		currentWord += currentChar;

	if (isWord)
		returnList->push_back(currentWord);
	if (nextChar.size() != 0)
	{
		vector<future<list<string>*>> childLists;
		for (map<char, unique_ptr<TriesNode>>::iterator iter = nextChar.begin(); iter != nextChar.end(); iter++)
		{
			//For each child of current Node, make them asyncrously make their own list of words
			childLists.push_back( async(&TriesNode::getAllWords, iter->second.get(), currentWord) );
		}

		//Merge child lists back into returnList

		for (int i = 0; i < childLists.size(); i++)
		{



			//                                         VVVVVVVVVVVVVV
			returnList->splice(returnList->end(), *(childLists[i].get()) );
		}

	}


	return returnList;
}
