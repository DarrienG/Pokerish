#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h"
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::ostream;
using std::string;

class hand
{
private:
	vector<card> cards;
	int rank;
	int powerCard;
	int threeFace, twoFace, fourFace;
	bool straightFlag, flushFlag, threeFlag, pairFlag;
	const static string rankString[];

	void resetFlags();

	bool hasStraightFlush();
	bool hasFourOfAKind();
	bool hasFullHouse();
	bool pairMod();
	bool hasFlush();
	bool hasStraight();
	bool hasThreeOfAKind();
	bool hasTwoPairs();
	bool duoPairMod();
	bool hasPair();

	int topPower();	
	void sort();
	void swap(int idx1, int idx2);

public:
	hand(vector<card> cards_);
	void replaceCards(string repString, deck &deckCards);
	int calcRank();
	bool operator>(const hand & rhs);
	bool operator<(const hand & rhs);
	string rankToString();
	void pushToDiscard();

	vector<card> getCards();
	int getRank();
	int getTwoFace();
	int getThreeFace();
	int getFourFace();
};

#endif
