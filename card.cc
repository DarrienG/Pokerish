#include "card.h"
#include <iostream>
#include <utility>

using std::cout;

// Why are these even here? That's a great question
// 2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace
const int card::faceEnum[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

const int card::suitEnum[] = {0, 1, 2, 3};

// Why not start with ace? That's the technical ordering after all, plus that way
// we get to have the numbers actually be their own numbers.
// Since an Ace is the most powerful card, this allows us to directly evaluate cards
// using their internal numbering system rather than performing odd operations on them.
// e.g. 13 always beats 1, but making logic for 1 beating 13 would be a real pain.

// Rather than 17 conditionals, we have two static arrays here to be used as lookup 
// tables. This uses a bit more memory, but drastically improves performance
const string card::faceTable[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
const string card::suitTable[] = {"Spades ♠", "Hearts ❤", "Clubs ♣", "Diamonds ♦"};

card::card(int face_, int suit_){
	infoPair.first = face_;
	infoPair.second = suit_;
}

// If the user wants a new line at the end of the string, all they need to do is add 
// a true as the argument. Argument is optional, so it requires no extra work from the 
// user
string card::toString(bool newLine){
	//cout << "Start toString\n";
	string nl;
	newLine ? nl = "\n" : nl = "";
	//cout << "End toString\n";
	return faceTable[infoPair.first] + " of " + suitTable[infoPair.second] + nl;
}

pair<int, int> card::getCardInfo(){
	return infoPair;
}
