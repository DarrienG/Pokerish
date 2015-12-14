#include "deck.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

deck::deck(){
	for (int i = 0; i < 13; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			card tmpCard(i,j);
			cardDeck.push_back(tmpCard);
		}
	}

	shuffle();
	lowFlag = 0;
}	

void deck::shuffle(){
	int randNum;

	srand(time(0));
	for(int i = 0; i < 52; ++i){
		// We have C++ 14, and still no C++ way to generate a random number? ಠ_ಠ
		randNum = rand() % 52;
		card tmpCard = cardDeck[i];
		cardDeck[i] = cardDeck[randNum];
		cardDeck[randNum] = tmpCard;
	}
}

// Takes card at the end of cardDeck, removes it from the vector, and returns it
card deck::dealCard(){
	if (!moreCards() || lowFlag)
	{
		cout << "Deck low size. Adding cards, and reshuffling.\n" << endl;
		cardDeck.clear();
		for (int i = 0; i < 13; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				card tmpCard(i,j);
				cardDeck.push_back(tmpCard);
			}
		}
		shuffle();
		lowFlag = 0;
	}

	// This makes it so we're moving less memory around, as we're only deleting the final element
	card topCard = *(cardDeck.end() - 1);
	cardDeck.erase(cardDeck.end() - 1);
	return topCard;
}

// This should really be called deckEmpty()
// dang spec *grumble grumble*
bool deck::moreCards(){
	if (cardDeck.size() > 0)
	{
		return true;
	}
	return false;
}

size_t deck::getDeckSize(){
	return cardDeck.size();
}

void deck::toggleEmptyFlag(){
	lowFlag = 1;
}