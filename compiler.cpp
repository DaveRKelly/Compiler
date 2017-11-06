/*
 * 
 * Duquesne Univeristy
 * System Programming and Design
 * Fall 2017
 * Dr. Adam Drozdek
 * 
 * Author: David Kelly
 *
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <functional>
#include <regex>
#include <execinfo.h>
using namespace std;

class Token {
	
private: 
	string name;
	int type, value, size, elType;

public:
	Token(string name);
	Token(string name, int type, int value);
	Token(string name, int type, int value, int size, int elType);

	void setHashValue(int hashValue);
	void setType(int type);
	void setValue(int value);
	void setSize(int size);
	void setElType(int elType);

	string getName();
	int getHashValue();
	int getType();
	int getValue();
	int getSize();
	int getElType();

};

Token::Token(string name) {
	this->name = name;
	this->type = 0;
	this->value = 0;
	size = 0;
	elType = 0;
}

Token::Token(string name, int type, int value) {
	this->name = name;
	this->type = type;
	this->value = value;
	size = 0;
	elType = 0;
}

Token::Token(string name, int type, int value, int size, int elType) {
	this->name = name;
	this->type = type;
	this->value = value;
	this->size = size;
	this->elType = elType;
}

void Token::setType(int type) { this->type = type; }

void Token::setValue(int value) { this->value = value; }

void Token::setSize(int size) { this->size = size; }

void Token::setElType(int elType) { this->elType = elType; }

string Token::getName() { return this->name; }

int Token::getType() { return this->type; }

int Token::getValue() { return this->value; }

int Token::getSize() { return this->size; }

int Token::getElType() { return this->elType; }

//TYPE
const int INTEGER = 1001;
const int INTEGER_CONSTANT = 1002;
const int INTEGER_VARIABLE = 1003;
const int INTEGER_CONSTANT_ARRAY = 1004;
const int INTEGER_VARIABLE_ARRAY = 1005;

const int CHARACTER = 1006;
const int CHARACTER_CONSTANT = 1007;
const int CHARACTER_VARIABLE = 1008;
const int CHARACTER_CONSTANT_ARRAY = 1009;
const int CHARACTER_VARIABLE_ARRAY = 1010;

const int RESERVED_WORD = 1011;
const int OPERATOR = 1012;
const int PUNCTUATION = 1013;
//const int RELATIONAL_OPERATOR = 1014;
const int ID = 1000;
const int STRING = 2010;

//VALUE
const int NOT_EQUAL_TO = 2000;
const int LESS_THAN_OR_EQUAL_TO = 2001;
const int GREATER_THAN_OR_EQUAL_TO = 2002;
const int EQUAL_TO = 2003;
const int OR = 2004;
const int AND = 2005;
const int OUT = 2006;
const int IN = 2007;
const int INCREMENT = 2008;
const int DECREMENT = 2009;

const int CONSTANT = 3000;
const int IF = 3001;
const int ELSE = 3002;
const int WHILE = 3003;
const int FOR = 3004;
const int Void = 3005;
const int MAIN = 3006;
const int INCLUDE = 3007;
const int IOSTREAM = 3008;
const int ENDL = 3009;
const int CIN = 3010;
const int COUT = 3011;
const int GOTO = 3012;
const int RETURN = 3013;
const int USING = 3014;
const int NAMESPACE = 3015;
const int STD = 3019;
const int Int = 3020;
const int Char = 3021;

int tmpNum = 0;
int totalTmp = 0; //Total number of temporary variables
string arg1; //String used to print out the quadruple -- saves the content
int LNum = 0;
int STs=0, Hs=0;

typedef unordered_map<string, vector<Token> > hashmap;
std::vector<std::vector<Token>*> identify;
std::vector<std::vector<std::vector<Token> > > history;

int strCount = 0;

hashmap st;

string findName(int *type, int *value);

void exp(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, 
	string *result, ofstream *inter);

void term(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, 
	string *result, ofstream *inter);

void factor(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, 
	string *result, ofstream *inter);

void ioAssign(int *ioT, int *ioV, bool output, ifstream *fIn, hashmap *SymbolTable, 
	ofstream *inter);

void condition(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *falsel, string *skipL, ofstream *inter);

void WhileLoop(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable,
	ofstream *inter);

void IfSeries(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	ofstream *inter);

void contentBlock(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, 
	string *result, ofstream *inter);

void semiConditionPrep(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter);

void RelOp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, int RoP, string *save, ofstream *inter);

void orExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter);

void andExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter);

void notExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter);

void hashID(hashmap *SymbolTable, string *name, int *type, int *value) {
	hashmap::const_iterator found = SymbolTable->find(*name);
	if (found == SymbolTable->end()) { //Does not exist in symbol table -- add its ID
		vector<Token> newRow;
		pair<string, vector<Token> > newP(*name, newRow); // Pair: the key, a new vector row
		SymbolTable->insert(newP);
		*type = identify.size();
		*value = SymbolTable->at(*name).size();
		Token newToken = Token(*name, *type, *value);
		SymbolTable->at(*name).push_back(newToken);
		identify.push_back(&(SymbolTable->at(*name)));
	} else { //Exists in the symbol table -- chain collisions
		if (SymbolTable->at(*name).empty()) {
			*type = RESERVED_WORD;
			if (name->compare("const") == 0)
				*value = CONSTANT;
			else if (name->compare("if") == 0)
				*value = IF;
			else if (name->compare("else") == 0)
				*value = ELSE;
			else if (name->compare("while") == 0)
				*value = WHILE;
			else if (name->compare("for") == 0)
				*value = FOR;
			else if (name->compare("void") == 0)
				*value = Void;
			else if (name->compare("main") == 0)
				*value = MAIN;
			else if (name->compare("include") == 0)
				*value = INCLUDE;
			else if (name->compare("iostream") == 0)
				*value = IOSTREAM;
			else if (name->compare("endl") == 0)
				*value = ENDL;
			else if (name->compare("cin") == 0)
				*value = CIN;
			else if (name->compare("cout") == 0)
				*value = COUT;
			else if (name->compare("goto") == 0)
				*value = GOTO;
			else if (name->compare("return") == 0)
				*value = RETURN;
			else if (name->compare("using") == 0)
				*value = USING;
			else if (name->compare("namespace") == 0)
				*value = NAMESPACE;
			else if (name->compare("std") == 0)
				*value = STD;
			else if (name->compare("int") == 0)
				*value = Int;
			else if (name->compare("char") == 0)
				*value = Char;
			else
				*value = ID;
			Token newToken = Token(*name, RESERVED_WORD, *value);
			SymbolTable->at(*name).push_back(newToken);
		} else {
			bool exists = false;
			int index = 0;
			for (int i = 0; i < SymbolTable->at(*name).size() && exists == false; i++) {
				if (name->compare(SymbolTable->at(*name).at(i).getName()) == 0) {
					exists = true;
					index = i;
				}
			}
			if (!exists) {
				if (SymbolTable->at(*name).at(0).getType() == RESERVED_WORD) {
					*type = RESERVED_WORD;
					if (name->compare("const") == 0)
						*value = CONSTANT;
					else if (name->compare("if") == 0)
						*value = IF;
					else if (name->compare("else") == 0)
						*value = ELSE;
					else if (name->compare("while") == 0)
						*value = WHILE;
					else if (name->compare("for") == 0)
						*value = FOR;
					else if (name->compare("void") == 0)
						*value = Void;
					else if (name->compare("main") == 0)
						*value = MAIN;
					else if (name->compare("include") == 0)
						*value = INCLUDE;
					else if (name->compare("iostream") == 0)
						*value = IOSTREAM;
					else if (name->compare("endl") == 0)
						*value = ENDL;
					else if (name->compare("cin") == 0)
						*value = CIN;
					else if (name->compare("cout") == 0)
						*value = COUT;
					else if (name->compare("goto") == 0)
						*value = GOTO;
					else if (name->compare("return") == 0)
						*value = RETURN;
					else if (name->compare("using") == 0)
						*value = USING;
					else if (name->compare("namespace") == 0)
						*value = NAMESPACE;
					else if (name->compare("std") == 0)
						*value = STD;
					else if (name->compare("int") == 0)
						*value = Int;
					else if (name->compare("char") == 0)
						*value = Char;
					else
						*value = ID;
					Token newToken = Token(*name, RESERVED_WORD, *value);
					SymbolTable->at(*name).push_back(newToken);
				} else { 
					*type = identify.size();
					*value = SymbolTable->at(*name).size();
					Token newToken = Token(*name, *type, *value);
					SymbolTable->at(*name).push_back(newToken);
					identify.push_back(&(SymbolTable->at(*name)));
				}
			} else { //It exists -- do not store -- however, change the value and type for output
				if (SymbolTable->at(*name).at(0).getType() == RESERVED_WORD) {
					*type = RESERVED_WORD;
					if (name->compare("const") == 0)
						*value = CONSTANT;
					else if (name->compare("if") == 0)
						*value = IF;
					else if (name->compare("else") == 0)
						*value = ELSE;
					else if (name->compare("while") == 0)
						*value = WHILE;
					else if (name->compare("for") == 0)
						*value = FOR;
					else if (name->compare("void") == 0)
						*value = Void;
					else if (name->compare("main") == 0)
						*value = MAIN;
					else if (name->compare("include") == 0)
						*value = INCLUDE;
					else if (name->compare("iostream") == 0)
						*value = IOSTREAM;
					else if (name->compare("endl") == 0)
						*value = ENDL;
					else if (name->compare("cin") == 0)
						*value = CIN;
					else if (name->compare("cout") == 0)
						*value = COUT;
					else if (name->compare("goto") == 0)
						*value = GOTO;
					else if (name->compare("return") == 0)
						*value = RETURN;
					else if (name->compare("using") == 0)
						*value = USING;
					else if (name->compare("namespace") == 0)
						*value = NAMESPACE;
					else if (name->compare("std") == 0)
						*value = STD;
					else if (name->compare("int") == 0)
						*value = Int;
					else if (name->compare("char") == 0)
						*value = Char;
					else
						*value = ID;
				} else { 
					*type = SymbolTable->at(*name).at(index).getType();
					*value = SymbolTable->at(*name).at(index).getValue();
				}
			}
		}
	}
}

void hashString(hashmap *SymbolTable, string *name, int *type, int *value) {
	hashmap::const_iterator found = SymbolTable->find(*name);
	*type = STRING;
	strCount++;
	if (found == SymbolTable->end()) { //Does not exist in symbol table -- add its ID
		vector<Token> newRow;
		*value = strCount;
		Token newToken = Token(*name, *type, *value);
		pair<string, vector<Token> > newP(*name, newRow); //Pair: the key, a new vector row
		SymbolTable->insert(newP);
		SymbolTable->at(*name).push_back(newToken);
	} else { //Exists in the symbol table -- chain for collisions
		bool exists = false;
		for (int i = 0; i < SymbolTable->at(*name).size() && exists == false; i++) {
			if (name->compare(SymbolTable->at(*name).at(i).getName()) == 0)
				exists = true;
		}
		if (!exists) {
			*value = strCount;
			Token newToken = Token(*name, *type, *value);
			SymbolTable->at(*name).push_back(newToken);
		}
	}
}

/*
 *
 * Saves reserved word list to symbol table via file
 * File must be in the same directory as the lexical analyzer
 * Stores the reserved words as one per line.
 * 
 */
void storeKnown(hashmap *SymbolTable, string *name) {
	ifstream reservedWordList("reservedWords.txt");
	if (reservedWordList.is_open()) {
		while (reservedWordList.good()) {
			getline(reservedWordList, *name);
			hashmap::const_iterator found = SymbolTable->find(*name);
			if (found == SymbolTable->end()) { //Does not exist in symbol table
				vector<Token> newRow;
				pair<string, vector<Token> > newP(*name, newRow); //Pair: Key, a new vector row
				SymbolTable->insert(newP);
			} else { //Exists in the symbol table - chain for collisions
				bool exists = false;
				for (int i = 0; i < SymbolTable->at(*name).size() && exists == false; i++) {
					if (name->compare(SymbolTable->at(*name).at(i).getName()) == 0)
						exists = true;
				}
				if (!exists) {
					Token newToken = Token(*name);
					SymbolTable->at(*name).push_back(newToken);
				}
			}
		}
	} else {
		throw 404;
	}
	reservedWordList.close();
}

bool isAlpha(char c) {
	return ('a' <= (int) c && (int) c <= 'z') || ('A' <= (int) c && (int) c <= 'Z');
}

bool isNum(char c) {
	return ('0' <= (int) c && (int) c <= '9');
}

bool isAlphaNum(char c){
	return isAlpha(c) || isNum(c);
}

/**
 * 
 * Before reading the file, create a hash function and hash the reserved words into the
 * symbol table. Check the symbol table when looking at a sequence of letters, digits,
 * and underscores. Retrieve necessary information to generate a token corresponding
 * to the sequence of characters.
 * 
 */
void lexicalAnalyzer(hashmap *SymbolTable) {
	string name;
	ofstream lex("lex.txt", ofstream::trunc);

	storeKnown(SymbolTable, &name); //Hash all of the reserved words into the Symbol Table.
	try {
		ifstream readFile("src.txt"); //This should be *.cpp but *.txt will work
		string line;
		int value = 0, type = 0, count = 0;
		bool doubleQuote = false, singleQuote = false, comment = false, id = false, first = true;
		if (!name.empty())
			name.clear();
		if (readFile.is_open()) {
			hashmap::const_iterator found;
			while (readFile.good()) {
				getline(readFile, line);
				for (int i = 0; i < line.size(); i++) { 
					//Comment block
					if (i < line.size() - 1 && line.at(i) == '/' && line.at(i + 1) == '*' && !comment && i != line.size() - 1) {
						comment = true;
						i++;
					} else if (i < line.size() - 1 && line.at(i) == '*' && line.at(i + 1) == '/' && comment && i != line.size() - 1) {
						comment = false;
						i++;
					} //End of comment block
					else if (!comment) { 
						if ((line.at(i) == '$' || line.at(i) == '@' || line.at(i) == '^' || line.at(i) == '`' || line.at(i) == '~') && !singleQuote && !doubleQuote) {
							throw 8080; //Illegal characters
						}
						//ID
					if ((isAlpha(line.at(i)) || (line.at(i) == '_')) && (!id && !singleQuote && !doubleQuote) && (i < line.size() - 1)) { //Initial start of ID
						id = true;
						name.push_back(line.at(i));
						//cout << "Start of ID";
				} else if ((isAlphaNum(line.at(i)) || (line.at(i) == '_')) && (id && !singleQuote && !doubleQuote) && (i < line.size() - 1)) { //The rest of the ID
					name.push_back(line.at(i));
							//cout << "Middle of ID";
				} else if (((i == line.size() - 1) && (!id && !singleQuote && !doubleQuote) && (isAlpha(line.at(i)) || (line.at(i) == '_'))) || ((id == true && singleQuote == false && doubleQuote == false) && (i == line.size() - 1) && (isAlphaNum(line.at(i)) || (line.at(i) == '_')))) {
					name.push_back(line.at(i));
					hashID(SymbolTable, &name, &type, &value);
					if (first){
						lex << left << setw(5) << type << value;
						first = false;
								break; //End of line and end of loop.
							} else {
								lex << endl << left << setw(5) << type << value;
							}
							name.clear();
							id = false;
						} else if ((id && !singleQuote && !doubleQuote) && !(isAlphaNum(line.at(i)) || (line.at(i) == '_'))) {
							hashID(SymbolTable, &name, &type, &value);
							if (first){
								lex << left << setw(5) << type << value;
								first = false;
							} else {
								lex << endl << left << setw(5) << type << value;
							}
							name.clear();
							id = false;
						} //End of ID
						//Strings
						if (line.at(i) == '\"' && !doubleQuote && i != line.size() - 1) {
							doubleQuote = true;
						} else if (doubleQuote && line.at(i) != '\"' && i != line.size() - 1) {
							name.push_back(line.at(i));
						} else if (doubleQuote && line.at(i) == '\"') {
							hashString(SymbolTable, &name, &type, &value);
							if (first) {
								lex << left << setw(5) << type << value;
								first = false;
							} else {
								lex << endl << left << setw(5) << type << value;
							}
							name.clear();
							doubleQuote = false;
						} else if ((line.at(i) != '\"' && doubleQuote && i == line.size() - 1) || (line.at(i) == '\"' && !doubleQuote && i == line.size() - 1)) {
							throw 666;
							doubleQuote = false;
						} //End of Strings
						//Characters
						if (line.at(i) == '\'' && !singleQuote && i != line.size() - 1) {
							singleQuote = true;
						} else if (singleQuote && count < 1) {
							count++;
							name.push_back(line.at(i));
							value = line.at(i);
						} else if (singleQuote == true && count == 1 && line.at(i) == '\'') {
							if (first){
								lex << left << setw(5) << CHARACTER << value;
								first = false;
							} else {
								lex  << endl << left << setw(5) << CHARACTER << value;
							}
							name.clear();
							count = 0;
							singleQuote = false;
						} else if ((singleQuote && count >= 2) || (singleQuote && count == 1 && line.at(i) != '\'') || (line.at(i) == '\'' && !singleQuote && i == line.size() - 1)) {
							throw 667;
							singleQuote = false;
							count = 0;
						} //End of Characters
						//Operators
						if ((line.at(i) == '+' || line.at(i) == '-' || line.at(i) == '*' || line.at(i) == '/' || line.at(i) == '%' || line.at(i) == '=' || line.at(i) == '>' || line.at(i) == '<' || line.at(i) == '!' || line.at(i) == '&' || line.at(i) == '|') && !singleQuote && !doubleQuote) {
							if (i < (line.size() - 1) && line.at(i) == '+'
									&& line.at(i + 1) == '+') {	//Doubles
								name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = INCREMENT;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '-'
							&& line.at(i + 1) == '-') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = DECREMENT;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '='
							&& line.at(i + 1) == '=') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = EQUAL_TO;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '>'
							&& line.at(i + 1) == '=') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = GREATER_THAN_OR_EQUAL_TO;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '<'
							&& line.at(i + 1) == '=') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = LESS_THAN_OR_EQUAL_TO;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '!'
							&& line.at(i + 1) == '=') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = NOT_EQUAL_TO;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '&'
							&& line.at(i + 1) == '&') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = AND;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '|'
							&& line.at(i + 1) == '|') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = OR;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '<'
							&& line.at(i + 1) == '<') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = OUT;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '>'
							&& line.at(i + 1) == '>') {
							name.push_back(line.at(i));
							name.push_back(line.at(i + 1));
							value = IN;
							i++;
						} else if (i < (line.size() - 1) && line.at(i) == '/'
							&& line.at(i + 1) == '/') {	
							line.clear();
							break;
							} else { //Bitwise Operators
								if (line.at(i) == '&' || line.at(i) == '|') {
									throw 700; //Illegal characters when by themselves -- not in str or char
								} else { //Any other legal operator
									name.push_back(line.at(i));
									value = line.at(i);
								}
							}
							if (first) {
								lex << left << setw(5) << OPERATOR << value;
								first = false;
							} else {
								lex << endl << left << setw(5) << OPERATOR << value;
							}
							name.clear();
						} //End of Operators
						//PUNCTUATION & DELIMITERS
						if ((line.at(i) == ';' || line.at(i) == '(' || line.at(i) == ')' || line.at(i) == '[' || line.at(i) == ']' || line.at(i) == ',' || line.at(i) == '.' || line.at(i) == '{' || line.at(i) == '}' || line.at(i) == '#') && singleQuote == false && doubleQuote == false) {
							name.push_back(line.at(i));
							value = line.at(i);
							// }
							if (first) {
								lex << left << setw(5) << PUNCTUATION << value;
								first = false;
							} else {
								lex << endl << left << setw(5) << PUNCTUATION << value;
							}
							name.clear();
						} //End of Punctuation
						//INTEGERS
						if (isNum(line.at(i)) && !singleQuote && !doubleQuote && !id) {
							if (i < (line.size() - 1) && isNum(line.at(i+1))) {
								name.push_back(line.at(i));
								value = atoi(name.c_str());
							} else {
								name.push_back(line.at(i));
								value = atoi(name.c_str());
								if (first) {
									lex << left << setw(5) << INTEGER << value;
									first = false;
								} else {
									lex << endl << left << setw(5) << INTEGER << value;
								}
								name.clear();
							}
						} //End of Integers
					}
				}
			}
		} else {
			throw 404;
		}
		readFile.close();
		//If a comment block is left open, thraw an error -- no closing */
		if (comment == true) {
			throw 668;
		}
	} catch (int e) {
		if (e == 404)
			cout << "\nError " << e << ": Unable to open File.\n";
		else if (e == 666)
			cout << "\nError " << e << ": String not terminated.\n";
		else if (e == 667)
			cout << "\nError " << e << ": Character not terminated.\n";
		else if (e == 668)
			cout << "\nError " << e << ": Comment not terminated.\n";
		else
			cout << "\nError " << e << ": Illegal Character.\n";
	}
	lex.close();
}

void getLineTV(ifstream *readFile, string *line, string *type, string *value) {
	getline(*readFile, *line);
	if (!readFile->eof() && line->size() > 5) {
		*type = line->substr(0, (line->find_first_of(" ")));
		*value = line->substr(5);
	} else {
		*type = "-1";
		*value = "-1";
	}
}

void getLineTVi(ifstream *readFile, int *type, int *value) {
	string line;
	getline(*readFile, line);
	if (!readFile->eof() && line.size() > 5) {
		*type = atoi(line.substr(0, (line.find_first_of(" "))).c_str());
		*value = atoi(line.substr(5).c_str());
	} else {
		*type = -1;
		*value = -1;
	}
}

/* 
 * 
 * Parse the declarations of the source code provided via file. 
 * Modifies the existing symbol table.
 * 
 */
void parseDeclarations(hashmap *SymbolTable) {
	try {
		ifstream readFile("lex.txt"); //This should be *.cpp but *.txt will work
		string line, type, value;
		int t1, t2, v1, v2, v3, v4;
		bool begin = false, con = false, var = false, id = false, 
		noBracket =	true, arraySize = false, init = false, val = false;

		if (readFile.is_open()) {
			while (readFile.good()) {
				getLineTV(&readFile, &line, &type, &value);
				t1 = atoi(type.c_str());
				v1 = atoi(value.c_str());
				if (t1 != -1 && v1 != -1) {
					//cout << "type =" << type << " value =" << value << endl;
					if (!begin) { //Parse if declaration has not started
						if (t1 == RESERVED_WORD 
							&& (v1 == CONSTANT || v1 == Int || v1 == Char)) {
							if (v1 == CONSTANT) {
								con = true;
								begin = true;
							} else {
								begin = true;
								//Variable is first?
								if (v1 == Int || v1 == Char) { 
								//cout << "\nInt or Char v1 = \n" << v1 << endl;
									var = true;
									v3 = v1;
								} else { //If not, throw an error
									throw 706;
									break;
								}
							}
						}
					} //End begin
					//VAR
					else if (!var) { //Token is either reserved word (vartype) or ID
						if (t1 == RESERVED_WORD) {
							if (v1 == Int || v1 == Char) { 
							//cout << "\nInt or Char v1 = \n" << v1 << endl;
								var = true;
								v3 = v1;
							} else { 
								throw 706;
								break;
							}
						} else {
							throw 701;
							break;
						}
					} //End var
					// ID
					else if (!id) {	// Looking for ID.
						//cout << "\nv3 = " << v3 << endl;
						if (t1 < 1000) { 
							//IDs are the only ones that are below 1000.
							//When checking IDs, also check if they'vealready changed
							//If so, then it's a redeclaration error.
							if ((identify.at(t1)->at(v1).getType() == INTEGER_CONSTANT
								|| identify.at(t1)->at(v1).getType() == INTEGER_VARIABLE
								|| identify.at(t1)->at(v1).getType() == CHARACTER_CONSTANT
								|| identify.at(t1)->at(v1).getType() == CHARACTER_VARIABLE)) {
									//Check if already changed.
								throw 705;
							break;
						} else {
							if (con) {
								if (v3 == Int) {
									identify.at(t1)->at(v1).setType(INTEGER_CONSTANT);
									t2 = t1;
									v2 = v1;
									v4 = INTEGER_CONSTANT;
								} else if (v3 == Char) {
									identify.at(t1)->at(v1).setType(CHARACTER_CONSTANT);
									t2 = t1;
									v2 = v1;
									v4 = CHARACTER_CONSTANT;
								} else {
									throw 706;
									break;
								}
							} else {
								if (v3 == Int) {
									identify.at(t1)->at(v1).setType(INTEGER_VARIABLE);
									t2 = t1;
									v2 = v1;
									v4 = INTEGER_VARIABLE;
								} else if (v3 == Char) {
									identify.at(t1)->at(v1).setType(CHARACTER_VARIABLE);
									t2 = t1;
									v2 = v1;
									v4 = CHARACTER_VARIABLE;
								} else {
									throw 706;
									break;
								}
							}
								id = true; //Have to set the value after the op and during execution
							}
						} else if (t1 == RESERVED_WORD) { //The word after variable is a reserved word, but is it an acceptable reserved word?
							if (v1 == MAIN) {
								if (con == true) {
									throw 707;
									break;
								}
								break; //End the search for declarations -- making this only work on declarations outside of method.
								var = false;
								con = false;
								begin = false;
							} else {
								cout << "v1 = " << v1;
								throw 700;
								break;
							}
						} else {
							cout << "\nt1 = " << t1 << " v1 = " << v1;
							throw 702;
							break;
						}
					} //End ID
					// INIT
					else if (!init) { //Looking for semicolon ';', comma ',', or operator '='
						if (t1 == PUNCTUATION && v1 == ';') { //Ends the declaration
							if (con) {
								cout << "\n HERE A";
								throw 704;
								break;
							} else { //End of variable declaration. Reset all boolean variables
								begin = false;
								var = false;
								id = false;
							}
						} else if (t1 == OPERATOR && v1 == '=') {
							init = true;
						} else if (t1 == PUNCTUATION && v1 == ',') { //Commas
							if (con) {
								cout << "\n HERE B";
								throw 704;
								break;
							} else { //End of variable declaration. Reset all boolean variables
								id = false;	//ID should be next if a comma is present
							}
						} else if (t1 == PUNCTUATION && v1 == '[') { //Brackets are read in as: PUNCTUATION (1013) 91'[' and 93 ']'
						//cout << "\n Bracket begin \n";
						noBracket = false;
						init = true;
					} else {
						cout << "\n HERE C";
						throw 704;
						break;
					}
					} // End Init
					// noBracket and arraySize
					else if (!noBracket && !arraySize) { //if the id was an array and we are checking for arraySize
						if (t1 == INTEGER) {
							arraySize = true;
							//cout << "\nt2 = " << t2 << " v2 = "<< v2 << " name = " << identify.at(t2)->at(v2).getName();
							identify.at(t2)->at(v2).setSize(v1); //Set size to v1 (the value of integer)
							identify.at(t2)->at(v2).setElType(v3); //Set ElType to v3 (the variable type for current declaration)
							identify.at(t2)->at(v2).setValue(0); //Set value to 0
							if (v4 == INTEGER_CONSTANT) {
								identify.at(t2)->at(v2).setType(INTEGER_CONSTANT_ARRAY);
							} else if (v4 == INTEGER_VARIABLE) {
								identify.at(t2)->at(v2).setType(INTEGER_VARIABLE_ARRAY);
							} else if (v4 == CHARACTER_CONSTANT) {
								identify.at(t2)->at(v2).setType(CHARACTER_CONSTANT_ARRAY);
							} else if (v4 == CHARACTER_VARIABLE) {
								identify.at(t2)->at(v2).setType(CHARACTER_VARIABLE_ARRAY);
							} else {
								cout << "\nv3 = " << v3 << " t2 = " << t2 << " v2 = " << v2 
								<< " identify type = " << identify.at(t2)->at(v2).getType() << endl;
								throw 703;
								break;
							}
						}
						else if (t1 == PUNCTUATION && v1 == ']') {
							identify.at(t2)->at(v2).setSize(-1); //Set size to v1 (the value of integer)
							identify.at(t2)->at(v2).setElType(v3); //Set ElType to v3 (the variable type for current declaration)
							identify.at(t2)->at(v2).setValue(0); //Set value to 0
							if (v4 == INTEGER_CONSTANT) {
								identify.at(t2)->at(v2).setType(INTEGER_CONSTANT_ARRAY);
							} else if (v4 == INTEGER_VARIABLE) {
								identify.at(t2)->at(v2).setType(INTEGER_VARIABLE_ARRAY);
							} else if (v4 == CHARACTER_CONSTANT) {
								identify.at(t2)->at(v2).setType(CHARACTER_CONSTANT_ARRAY);
							} else if (v4 == CHARACTER_VARIABLE) {
								identify.at(t2)->at(v2).setType(CHARACTER_VARIABLE_ARRAY);
							} else {
								cout << "\nv3 = " << v3 << " t2 = " << t2
								<< " v2 = " << v2 << " identify type = "
								<< identify.at(t2)->at(v2).getType()
								<< endl;
								throw 703;
								break;
							}
							noBracket = true;
							arraySize = false;
						}
						else {
							cout << "\n HERE D";
							cout << "\nt1 = "<<t1 << "v1 = "<<v1<< endl;
							throw 704;
							break;
						}
					} //End arraySize
					//noBracket
					else if (!noBracket && arraySize) { //If ID was an array, we are checking for end BRACKET
						if (t1 == PUNCTUATION && v1 == ']') {
							noBracket = true;
							arraySize = false;
						} else {
							cout << " TEST ME \n";
							throw 704;
							break;
						}
					} //End Brackets
					//VAL
					else if (!val) { 
						if (t1 < 1000) { 
							val = true;
							identify.at(t2)->at(v2).setValue(
								identify.at(t1)->at(v1).getValue());
						} else if (t1 == INTEGER) { 
							val = true;
							identify.at(t2)->at(v2).setValue(v1);
						} else if (t1 == CHARACTER) {
							val = true;
							identify.at(t2)->at(v2).setValue(v1); 
						} else if (t1 == PUNCTUATION && v1 == ';') { //If no value, then there is the end signs.
							begin = false;
							con = false;
							var = false;
							id = false;
							init = false;
							val = false;
						} else if (t1 == PUNCTUATION && v1 == ',') {
							id = false;
							init = false;
							val = false;
						} else {
							throw 706;
							break;
						}
					} //End value
					//ELSE (end of declaration)
					else { //The end of the declaration or comma -- checking for a ';' or ','
						if (t1 == PUNCTUATION && v1 == ';') { //Reset all the boolean variables
							begin = false;
							con = false;
							var = false;
							id = false;
							init = false;
							val = false;
						} else if (t1 == PUNCTUATION && v1 == ',') { //If comma, then an id follows
							id = false;
							init = false;
							val = false;
						} else {
							throw 704;
							break;
						}
					} //End else (End of Declarations)
				} //End of type -1, val -1, end of file conditional
			}
			readFile.close();
		} else {
			throw 404;
		}
	} catch (int e) {
		if (e == 404)
			cout << "\nError " << e << ": Unable to open File!\n";
		else if (e == 700)
			cout << "\nError " << e << ": Expected id, found Reserved Word!\n";
		else if (e == 701)
			cout << "\nError " << e
		<< ": Expected variable type, token found not a variable\n";
		else if (e == 702)
			cout << "\nError " << e
		<< ": Expected id, token found not an id!\n";
		else if (e == 703)
			cout << "\nError " << e
		<< ": Illegal type name used in source program!\n";
		else if (e == 704)
			cout << "\nError " << e << ": Syntax Error!\n";
		else if (e == 705)
			cout << "\nError " << e
		<< ": Variable or constant declared twice!\n";
		else if (e == 706)
			cout << "\nError " << e
		<< ": Illegal value type used in source program!\n";
		else
			cout << "\nError " << e
		<< ": Constants must be initialized in declarations!\n";
	}
}

bool isVar(int type) {
	return type == INTEGER_VARIABLE || type == CHARACTER_VARIABLE;
}

bool isCon(int type) {
	return type == INTEGER_CONSTANT || type == CHARACTER_CONSTANT;
}

bool isVarA(int type) {
	return type == INTEGER_VARIABLE_ARRAY || type == CHARACTER_VARIABLE_ARRAY;
}
bool isConA(int type) {
	return type == INTEGER_CONSTANT_ARRAY || type == CHARACTER_CONSTANT_ARRAY;
}

bool isVarST(int type) {
	return isVarA(type) || isVar(type);
}

bool isConST(int type) {
	return isConA(type) || isCon(type);
}

bool isArrST(int type) {
	return isConA(type) || isVarA(type);
}

bool isArrID(hashmap *SymbolTable, int *t, int *v) {
	return isArrST((int)SymbolTable->at(findName(t, v)).at(*v).getType());
}

bool isVarID(hashmap *SymbolTable, int *t, int *v) {
	return isVarST((int)SymbolTable->at(findName(t, v)).at(*v).getType());
}

bool isConID(hashmap *SymbolTable, int *t, int *v) {
	return isConST((int)SymbolTable->at(findName(t, v)).at(*v).getType());
}

bool isDeclaredID(hashmap *SymbolTable, int *t, int *v) {
	return isVarID(SymbolTable, t, v) || isConID(SymbolTable, t, v);
}

string newTmp() {
	string tmp = "_t_" + to_string(tmpNum);
	tmpNum++;
	if (tmpNum > totalTmp)
		totalTmp++;
	return tmp;
}

void resetTmp() {
	tmpNum = 1;
}

/**
 * 
 * Checks the next type and value in the file and returns them
 * 
 */

int nextIs(int codeType, int codeValue, ifstream *fIn) {
	int whatType, whatValue;
	streampos pos = fIn->tellg();
	getLineTVi(fIn, &whatType, &whatValue);
	if (whatType == codeType && whatValue == codeValue)
		return codeValue;
	if (fIn->eof())
		fIn->clear();
	fIn->seekg(pos, ios::beg);
	return 0;
}

int nextIsT(int codeType, ifstream *fIn) {// for type check
	int whatType, waste;
	streampos pos = fIn->tellg();
	getLineTVi(fIn, &whatType, &waste);
	if (whatType == codeType)
		return codeType;
	if (fIn->eof())
		fIn->clear();
	fIn->seekg(pos, ios::beg);
	return 0;
}

// <exp>:= <term> {+ <term> | - <term>}
void exp(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, string *result, ofstream *inter) {
	int op;
	string arg2 = *result;

	bool negate1 = false, negate2 = false;
	while (*type == OPERATOR && *value == '-') {
		getLineTVi(fIn, type, value);
		negate1 = !negate1;
	}
	term(type, value, fIn, SymbolTable, result, inter);
	if(negate1){
		*inter << left << setw(7) << '-';
		*inter << left << setw(14) << result->c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
	while ((op = nextIs(OPERATOR, '+', fIn)) || (op = nextIs(OPERATOR, '-', fIn))) {
		getLineTVi(fIn, type, value);
		while (*type == OPERATOR && *value == '-') {
			getLineTVi(fIn, type, value);
			negate2 = !negate2;
		}
		term(type, value, fIn, SymbolTable, &arg2, inter);
		if (negate2) {
			*inter << left << setw(7) << '-';
			*inter << left << setw(14) << arg2.c_str();
			arg2 = newTmp();
			*inter << left << arg2.c_str() << endl;
		}
		*inter << left << setw(7) << (char)op;
		*inter << left << setw(7) << result->c_str();
		*inter << left << setw(7) << arg2.c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
}

// <term>:= <factor> {* <factor> | / <factor>}
void term(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, string *result, ofstream *inter) {
	int op;
	string arg2 = *result;

	bool negate1 = false, negate2 = false;
	while (*type == OPERATOR && *value == '-') {
		getLineTVi(fIn, type, value);
		negate1 = !negate1;
	}
	factor(type, value, fIn, SymbolTable, result, inter);
	if(negate1){
		*inter << left << setw(7) << '-';
		*inter << left << setw(14) << result->c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
	while ((op = nextIs(OPERATOR, '*', fIn)) || (op = nextIs(OPERATOR, '/', fIn)) || (op = nextIs(OPERATOR, '%', fIn))) {
		getLineTVi(fIn, type, value);
		while (*type == OPERATOR && *value == '-') {
			getLineTVi(fIn, type, value);
			negate2 = !negate2;
		}
		factor(type, value, fIn, SymbolTable, &arg2, inter);
		if (negate2) {
			*inter << left << setw(7) << '-';
			*inter << left << setw(14) << arg2.c_str();
			arg2 = newTmp();
			*inter << left << arg2.c_str() << endl;
		}
		*inter << left << setw(7) << (char)op;
		*inter << left << setw(7) << result->c_str();
		*inter << left << setw(7) << arg2.c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
}

// <factor>:= id | int | (<exp>)
void factor(int *type, int *value, ifstream *fIn, hashmap *SymbolTable, string *result,  ofstream *inter) {
	if((*type == PUNCTUATION && *value == '(')) {
		getLineTVi(fIn, type, value);
		exp(type, value, fIn, SymbolTable, result, inter);
		if ( ')' != nextIs(PUNCTUATION, ')', fIn)){
			cout << "\nIn Factor:\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = "
			<< *value;
			throw 813;
		}
	}
	else if (*type < 1000){
		if(isArrID(SymbolTable, type, value)) {
			if('[' == nextIs(PUNCTUATION, '[', fIn)) {
				string arrName = findName(type, value);
				getLineTVi(fIn, type, value);
				exp(type, value, fIn, SymbolTable, result, inter);
				string arr = newTmp();
				*inter << left << setw(7) << '=' << setw(14) << result->c_str() << arr.c_str() << endl;
				if (']' != nextIs(PUNCTUATION, ']', fIn)) {
					cout << "\nInside Factor:\ntype = " << *type << " value = " << *value;
					getLineTVi(fIn, type, value);
					cout << "\nNextIs type = " << *type << " value = " << *value;
					throw 807;
				}
				*result = newTmp();
				*inter << left << setw(7) << "=[]" << setw(7) << arrName.c_str() << setw(7) << arr.c_str() 
				<< result->c_str() << endl;
			}
			else {
				cout << "\ntype = " << *type << " value = " << *value;
				getLineTVi(fIn, type, value);
				cout << "\nNextIs type = " << *type << " value = " << *value;
				throw 808;
			}
		}
		else if(isDeclaredID(SymbolTable, type, value)) {
			*result = findName(type, value);
		}
		else {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = " << *value;
			throw 805;
		}
	}
	else if ( *type == INTEGER || *type == CHARACTER) {
		*result = to_string(*value);
	}
}

void assignment (int *type, int *value, ifstream *fIn, hashmap *SymbolTable,  ofstream *inter) {
	string result, assign;
	if ('=' == nextIs(OPERATOR, '=', fIn)) { 
		assign = findName(type, value); 
		result = assign;
		getLineTVi(fIn, type, value); 
		exp(type, value, fIn, SymbolTable, &result, inter);
		*inter << left << setw(7) << '=' << setw(14) << result.c_str() << assign.c_str() << endl;
		if (';' != nextIs(PUNCTUATION, ';', fIn)) {
			throw 814; 
		}
	}
	// Array Assignment: Checks through whole array and checks for '=', if no '=' then do nothing. This would be inefficient, but works
	//	in order to be more efficient, make a safe way of checking if there is an equals sign at the end of this w/o losing place in
	//	the stream if there is an = sign, because then you must reset to proper location and evaluate the expresion within the array,
	//	but this would get rid of evaluating array when there is no reason to (no '=' sign)
	else if ('[' == nextIs(PUNCTUATION, '[', fIn)) {
		assign = findName(type, value); //Save array assignment token as the last token to be printed.
		result = assign;
		getLineTVi(fIn, type, value); //Set onto next token -- the begining of exp in []
		exp(type, value, fIn, SymbolTable, &result, inter);
		string arr = newTmp();
		*inter << left << setw(7) << '=' << setw(14) << result.c_str() << arr.c_str() << endl;
		if (']' != nextIs(PUNCTUATION, ']', fIn)) {
			cout << "\nInside Factor:\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = " << *value;
			throw 807;
		}
		//Check for = in case this is an assignment
		if ('=' == nextIs(OPERATOR, '=', fIn)) {
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &result, inter);
			*inter << left << setw(7) << "[]=" << setw(7) << result.c_str() << setw(7) << arr.c_str() << assign.c_str() << endl;
		}
	}
	else if ('+' == nextIs(OPERATOR, '+', fIn) || '-' == nextIs(OPERATOR, '-', fIn)) { 
		cout << "\ntype = " << *type << " value = " << *value;
		getLineTVi(fIn, type, value);
		cout << "\nNextIs type = " << *type << " value = " << *value;
		throw 801;
	} else if ('*' == nextIs(OPERATOR, '*', fIn)
		|| '/' == nextIs(OPERATOR, '/', fIn)
		|| '%' == nextIs(OPERATOR, '%', fIn)) { 
		cout << "\ntype = " << *type << " value = " << *value;
		getLineTVi(fIn, type, value);
		cout << "\nNextIs type = " << *type << " value = " << *value;
		throw 802;
	} else {
		cout << "\ntype = " << *type << " value = " << *value;
		getLineTVi(fIn, type, value);
		cout << "\nNextIs type = " << *type << " value = " << *value;
		throw 810;
	}
}

// io:= <exp> | string | id
void ioAssign(int *ioT, int *ioV, bool output, ifstream *fIn, hashmap *SymbolTable,  ofstream *inter) {
	getLineTVi(fIn, ioT, ioV);
	string result;
	if (output) { 
		if (*ioT == STRING) {
			string str = "\"";
			str.append(findName(ioT, ioV));
			str.push_back('"');
			if (OUT == nextIs(OPERATOR, OUT, fIn)) {
				*inter << left << setw(7) << "cout" << str.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cout" << str.c_str() << endl;
			}
		}
		if (*ioT == CHARACTER) {
			string str = "'";
			str.push_back((char)*ioV);
			str.push_back('\'');
			if (OUT == nextIs(OPERATOR, OUT, fIn)) {
				*inter << left << setw(7) << "cout" << str.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cout" << str.c_str() << endl;
			}

		} else if (*ioT == INTEGER) {
			exp(ioT, ioV, fIn, SymbolTable, &result, inter);
			if (OUT == nextIs(OPERATOR, OUT, fIn)) {
				*inter << left << setw(7) << "cout" << result.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cout" << result.c_str() << endl;
			}
		} else if (*ioT < 1000) {
			if (isDeclaredID(SymbolTable, ioT, ioV)) {
				exp(ioT, ioV, fIn, SymbolTable, &result, inter);
				if (OUT == nextIs(OPERATOR, OUT, fIn)) {
					*inter << left << setw(7) << "cout" << result.c_str() << endl;
					ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
				} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
					*inter << left << setw(7) << "cout" << result.c_str() << endl;
				}
			}
			else {
				throw 805;
			}
		} else if (*ioT == RESERVED_WORD && *ioV == ENDL) {
			if (OUT == nextIs(OPERATOR, OUT, fIn)) {
				*inter << left << setw(7) << "cout" << "\\n" << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cout" << "\\n" << endl;
			}
		}
		else if (*ioT == RESERVED_WORD) {
			throw 806;
		}
	} else {
		if (*ioT == STRING) {
			string str = "\"";
			str.append(findName(ioT, ioV));
			str.push_back('"');
			if (IN == nextIs(OPERATOR, IN, fIn)) {
				*inter << left << setw(7) << "cin" << str.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cin" << str.c_str() << endl;
			}
		}
		if (*ioT == CHARACTER) {
			string str = "'";
			str.push_back((char)*ioV);
			str.push_back('\'');
			if (IN == nextIs(OPERATOR, IN, fIn)) {
				*inter << left << setw(7) << "cin" << str.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cin" << str.c_str() << endl;
			}
		} else if (*ioT == INTEGER) {
			exp(ioT, ioV, fIn, SymbolTable, &result, inter);
			if (IN == nextIs(OPERATOR, IN, fIn)) {
				*inter << left << setw(7) << "cin" << result.c_str() << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cin" << result.c_str() << endl;
			}
		}
		else if (*ioT < 1000) {
			if (isDeclaredID(SymbolTable, ioT, ioV)) {
				exp(ioT, ioV, fIn, SymbolTable, &result, inter);
				if (IN == nextIs(OPERATOR, IN, fIn)) {
					*inter << left << setw(7) << "cin" << result.c_str() << endl;
					ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
				} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
					*inter << left << setw(7) << "cin" << result.c_str() << endl;
				}
			}
			else{
				throw 805;
			}
		} else if (*ioT == RESERVED_WORD && *ioV == ENDL) {
			if (IN == nextIs(OPERATOR, IN, fIn)) {
				*inter << left << setw(7) << "cin" << "\\n" << endl;
				ioAssign(ioT, ioV, output, fIn, SymbolTable, inter);
			} else if (';' == nextIs(PUNCTUATION, ';', fIn)) {
				*inter << left << setw(7) << "cin" << "\\n" << endl;
			}
		}
		else if (*ioT == RESERVED_WORD) {
			throw 806;
		}
	}
}

string newL() {
	string tmpL = "L_" + to_string(LNum);
	LNum++;
	return tmpL;
}

void resetL() {
	LNum = 1;
}

void checkNext(ifstream *fIn) {
	int whatType, whatValue;
	streampos pos = fIn->tellg();
	getLineTVi(fIn, &whatType, &whatValue);
	cout << "Checking Next:\nNext Type = " << whatType << " Next Value = " << whatValue << endl;
	if (fIn->eof())
		fIn->clear();
	fIn->seekg(pos, ios::beg);
}

void checkNexty(ifstream *fIn) {
	int whatType, whatValue;
	streampos pos = fIn->tellg();
	getLineTVi(fIn, &whatType, &whatValue);
	cout << "Checking Next:\nNext Type = " << whatType << " Next Value = " << whatValue << endl;
	getLineTVi(fIn, &whatType, &whatValue);
	cout << "Checking Next2:\nNext2 Type = " << whatType << " Next Value = " << whatValue << endl;
	getLineTVi(fIn, &whatType, &whatValue);
	cout << "Checking Next3:\nNext3 Type = " << whatType << " Next Value = " << whatValue << endl;
	getLineTVi(fIn, &whatType, &whatValue);
	cout << "Checking Next4:\nNext4 Type = " << whatType << " Next Value = " << whatValue << endl;
	if (fIn->eof())
		fIn->clear();
	fIn->seekg(pos, ios::beg);
}

int checkNextR(int codeType, int codeValue, ifstream *fIn) {
	int whatType, whatValue;
	streampos pos = fIn->tellg();
	fIn->seekg(pos, ios::beg);
	getLineTVi(fIn, &whatType, &whatValue);
	if (fIn->eof())
		fIn->clear();
	fIn->seekg(pos, ios::beg);
	if (whatType == codeType && whatValue == codeValue)
		return codeValue;
	else
		return 0;
}

void orExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter) {
	int op;
	string exp2 = *result;
	andExp(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, inter);
	while((op = nextIs(OPERATOR,OR,fIn))){
		andExp(type, value, isWhile, fIn, SymbolTable, &exp2, skipTrue, boolResult, inter);
		*inter << left << setw(7) << '|' << setw(7) << result->c_str() << setw(7) << exp2.c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
}

void andExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter) {
	int op;
	string exp2 = *result;
	notExp(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, inter);
	while((op = nextIs(OPERATOR,AND,fIn))){
		notExp(type, value, isWhile, fIn, SymbolTable, &exp2, skipTrue, boolResult, inter);
		*inter << left << setw(7) << '&' << setw(7) << result->c_str() << setw(7) << exp2.c_str();
		*result = newTmp();
		*inter << left << result->c_str() << endl;
	}
}

void notExp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, ofstream *inter) {
	if('!' == nextIs(OPERATOR,'!',fIn)) {
		string save;
		RelOp(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, 0, &save, inter);
		*inter << left << setw(7) << '^' << setw(7) << result->c_str() << setw(7) << '1' << result->c_str() << endl;
	} else {
		string save;
		RelOp(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, 0, &save, inter);
	}
}

void RelOp(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult, int RoP, string *save, ofstream *inter) {
	if ('(' == nextIs(PUNCTUATION, '(', fIn)) {
		orExp(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, inter);
		if(')' != nextIs(PUNCTUATION, ')', fIn)){
			throw 836;
		}
	} else {
		semiConditionPrep(type, value, isWhile, fIn, SymbolTable, result, skipTrue, boolResult, inter);
	}
}

void contentBlockPrep(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, ofstream *inter) {
	bool lookForCurly = false;
	if ('{' == nextIs(PUNCTUATION, '{', fIn)) {
		lookForCurly = true;
	}
	getLineTVi(fIn, type, value);
	contentBlock(type, value, fIn, SymbolTable, result, inter);
	while(lookForCurly && ('}' != nextIs(PUNCTUATION,'}',fIn) && !fIn->eof())) {
		getLineTVi(fIn, type, value);
		contentBlock(type, value, fIn, SymbolTable, result, inter);
	}
	if (fIn->eof()) {
		if (isWhile) {
			throw 824;
		} else {
			throw 825;
		}
	} else if (!lookForCurly && '}' == nextIs(PUNCTUATION, '}', fIn)) {
		if (isWhile) {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = "
			<< *value;
			throw 826;
		} else {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = "
			<< *value;
			throw 827;
		}
	}
}

void semiCondition(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, ofstream *inter) {
	string exp2 = *result;
	getLineTVi(fIn, type, value);
	exp(type, value, fIn, SymbolTable, result, inter);

	getLineTVi(fIn, type, value);
	if(*type == OPERATOR){
		if(*value == NOT_EQUAL_TO){
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << "!=" << setw(7) << result->c_str() << setw(7) 
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		} else if(*value == '<') {
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << "<" << setw(7) << result->c_str() << setw(7)
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		} else if(*value == LESS_THAN_OR_EQUAL_TO) {
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << "<=" << setw(7) << result->c_str() << setw(7) 
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		} else if(*value == EQUAL_TO) {
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << "==" << setw(7) << result->c_str() << setw(7)
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		} else if(*value == GREATER_THAN_OR_EQUAL_TO){
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << ">=" << setw(7) << result->c_str() << setw(7) 
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		} else if(*value == '>') {
			getLineTVi(fIn, type, value);
			exp(type, value, fIn, SymbolTable, &exp2, inter);
			*skipTrue = newL();
			*inter << left << setw(7) << ">" << setw(7) << result->c_str() << setw(7) 
			<< exp2.c_str() << setw(7) << "goto" << skipTrue->c_str() << endl;
		}
	}
}

void semiConditionPrep(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *result, string *skipTrue, string *boolResult,  ofstream *inter) {
	semiCondition(type, value, isWhile, fIn, SymbolTable, result, skipTrue, inter);
	string toTrue = newL();
	*result = newTmp();
	*inter << left << setw(7) << '=' << setw(14) << '0' << result->c_str() << endl;
	*inter << left << setw(7) << "goto" << toTrue.c_str() << endl;
	*inter << left << skipTrue->c_str() << endl;
	*inter << left << setw(7) << '=' << setw(14) << '1' << result->c_str() << endl;
	*inter << left << toTrue.c_str() << endl;
}

void condition(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable, 
	string *falseL, string *skipL,  ofstream *inter) {
	string result, skipTrue, boolResult;
	string save = boolResult;
	orExp(type, value, isWhile, fIn, SymbolTable, &result, &skipTrue, &boolResult, inter);

	*inter << left << setw(7) << "==" << setw(7) << result.c_str() << setw(7) << '0' 
	<< setw(7) << "goto" << falseL->c_str() << endl;

	if (')' == nextIs(PUNCTUATION, ')', fIn)) {
		contentBlockPrep(type, value, isWhile, fIn, SymbolTable, &result, inter);
		string storeSkip;
		if(!isWhile){
			*skipL = newL();
		}
		*inter << left << setw(7) << "goto" << skipL->c_str() << endl;
	} else {
		if (isWhile) {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = " << *value;
			throw 830;
		} else {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = " << *value;
			throw 831;
		}
	}
}

void WhileLoop(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable,  ofstream *inter) {
	string initialL = newL();
	*inter << left << initialL.c_str() << endl;
	string falseL = newL();
	string falseLabel = falseL;
	if ('(' == nextIs(PUNCTUATION, '(', fIn)) {
		condition(type, value, true, fIn, SymbolTable, &falseL, &initialL, inter);
		*inter << left << falseL.c_str() << endl;
	} else {
		cout << "\ntype = " << *type << " value = " << *value;
		getLineTVi(fIn, type, value);
		cout << "\nNextIs type = " << *type << " value = " << *value;
		throw 820;
	}
}

void IfSeries(int *type, int *value, bool isWhile, ifstream *fIn, hashmap *SymbolTable,  ofstream *inter) {
	string endOfIf;
	if ('(' == nextIs(PUNCTUATION, '(', fIn)) {
		string falseL = newL();
		condition(type, value, false, fIn, SymbolTable, &falseL, &endOfIf, inter);
		*inter << left << falseL.c_str() << endl;
	} else if (ELSE == nextIs(RESERVED_WORD, ELSE, fIn)) {
		throw 833;
	} else{
		cout << "\nFirst If in If Series.";
		cout << "\nElse currently believes it need parenthesises when it does not."
		"\n The real problem is an Else shouldn't be here.";
		cout << "\ntype = " << *type << " value = " << *value;
		getLineTVi(fIn, type, value);
		cout << "\nNextIs type = " << *type << " value = " << *value;
		throw 821;
	}
	bool hitElse = false;
	while (ELSE == nextIs(RESERVED_WORD,ELSE, fIn) && !hitElse) {
		if (IF == nextIs(RESERVED_WORD,IF, fIn) && !hitElse) {	//else if
			if ('(' == nextIs(PUNCTUATION, '(', fIn)) { 
				cout << endl << "ELSE IF" << endl;
				string falseL = newL();
				condition(type, value, false, fIn, SymbolTable, &falseL, &endOfIf, inter);
				*inter << left << falseL.c_str() << endl;
			} else {
				cout << "\ntype = " << *type << " value = " << *value;
				getLineTVi(fIn, type, value);
				cout << "\nNextIs type = " << *type << " value = " << *value;
				throw 821;
			}
		} else { //else
			if (!hitElse) {
				hitElse = true;
				string result;
				contentBlockPrep(type, value, isWhile, fIn, SymbolTable, &result, inter);
			}
		}
	}
	*inter << left << endOfIf.c_str() << endl;
}

void contentBlock(int *type, int *value, ifstream *fIn, hashmap *SymbolTable,
	string *result, ofstream *inter) {
	if (*type < 1000 && *type >= 0) {
		assignment(type, value, fIn, SymbolTable, inter);
	} else if (*type == RESERVED_WORD) {
		if (*value == COUT) { 
			if (OUT == nextIs(OPERATOR, OUT, fIn)) { 
				if (*type == -1 && *value == -1) { 
					throw 800;
				}
				ioAssign(type, value, true, fIn, SymbolTable, inter);
			} else {
				throw 803;
			}
		} else if (*value == CIN) {	
			if (IN == nextIs(OPERATOR, IN, fIn)) { 
				if (*type == -1 && *value == -1) { 
					throw 800;
				}
				ioAssign(type, value, false, fIn, SymbolTable, inter);
			} else {
				throw 804;
			}
		} else if (*value == WHILE) { 
			WhileLoop(type, value, true, fIn, SymbolTable, inter);
		} else if (*value == IF || *value == ELSE) { 
			IfSeries(type, value, false, fIn, SymbolTable, inter);
		} else if (*value == ELSE) {
			cout << "\ntype = " << *type << " value = " << *value;
			getLineTVi(fIn, type, value);
			cout << "\nNextIs type = " << *type << " value = " << *value;
			throw 834;
		}
	}
}

void parse2(hashmap *SymbolTable) {
	try {
		bool halt = true;
		string result;
		ifstream fIn("lex.txt");
		if (fIn.is_open()) {
			int type, value;
			resetL();
			ofstream inter("intermediate.txt", ofstream::trunc);
			while (fIn.good()) {
				getLineTVi(&fIn, &type, &value);
				if (halt) {
					if (type == RESERVED_WORD && value == MAIN)
						halt = false;
				} else { 
					resetTmp();
					contentBlock(&type, &value, &fIn, SymbolTable, &result, &inter);// the content contained by '{' and '}'
				}
			}
			fIn.close();
			inter.close();
		} else {
			throw 404;
		}
	} catch (int e) {
		if (e == 404)
			cout << "\nError " << e << ": Unable to open File.\n";
		else if (e == 800)
			cout << "\nError " << e << ": Incomplete assignment.\n";
		else if (e == 801)
			cout << "\nError " << e << ": Expression without assignment.\n";
		else if (e == 802)
			cout << "\nError " << e << ": Term without assignment.\n";
		else if (e == 803)
			cout << "\nError " << e << ": cout must be followed by \"<<\".\n";
		else if (e == 804)
			cout << "\nError " << e << ": cin must be followed by \">>\".\n";
		else if (e == 805)
			cout << "\nError " << e << ": Undeclared variable in assignment statement.\n";
		else if (e == 806)
			cout << "\nError " << e << ": Use of a reserved word in assignment statement.\n";
		else if (e == 807)
			cout << "\nError " << e << ": Missing right bracket ']'.\n";
		else if (e == 808)
			cout << "\nError " << e << ": Variable declared as an Array is missing its following brackets.\n";
		else if (e == 809)
			cout << "\nError " << e << ": Missing '<<' or ';' in cout assignment statement.\n";
		else if (e == 810)
			cout << "\nError " << e << ": Improper token following in assignment statement.\n";
		else if (e == 811)
			cout << "\nError " << e << ": Non-Integer or Non-Character Token in assignment statement.\n";
		else if (e == 812)
			cout << "\nError " << e << ": Missing '<<' or ';' in cout statement.\n";
		else if (e == 813)
			cout << "\nError " << e << ": Missing closing parenthesis in expression.\n";
		else if (e == 814)
			cout << "\nError " << e << ": Missing semi-colon at end of assignment statement.\n";
		else if (e == 820)
			cout << "\nError " << e << ": Missing beginning parenthesis at start of while condition.\n";
		else if (e == 821)
			cout << "\nError " << e << ": Missing beginning parenthesis at start of if condition.\n";
		else if (e == 822)
			cout << "\nError " << e << ": Missing ending parenthesis at end of while condition.\n";
		else if (e == 823)
			cout << "\nError " << e << ": Missing ending parenthesis at end of if condition.\n";
		else if (e == 824)
			cout << "\nError " << e << ": Missing ending curly brace at end of while block.\n";
		else if (e == 825)
			cout << "\nError " << e << ": Missing ending curly brace at end of if block.\n";
		else if (e == 826)
			cout << "\nError " << e << ": Excess ending curly brace at end of while block.\n";
		else if (e == 827)
			cout << "\nError " << e << ": Excess ending curly brace at end of if block.\n";
		else if (e == 828)
			cout << "\nError " << e << ": Missing ending parenthesis in while conditional statement.\n";
		else if (e == 829)
			cout << "\nError " << e << ": Missing ending parenthesis in if conditional statement.\n";
		else if (e == 830)
			cout << "\nError " << e << ": Excess ending parenthesis in while conditional statement.\n";
		else if (e == 831)
			cout << "\nError " << e << ": Excess ending parenthesis in if conditional statement.\n";
		else if (e == 832)
			cout << "\nError " << e << ": Incorrect operator in condition's boolean logic.\n";
		else if (e == 833)
			cout << "\nError " << e << ": If Else statement without an an initial if statement.\n";
		else if (e == 834)
			cout << "\nError " << e << ": Else statement without an an initial if statement.\n";
		else if (e == 835)
			cout << "\nError " << e << ": Missing Negation's beginning parenthesis.\n";
		else if (e == 834)
			cout << "\nError " << e << ": Missing Negation's ending parenthesis.\n";
	}
}

string findName(int * type, int * value){
	for (hashmap::iterator found = st.begin();
			found != st.end(); ++found) { //Iterate through different tokens
		if (!found->second.empty()) {
			for (int i = 0; i < found->second.size(); i++) {
				if(found->second.at(i).getType() == *type && found->second.at(i).getValue() == *value){
					return found->second.at(i).getName();
				}
			}
		}
	}
	return "Item Non-Existent in Symbol Table.";
}
bool isInt(int type){
	return (type == INTEGER_CONSTANT || type == INTEGER_VARIABLE || type == INTEGER_CONSTANT_ARRAY || type == INTEGER_VARIABLE_ARRAY);
}

bool isChar(int type){
	return (type == CHARACTER_VARIABLE || type == CHARACTER_CONSTANT || type == CHARACTER_CONSTANT_ARRAY || type == CHARACTER_VARIABLE_ARRAY);
}

bool isSetVar(int type){
	return (isInt(type) || isChar(type));
}

void createDeclarations(hashmap *SymbolTable){ 
	for (hashmap::iterator found = SymbolTable->begin();
		found != SymbolTable->end(); ++found) {
		if (!found->second.empty()) {
			for (int i = 0; i < found->second.size(); i++) {
				int t = found->second.at(i).getType();
				if (isSetVar(t)){
					if (isConST(t)){
						cout << "const ";
					}
					if (t == INTEGER_VARIABLE || t == INTEGER_CONSTANT) {
						cout << "int " << found->second.at(i).getName() << " = " << found->second.at(i).getValue() << ';' << endl;
					}
					else if (t == CHARACTER_VARIABLE || t == CHARACTER_CONSTANT) {
						cout << "char " << found->second.at(i).getName() << " = '" << (char)found->second.at(i).getValue() << "';" << endl;
					}
					else if (t == INTEGER_VARIABLE_ARRAY || t == INTEGER_CONSTANT_ARRAY) { 
						cout << "int " << found->second.at(i).getName() << "[";
						if (found->second.at(i).getSize() >= 0) {
							cout << found->second.at(i).getSize();
						}
						cout <<"];" << endl;
					}
					else if (t == CHARACTER_VARIABLE_ARRAY || t == CHARACTER_CONSTANT_ARRAY) { 
						cout << "char " << found->second.at(i).getName() << "[";
						if (found->second.at(i).getSize() >= 0) {
							cout << found->second.at(i).getSize();
						}
						cout <<"];" << endl;
					} else {
						cout << "Error: Problem reading Set Variables when creating declarations.\n";
					}
				}
			}
		}
	}
	for (int i = 0; i < totalTmp; i++){	
		if (i == 0) {
			cout << "int";
		}
		if(i != totalTmp-1) {
			cout << " _t_" << i << ',';
		} else {
			cout << " _t_" << i << ";\n";
		}
	}
}

//Splits intermediate code into tokens
vector<string> split(string line) {	
	vector<string> result;
	if(line.size() <= 7) {
		result.push_back(line.substr(0, line.find_first_of(" ")));
	} else {
		string seg = line.substr(0, line.find_first_of(" "));
		result.push_back(seg);
		line = line.substr(7);
		if(seg.compare("cout") == 0 || seg.compare("cin") == 0) {
			result.push_back(line);
		} else { //Not output/input
			while(!(line.size() <= 0)) { 
				if (line.size()<=7) {
					if (line.find(" ") != string::npos) {
						line = line.substr(line.find_first_of(" "), line.find_last_of(" "));
					}
					result.push_back(line);
					break;
				} else {
					if ((line.substr(0,7)).compare("       ") == 0) {
						line = line.substr(7);
					} else {
						result.push_back(line.substr(0, line.find_first_of(" ")));
						line = line.substr(7);
					}
				}
			}
		}
	}
	return result;
}

//Translate intermediate code into c++ code
void translate() {
	try {
		ifstream in("intermediate.txt");
		if (in.is_open()) {
			string line;
			while(in.good()) {
				getline(in,line);
				if(line.empty()) {
					break; //File is done, corrupted, or incorrect
				}
				vector<string> str = split(line);
				if(str.size() == 1) {
					cout << str.at(0) << ':' << endl;
				} else if(str.size() == 2) { //goto Label or cout/cin
					if (str.at(0).compare("cout") == 0) {
						cout << str.at(0) << " << ";
						if (str.at(1).compare("\\n") == 0) {
							cout << "endl";
						} else {
							cout << str.at(1);
						}
						cout << ';' << endl;
					} else if (str.at(0).compare("cin") == 0) {
						cout << str.at(0) << " >> " << str.at(1) << ';' << endl;
					} else if (str.at(0).compare("goto") == 0) {//goto label
						cout << str.at(0) << " " << str.at(1) << ';' << endl;
					} else {
						cout << "str.at(0) = " << str.at(0) << endl;
						throw 900;
					}
				} else if(str.size() == 3) { //assignment =
					if (str.at(0).compare("=") == 0) {
						cout << str.at(2) << " " << str.at(0) << " " << str.at(1) << ';' << endl;
					} else {
						throw 901;
					}
				} else if(str.size() == 4) { //arithmatic or RELOP (& or ^)
					if (str.at(0).compare("+") == 0 || str.at(0).compare("-") == 0 || str.at(0).compare("*") == 0 || str.at(0).compare("/") == 0 || str.at(0).compare("%") == 0) {	//arithmetic
						cout << str.at(3) << " = " << str.at(1) << " " << str.at(0) << " " << str.at(2) << ';' << endl;
					}
					//REL OPs
					else if (str.at(0).compare("^") == 0) {	// ^
						cout << str.at(3) << " = " << str.at(1) << " ^ " << str.at(2) << ';' << endl;
					} else if ( str.at(0).compare("&") == 0) { // &
						cout << str.at(3) << " = " << str.at(1) << " & " << str.at(2) << ';' << endl;
					} else if (str.at(0).compare("|") == 0) { // |
						cout << str.at(3) << " = " << str.at(1) << " | " << str.at(2) << ';' << endl;
					} else if (str.at(0).compare("=[]") == 0) {
						cout << str.at(3) << " = " << str.at(1) << '[' << str.at(2) << "];" << endl;
					} else if (str.at(0).compare("[]=") == 0) {
						cout << str.at(3) << "[" << str.at(2) << "] = " << str.at(1) << ";" << endl;
					} else {
						throw 902;
					}
				} else if(str.size() == 5) { //	conditional
					if (str.at(0).compare(">") == 0 || str.at(0).compare(">=") == 0  || str.at(0).compare("==") == 0 || str.at(0).compare("<=") == 0 || str.at(0).compare("<") == 0) {
						cout << "if ( " << str.at(1) << " " << str.at(0) << " " << str.at(2) << " ){ goto " << str.at(4) << ":}\n";
					} else {
						throw 903;
					}
				} else {
					throw 904;
				}
			}
			in.close();
		} else {
			throw 404;
		}
	} catch (int e) {
		if (e == 404)
			cout << "Error " << e << ": File could not be opened.\n";
		else if (e == 900)
			cout << "Error " << e << ": Invalid data in intermediate code for a two token line.\n";
		else if (e == 901)
			cout << "Error " << e << ": Invalid data in intermediate code for a three token line.\n";
		else if (e == 902)
			cout << "Error " << e << ": Invalid data in intermediate code for a four token line.\n";
		else if (e == 903)
			cout << "Error " << e << ": Invalid data in intermediate code for a five token line.\n";
		else if (e == 904)
			cout << "Error " << e << ": Invalid data in intermediate code: Too many tokens.\n";
	}
}

void codeGeneration(hashmap *SymbolTable) {
	createDeclarations(SymbolTable); //Creates declarations of all vars from symbol table
	cout << "\nint main(){\n";
	translate();	//Translates each line of intermediate code into 1 line c++ code.
	cout << "return 0;\n}\n";
}

void printST(hashmap *SymbolTable) {
	printf("\n%*s %*s %*s %*s %*s %*s\n", -25, "Hash Value:", -10, "Name:", -7,
		"Type:", -10, "Value:", -7, "Size:", -7, "elType:");
	hashmap::hasher func = SymbolTable->hash_function();
	for (hashmap::iterator found = SymbolTable->begin();
		found != SymbolTable->end(); ++found) {
		if (!found->second.empty()) {
			STs++;
			printf("%*lu %*s %*c \n", -25, func(found->first), -10,
				&found->first.at(0), 0, ':');
			for (int i = 0; i < found->second.size(); i++) {
				printf("%*c %*s %*i %*i %*i %*i \n", -25, ' ', -10,
					found->second.at(i).getName().c_str(), -7,
					found->second.at(i).getType(), -10,
					found->second.at(i).getValue(), -7,
					found->second.at(i).getSize(), -7,
					found->second.at(i).getElType());
			}
			cout << endl;
		}
	}
}

//Record symbol table into history
void record1(hashmap *SymbolTable) {
	for (hashmap::iterator found = SymbolTable->begin();
		found != SymbolTable->end(); ++found) {
		if (!found->second.empty()) {
			Hs++;
			vector<vector<Token> > newRow; //Collision vector.
			for (int i = 0; i < found->second.size(); i++) { 
				vector<Token> newVariation; //Variation vector for storing changes in states throughout history
				newVariation.push_back(found->second.at(i));
				newRow.push_back(newVariation);
			}
			history.push_back(newRow); //Stores the new row into the main vector of different tokens
		}
	}
}

void printHistory() {
	for(int i = 0; i < history.size(); i++){
		for(int j = 0; j < history.at(i).size(); j++){
			for(int k = 0; k < history.at(i).at(j).size(); k++){
				printf("%*c %*s %*i %*i %*i %*i \n", -25, ' ', -10,
					history.at(i).at(j).at(k).getName().c_str(), -7,
					history.at(i).at(j).at(k).getType(), -10,
					history.at(i).at(j).at(k).getValue(), -7,
					history.at(i).at(j).at(k).getSize(), -7,
					history.at(i).at(j).at(k).getElType());
			}//Variants printed
			cout << endl;
		}//Collisions printed
		cout << '-' << endl;
	}
}

int main(int argc, char** argv) {
	cout
	<< "The print out of the Symbol Table will show ALL rows in the Symbol Table,"
	<< " including rows that are empty w/ no actual Tokens in the vector:\n\n";
	hashmap SymbolTable;
	SymbolTable.reserve(200);
	lexicalAnalyzer(&SymbolTable);
	//printST(&SymbolTable);

	//record1(&SymbolTable);
	st = SymbolTable;
	/*
	cout << "\nHistory:\n";
	printHistory();
	//if(history.size() == SymbolTable.size())
	if(Hs == STs)
		cout << "\nMatch in Size:\n";
	else
		cout << "\nNot match in Size: ST: " << STs << " History: "<< Hs <<"\n";
	*/
	parseDeclarations(&SymbolTable);
	//cout << "\nAdjusted Symbol Table\n";
	//record2(&SymbolTable);
	//printST(&SymbolTable);
	//cout << "\nParse Tree:\n";
	//printf("%*s %*s %*s %s \n", -4, "OP", -6,"arg1" , -6, "arg2", "Result");
	parse2(&SymbolTable);

	// Code Gen
	codeGeneration(&SymbolTable);

	return 0;
}