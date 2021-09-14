/**
@date 7/30/2021

TriesNode.h

Header file for the class that will be the node for a Tries data structure. Used for identifiying words from a dictionary



*/




#pragma once

#include <map>
#include <string>
#include <vector>
#include <list>

#include <memory> //For smart pointers, something new

#include "PrintFunctor.h" //for printing to stream
#include <thread> //for threaded process of printing children
#include <future> //For async creation of list of child words in order

using namespace std;

class TriesNode
{
private:
	bool isWord;
	char currentChar;
	//map<char, TriesNode*> nextChar;
	map<char, unique_ptr<TriesNode>> nextChar; //Trying out SmartPointer unique_ptr, to handle emory and such in map


	//TODO determine if need a TriesNode* parent, which refeneces a parent node, which would be used for back-propagation
	TriesNode* parent; //reference to parent node, for back-propagation


	void pruneBranch(TriesNode* targetNode, char targetChar); //Helper method for deleting a word


public:
	TriesNode();
	TriesNode(string targetWord, TriesNode* newParent);
	TriesNode(string targetWord, int posInString, TriesNode* newParent); //extension of above constructor, except iterating through characters

	~TriesNode();


	bool isAWord();
	char getCurrentChar();


	//TODO consider returning a TriesNode instead, so that any further update an implmentation can be made (ie add definitions, add extended words that make use of this one as a stem, etc)
	bool insertWord(string targetWord);
	bool insertWord(string targetWord, int posInString);


	TriesNode* findWord(string targetWord); //Parent function of findWord(), implicitly starts search from beginning of targetWrod
	TriesNode* findWord(string targetWord, int posInString); //Different from isWord. Will return the node (if any) of a valid word. This allows for getting info on the word (ie. definition, synoyms, etc) or editing like delete.
	bool deleteWord(string targetWord);

	//Print function, which will implement threads to recursively print children
	void printWord(PrintFunctor& printOutput, string currentWord = "");

	list<string>* getAllWords(string currentWord);
};

