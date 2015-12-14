#ifndef CARD_H
#define CARD_H

#include <array>
#include <string>
#include <utility>

using std::string;
using std::pair;

class card
{
friend class deck;

private:
	// face, suit
	pair<int, int> infoPair;
	static const int suitEnum[];
	static const int faceEnum[];	
	static const string suitTable[];
	static const string faceTable[];

public:
	card(int face_, int suit_);
	string toString(bool newLine = 0);
	pair<int, int> getCardInfo();

};	

#endif // CARD_H