#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>

using std::vector;

class deck 
{
private:
	bool lowFlag;
	vector<card> cardDeck;

public:
	deck();
	void shuffle();
	card dealCard();
	bool moreCards();
	size_t getDeckSize();
	void toggleEmptyFlag();
};

#endif // DECK_H
