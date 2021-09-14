#include "Tries.h"

using namespace std;





/****************************** PRIVATE **********************************************************************************************/







/*   Hold off on this, sending an ifstream may be redundant
bool Tries::processDictionary(const ifstream& infile)
{
	return false;
}
*/


/**
	Main helper function for reading a file with a dictionary of words and store in current Tries
	For now, file is a .txt file, and words are seperated by newline or some blankspace delimitor

	TODO Later, implement a more structurted format for file (and maybe custom file extension) for extra features, like definitions and such
	Start with one word per line again, followed by deliminators for definition, then something else, etc
*/
bool Tries::processDictionary()
{
	string inputLine; //For reading in one line at a time. For now, it's just the word. Later, may implement other info on the word seperated by a deliminator, such as definitions and such. All contained on current line
	string word;
	//string definition

	while (getline(infile, inputLine)) //NOTE: this getline is part of std library, NOT ifstream.
	{
		//TODO Provide parsing by delimitor when more features of tries added
		//ie definition:  getLine(stringstream, definition)



		stringstream inputLineStream(inputLine); //Turns inputline into a stream object, which allows to do getLine again for parsing tokens
		
		//use getLine(inputLineStream, someVariable, ' ') each time a token is needed. Later, use different delimitor since one filed could be 'definition', which could be whole paragraphs with spaces
		getline(inputLineStream, word, ' ');
		//getline(inputLineStream, definition, ' ');




		insertWord(word);
		//insertWord(word, definition, ....);
		

	}

	//TODO For now just return True. Shouldn't be errors since inserting words only returns false if already a word. This function only returns false on ERRORS
	//In this case, should be formatting error or something
	return true;
}







/**
	Helper function for printing words. Will make use of Mutex printMutex to handle multi-threading when printing out
	@param outputStream A given ouput point where the given word is printed to, either a file (outfile) or the console
	@param targetWord The word to be printed to the outputStream. Word will be derived from parallel threads calling this function
*/



/*  Hold off...
void Tries::printWordOut(ostream outputStream, string targetWord)
{
	printMutex.lock();
	outputStream << targetWord;
	printMutex.unlock();
}


*/










/******************************			Public		 **********************************************************************************************/








/*******     Cosntructors	********/


Tries::Tries()
{
	root.reset(new TriesNode());
	isUpdated = false;
	workingFilePath = "";
}

Tries::Tries(string filePath)
{
	isUpdated = false;
	workingFilePath = "";

	root.reset(new TriesNode());


	bool isOpen;
	infile.open(filePath);
	isOpen = infile.is_open();
	if (!isOpen)
	{
		cout << "ERROR Cannot open file path, creating empty Tries";
		
	}
	else
	{

		bool successfulLoad = processDictionary(); //TODO in future, implement return boolean which would idicate errors in parsing/format, which would require either creating an empty Tries, or quiting

		if (successfulLoad)
		{
				workingFilePath = filePath;
		}
		else
		{
			//Some sort of issue with the loading file, keep what's loaded but don't assign to the given file
			cout << "WARNING! Input dictionary did not load correctly, this Tries structure will not reference the given file...\n";
		}

		infile.close();

	}



}









/*******     Load/Save	********/



bool Tries::loadDictionary(string filePath)
{

	//TODO Check if a file is already open

	if (infile.is_open())
		infile.close();
		

	bool isOpen;
	infile.open(filePath);
	isOpen = infile.is_open();
	if (!isOpen)
	{
		cout << "ERROR Cannot open file path, creating empty Tries";
		return false;

	}
	else
	{
		
		bool successfulLoad = processDictionary(); //TODO in future, implement return boolean which would idicate errors in parsing/format, which would require either creating an empty Tries, or quiting

		if (successfulLoad)
		{
			if (workingFilePath == "")
			{
				workingFilePath = filePath;
			}
			else
				isUpdated = true;
			
		}
		else
		{
			if (workingFilePath == "")
			{
				//Do nothing, if there was some issue when loading, best to not have possiblity to save to file that was source of issue 
			}
			else
				isUpdated = true;
		}

		infile.close();
	}


	return true;
}






/*
	Default save to file, using workingFilePath

	Creates a PrintFunctor with buffer to outFile, the calls root node's printWord(printFunctor, currentWord = ""),
	which recursively calls functor to print currentWord + currentChar (if isWord), the creates a group of threads to recursively
	do same thing for each mapped nect letter.

*/

bool Tries::saveDictionary()
{
	

	//Check if there is currently a workingFilePath
	if (workingFilePath == "")
	{

		//TODO Ideally, prompt user (typically in GUI with OS save file feature) for name of file and location to save. For now,
		//COUT to prompt user for name, and save to cwd (Won't error check validity of name for now...)

		cout << "WARNING! No active working file path. Please provide a valid file name... \n";

		string tempFilePathName;
		getline(cin, tempFilePathName);
		tempFilePathName += ".txt";

		workingFilePath = tempFilePathName;
	}



	bool isOpen;
	isOpen = outfile.is_open();

	if (isOpen)
	{
		cout << "WARNING! Outfile already opened, closing to write out dictionary for current workingFilePath \n";
		outfile.close();
	}


	outfile.open(workingFilePath);
	isOpen = outfile.is_open();
	if (!isOpen)
	{
		cout << "ERROR Issue opening file for writing out, exiting out...\n";
		return false;

	}
	else
	{
		PrintFunctor* printOut = new PrintFunctor(outfile);

		root->printWord(*printOut);


		//NOTE: Because TriesNode::printWord() calls recursive threads and joins with the child threads, the root call will waith until children are finished, thus freeing printOut and allowing the program to close the outfile

		delete printOut;


		outfile.close();

		return true;
	}

	
}


bool Tries::saveDictionaryAs(string fileName)
{
	workingFilePath = fileName;

	bool isOpen;
	isOpen = outfile.is_open();

	if (isOpen)
	{
		cout << "WARNING! Outfile already opened, closing to write out dictionary for current workingFilePath \n";
		outfile.close();
	}

	outfile.open(workingFilePath);
	isOpen = outfile.is_open();

	if (!isOpen)
	{
		cout << "ERROR Cannot open file for writing out, exiting out...\n";
		return false;

	}
	else
	{
		PrintFunctor* printOut = new PrintFunctor(outfile);

		root->printWord(*printOut);


		//NOTE: Because TriesNode::printWord() calls recursive threads and joins with the child threads, the root call will waith until children are finished, thus freeing printOut and allowing the program to close the outfile

		delete printOut;


		outfile.close();

		return true;
	}

	


}


//TODO Hold off on this, may need to implement a better file system library for actually dealing with paths and filenames...

bool Tries::saveDictionaryAs(string fileName, string filePath)
{
	cout << "WARNING!! saveDictionaryAs() with a path structure not implemented yet. (Try using only filename inntead...)\n";
	return false;
}






//Prints to console, using parallel threads to output (Does not guarntee order)
bool Tries::printDictionary()
{
	PrintFunctor* printOut = new PrintFunctor(cout);

	root->printWord(*printOut);


	//NOTE: Because TriesNode::printWord() calls recursive threads and joins with the child threads, the root call will waith until children are finished, thus freeing printOut and allowing the program to close the outfile

	delete printOut;

	return true;
}






//Because the words need to be in alphabetical order, the use of parrallel threads is tricky
//Rather than have each word itself call on the functor PrintFunctor, the program will have each branch in the Tries
//return a linked list, consisting of:
//the current word (if is word), a list of all words of the first child, a list of all words of second....etc
//The parallelism comes from having each child call be it's own thread (or in this case, a future async call), thus having the children work
//simultaneously to build a list, then merge them in order with the calling root.
bool Tries::printInOrderDictionary()
{
	list<string>* listAllWordsOrdered = root->getAllWords("");
	for (auto iter : *listAllWordsOrdered)
	{
		cout << iter << "\n";
	}

	delete listAllWordsOrdered;
	return true;
}











/*******     Word Processing	********/




bool Tries::insertWord(string newWord)
{
	//TODO later implementation, may want to switch to returning a pointer to a TriesNode where additional info can be added for that word, or make use of inserting extensions of word
	bool insertResult = root->insertWord(newWord);

	//Place any additional implementation here, such as updating a word count

	return insertResult;
}

bool Tries::deleteWord(string targetWord)
{
	return root->deleteWord(targetWord);
}

TriesNode* Tries::findWord(string targetWord)
{
	return root->findWord(targetWord);
}

bool Tries::isWord(string targetWord)
{
	TriesNode* isWordResult = root->findWord(targetWord);
	return (isWordResult != nullptr) ? true : false;
}
