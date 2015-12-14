#ifndef BOT_H
#define BOT_H 

#include "card.h"
#include "deck.h"
#include "hand.h"
#include <vector>
#include <string>
#include <utility>

using std::vector;
using std::string;

class bot
{
private:
	size_t deckSize;
	int markedIndexH;
	int markedIndexD;
	int markedSuit;
	vector<card> discardTracker;

	int faceCount[13];
	int suitCount[4];
	vector<int> badIndices;
	vector<int> goodIndices;
	pair<int, int> keepPair;

	string ignoreIndices(int face, hand &cards);
	string ignoreIndices(int face1, int face2, hand &cards);
	vector<int> convertToIndices(string inputString);
	double probabilityFace(int face, int times = 1);
	double probabilitySuit(int suit, int times = 1);
	void resetCounts();

	double houseTriage(hand &cards);
	string threeHouseAction(string tmpString);
	string threeFlushAction(string tmpString);

	double dupDuo(hand &cards);
	double duoDuo(hand &cards);
	string dupAction(string tmpString);
	string duoAction(string tmpString);

	double singleFlush(hand &cards);
	double singleDPair(hand &cards);
	string singleFlushAction(string tmpString, hand &cards);
	string singleDPairAction(string tmpString);


public:
	bot();
	string sculptHand(size_t deckSize, hand &cards);
	void pushToDiscard(hand &cards);
};

#endif