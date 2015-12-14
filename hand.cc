#include "hand.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

const string hand::rankString[] = {"Singles", "One Pair", "Two Pairs", "Three of a kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush"};

hand::hand(vector<card> cards_){
	if (cards_.size() < 5)
	{
		cout << "Sizing Error: Hand size too small." << endl;
		exit(1);
	}
	
	// Adding cards to the user's hand
	for (int i = 0; i < 5; ++i)
	{
		cards.push_back(cards_[i]);
	}

	// Sorts hand and determines how powerful hand is
	calcRank();
}

vector<card> hand::getCards(){
	return cards;
}

// User decides which cards they would like to keep by entering a string of 0's, and 1's
// where 1's mean that at that spot, they would like to replace that card with another one
// Example string: 00010 would have the 4th card erased, and deleted and replaced with a new
// card from the deck at the end
void hand::replaceCards(string repString, deck &deckCards){
	// Hand will always be of size 5, so we hardcode in the size 5
	size_t stringSize = repString.size();
	if (stringSize < 6)
	{
		for (size_t i = 0; i < 6 - stringSize; ++i)
		{
			repString.append("0");
		}
	}

	for (int i = 0, index = 0, cnt = 0; i < 5; ++i, ++index)
	{
		if (repString.at(i) == '1' && cnt < 3)
		{
			cards.erase(cards.begin() + index);
			cards.push_back(deckCards.dealCard());
			--index;
			++cnt;
		}
	}
	calcRank();
}

int hand::calcRank(){
	sort();
	resetFlags();

	// Inadvertently sets hasStraight and hasFlush flags
	if (hasStraightFlush())
	{
		powerCard = topPower();
		rank = 8;
		return 8;
	}

	if (hasFourOfAKind())
	{
		powerCard = fourFace;
		rank = 7;
		return 7;
	}

	// inadvertantly sets threeFlag and pairFlag
	if (hasFullHouse())
	{
		powerCard = topPower();
		rank = 6;
		return 6;
	}

	if (flushFlag)
	{
		powerCard = topPower();
		rank = 5;
		return 5;
	}

	if (straightFlag)
	{
		powerCard = topPower();
		rank = 4;
		return 4;
	}

	if (threeFlag)
	{
		powerCard = threeFace;
		rank = 3;
		return 3;
	}

	if (hasTwoPairs())
	{
		powerCard = twoFace;
		rank = 2;
		return 2;
	}

	if (pairFlag)
	{
		powerCard = twoFace;
		rank = 1;
		return 1;
	}

	powerCard = topPower();
	rank = 0;
	return 0;
}

bool hand::operator>(const hand &rhs){
	return rank == rhs.rank ? powerCard > rhs.powerCard : rank > rhs.rank; 
}

bool hand::operator<(const hand &rhs){
	return rank == rhs.rank ? powerCard < rhs.powerCard : rank < rhs.rank; 
}

// Basic bubble sort to sort cards BY FACE for easier evaluation 
// Yes I could use a better sort, but we're only working with sets of
// 5 values. O(n^2) only means 25 iterations, and that's small potatoes
// Or is this insertion sort? I don't remember. Anyway, it's what we're using
void hand::sort(){
	bool swapped = true;
	int idx = 0;
	while(swapped){
		swapped = false;
		for (int i = 0; i < 5 - idx; ++i)
		{
			if (cards[i].getCardInfo().first > cards[i + 1].getCardInfo().first)
			{
				swap(i, i + 1);
				swapped = true;
			}
		}
	}
}


void hand::swap(int idx1, int idx2){
	card tmpCard = cards[idx1];
	cards[idx1] = cards[idx2];
	cards[idx2] = tmpCard;
}

// Finds the most powerful card in the player's hand 
// Necessary after modifying the hand in any way
// It is assumed that the player's cards are already sorted
// In fact, the only time this function is ever called is inside sort

void hand::resetFlags(){
		straightFlag = 0;
		flushFlag = 0;
		threeFlag = 0;
		pairFlag = 0;
}

string hand::rankToString(){
	return rankString[rank];
}

// Used to determine the most powerful card in singles, a straight, a flush, a full house,
// and a straight flush
int hand::topPower(){
	return cards[cards.size() - 1].getCardInfo().first;
}

int hand::getRank(){
	return rank;
}

int hand::getTwoFace(){
	return twoFace;
}

int hand::getThreeFace(){
	return threeFace;
}

int hand::getFourFace(){
	return fourFace;
}

/************************************************
 * RANK CALCULATIONS BEGIN
 ************************************************/

bool hand::hasStraightFlush(){
	return hasStraight() && hasFlush();
}

bool hand::hasFourOfAKind(){
	int count = 1;
	int faceMatch = cards[0].getCardInfo().first;

	// Set faceMatch to the face of the first card in player's hand 
	// Compare to face of every other card, increment count when like
	// card is found
	for (size_t i = 1; i < cards.size(); ++i)
	{
		if (faceMatch == cards[i].getCardInfo().first)
		{
			++count;
		}
	}

	if (count == 4)
	{
		fourFace = faceMatch;
		return 1;
	}

	// Count is less than 4, but greater than 1, thus we know there is not
	// 4 of a kind
	if (count > 1)
	{
		return 0;
	}

	// Same deal as above, but now faceMatch is the second card, and we compare to
	// every card after that
	count = 1;
	faceMatch = cards[1].getCardInfo().first;	
	for (size_t i = 2; i < cards.size(); ++i)
	{
		if (faceMatch == cards[i].getCardInfo().first)
		{
			++count;
		}
	}

	if (count == 4)
	{
		return 1;
	}

	return 0;

}

bool hand::hasFullHouse(){
	return hasThreeOfAKind() ? pairMod() : 0;
}

// We know the hand has a triple, now we need to see if it has a double too 
bool hand::pairMod(){
	vector<card> modifiedHand = cards;
	for (int i = 0, idx = 0; i < 5; ++i, ++idx)
	{
		if (modifiedHand[idx].getCardInfo().first == threeFace)
		{
			modifiedHand.erase(modifiedHand.begin() + idx);
			--idx;
		}
	}

	return modifiedHand[0].getCardInfo().first == modifiedHand[1].getCardInfo().first ? 1 : 0;
}

bool hand::hasFlush(){
	int startFace = cards[0].getCardInfo().second;	
	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (startFace != cards[i].getCardInfo().second)
		{
			return 0;
		}
	}
	flushFlag = 1;
	return 1;
}

bool hand::hasStraight(){
	int startGroup = cards[0].getCardInfo().first;
	for (size_t i = 0; i < cards.size(); ++i, ++startGroup)
	{
		if (startGroup != cards[i].getCardInfo().first)
		{
			return 0;
		}
	}
	straightFlag = 1;
	return 1;
}

// I wish I knew a more efficient way to do these (◞‸◟；)
bool hand::hasThreeOfAKind(){
	threeFace = -1;
	int count, faceMatch;

	for (int i = 0; i < 3; ++i)
	{
		count = 1;
		faceMatch = cards[i].getCardInfo().first;
		for (size_t j = i + 1; j < cards.size(); ++j)
		{
			if (faceMatch == cards[j].getCardInfo().first)
			{
				++count;
			}
		}
		if (count == 3)
		{
			threeFace = faceMatch;
			threeFlag = 1;
			return 1;
		}
	}
	return 0;
}

bool hand::hasTwoPairs(){
	return hasPair() ? duoPairMod() : 0;
}

bool hand::duoPairMod(){
	vector<card> modifiedHand = cards;
	for (int i = 0, idx = 0; i < 5; ++i, ++idx)
	{
		if (modifiedHand[idx].getCardInfo().first == twoFace)
		{
			modifiedHand.erase(modifiedHand.begin() + idx);
			--idx;
		}
	}
	// Time for the most verbose line of my whole C++ career
	if (modifiedHand[0].getCardInfo().first == modifiedHand[1].getCardInfo().first)
	{
		if (twoFace < modifiedHand[0].getCardInfo().first)
		{
			twoFace = modifiedHand[0].getCardInfo().first;
		}
		return 1;
	}

	if (modifiedHand[0].getCardInfo().first == modifiedHand[2].getCardInfo().first)
	{
		if (twoFace < modifiedHand[0].getCardInfo().first)
		{
			twoFace = modifiedHand[0].getCardInfo().first;
		}
		return 1;
	}

	if (modifiedHand[1].getCardInfo().first == modifiedHand[2].getCardInfo().first)
	{
		if (twoFace < modifiedHand[0].getCardInfo().first)
		{
			twoFace = modifiedHand[0].getCardInfo().first;
		}
		return 1;
	}

	return 0;
}

bool hand::hasPair(){
	int count, faceMatch;
	twoFace = -1;

	// We are now searching to see if there are pairs based off of a specific index
	// This is necessary so that we can check to see if there is more than one set of 
	// pairs, as the following loop shorts after finding one

	for (int i = 0; i < 5; ++i)
	{
		count = 1;
		faceMatch = cards[i].getCardInfo().first;
		for (size_t j = i + 1; j < cards.size(); ++j)
		{
			if (faceMatch == cards[j].getCardInfo().first)
			{
				++count;
			}
		}
		if (count == 2)
		{
			twoFace = faceMatch;
			pairFlag = 1;
			return 1;
		}
	}
	return 0;
}

/************************************************
 * RANK CALCULATIONS END
 ************************************************/