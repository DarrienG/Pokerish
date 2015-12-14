#include "bot.h"

#include <iostream>
using std::cout;
using std::endl;

inline double Factorial(int x) {
  return (x == 1 ? x : x * Factorial(x - 1));
}

bot::bot(){
	deckSize = 0;
	for (int i = 0; i < 13; ++i)
	{
		faceCount[i] = 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		suitCount[i] = 0;
	}
}

string bot::sculptHand(size_t deckSize_, hand &cards){
	pushToDiscard(cards);

	if (deckSize_ > deckSize)
	{
		discardTracker.clear();
		resetCounts();
	}
	deckSize = deckSize_;

	// We have a strong enough hand that there is not reason to modify it
	// User has a 1/254 (0.393700787%) chance to beat at best. Safe to say we don't care.
	if (cards.getRank() > 3)
	{
		return "00000";
	}

	// With everything 3 or below, we want to shoot for the upper ranks
	// Program will calculate the approximate possibility of hitting any of the 
	// upper ranks, and choose the one with the highest possibility

	string action;
	vector<double> actionChoice; 

	// Three of a kind
	// We need to get the indices of the cards that make up 3 of a kind, so we can
	// ignore them
	if (cards.getRank() == 3)
	{
		action = ignoreIndices(cards.getThreeFace(), cards);
		houseTriage(cards);
		return threeHouseAction(action);
	}

	if (cards.getRank() == 2)
	{	
		// It is a given that the second position will always have part of the first pair into it,
		// because pairs are contiguous, and there are only 5 spots for cards.
		// As we evaluate the higher of the two pairs to store in twoFace, and we know there is a 
		// second pair, we can hardcode that location in, and call it a day
		action = ignoreIndices(cards.getCards()[3].getCardInfo().first, cards.getCards()[1].getCardInfo().first, cards);
		return action;
	}

	if (cards.getRank() == 1)
	{
		action = ignoreIndices(cards.getTwoFace(), cards);	
		actionChoice.push_back(dupDuo(cards));
		
		// Get probability of triple, 3/prob of getting 3rd card
		// Do not modify string at all if happens -> inadvertantly checks for quads and full house too
		
		actionChoice.push_back(duoDuo(cards));
		
		// Get probability of second pair max(1/prob each bad card)
		// Make best probablity in badIndices become good

		// TODO: DEFINE dupAction and duoAction
		return actionChoice[0] > actionChoice[1] ? dupAction(action) : duoAction(action);
	}

	// rank == 0, need to:
	// 1. Check number of like suits, and see probability of a flush
	// 2. Check if numbers are incrementing at all, and see probability of a straight
	// 3. Check probability of each card for getting 3 of a kind, pick highest for calculations
	// 4. Check probability of getting a pair with each of the values, average together two percentages 
	else{
		// Forcing all values to 1, every index becomes a bad index
		// We now must modify string so that we keep all advantageous cards
		
		action = "11111";

		actionChoice.push_back(singleFlush(cards));
		actionChoice.push_back(singleDPair(cards));

		return actionChoice[0] > actionChoice[1] ? singleFlushAction(action, cards) : singleDPairAction(action);

	}

	return action;
}

// Returns a string of 0's and 1's to show which cards to ignore. 
// A spot with a 0 means ignore, which a spot with a 1 means we will take action on it
// Structure of replaceCards() mimicked so we can easily pass the final modified string into it
string bot::ignoreIndices(int face, hand &cards){
	string ignoreS = "";
	for (size_t i = 0; i < cards.getCards().size(); ++i)
	{
		if (cards.getCards()[i].getCardInfo().first == face)
		{
			ignoreS.append("0");
			goodIndices.push_back(i);
		}
		else{
			ignoreS.append("1");
			badIndices.push_back(i);
		}
	}
	return ignoreS;
}

// Modified ignoreIndices primarily made for two pairsm but can be used elsewhere
string bot::ignoreIndices(int face1, int face2, hand &cards){
	string ignoreS = "";
	for (size_t i = 0; i < cards.getCards().size(); ++i)
	{
		if (cards.getCards()[i].getCardInfo().first == face1 || cards.getCards()[i].getCardInfo().first == face2)
		{
			ignoreS.append("0");
			goodIndices.push_back(i);
		}
		else{
			ignoreS.append("1");
			badIndices.push_back(i);
		}
	}
	return ignoreS;
}

void bot::pushToDiscard(hand &cards){
	for (size_t i = 0; i < cards.getCards().size(); ++i)
	{
		discardTracker.push_back(cards.getCards()[i]);
		faceCount[cards.getCards()[i].getCardInfo().first] += 1;
		faceCount[cards.getCards()[i].getCardInfo().second] += 1;
	}
}

// Deck has been reset, so we need to reset the values in our value counter
void bot::resetCounts(){
	for (int i = 0; i < 13; ++i)
	{
		faceCount[i] = 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		suitCount[i] = 0;
	}	
}

/************************************
 * SINGLE CARD PROBABILITY BEGIN
 ************************************/

double bot::probabilityFace(int face, int times){
	if (times > 4 - faceCount[face])
	{
		return 0;
	}

	double prob = (Factorial(deckSize)/(Factorial(times) * Factorial(deckSize - times)));

	return 1/prob;
}

double bot::probabilitySuit(int suit, int times){
	if (times > 13 - suitCount[suit])
	{
		return 0;
	}

	double prob = (Factorial(deckSize)/(Factorial(times) * Factorial(deckSize - times)));

	return 1/prob;
}

/************************************
 * SINGLE CARD PROBABILITY END
 ************************************/

/************************************
 * COMBINATION PROBABILITY BEGIN
 ************************************/

double bot::houseTriage(hand &cards){
	double prob1, prob2, prob3, max = 0, tmp = 0;
	// Feels like I'm coding in Java again
	prob1 = probabilityFace(cards.getCards()[badIndices[0]].getCardInfo().first);
	prob2 = probabilityFace(cards.getCards()[badIndices[1]].getCardInfo().first);

	for (int i = 0; i < 13; ++i)
	{
		if (i == badIndices[0] || i == badIndices[1])
		{
			++i;
		}
		tmp = probabilityFace(i, 2);
		if (max < tmp)
		{	
			max = tmp;
		}
	}

	prob3 = max;

	max = prob1 > prob2 ? prob1 : prob2;
	max = max > prob3 ? max : prob3;

	// markedIndexH is a way of deciding how we will evaluate for our Full House
	// the markedIndexH decides which index is OK to keep from badIndices 
	// We can't replace it immediately, because we don't yet know if Full House is the most 
	// advantageous move

	if (max == prob1)
	{
		markedIndexH = 0;
	}
	if (max == prob2)
	{
		markedIndexH = 1;
	}
	else{
		markedIndexH = -1;
	}

	return max;
}

// To see if we can get a duplicate of one of the cards we already have
double bot::dupDuo(hand &cards){
	return 3 * probabilityFace(cards.getCards()[goodIndices[0]].getCardInfo().first);
}

double bot::duoDuo(hand &cards){
	double prob1, prob2, prob3;
	prob1 = probabilityFace(cards.getCards()[badIndices[0]].getCardInfo().first);
	prob2 = probabilityFace(cards.getCards()[badIndices[1]].getCardInfo().first);
	prob3 = probabilityFace(cards.getCards()[badIndices[2]].getCardInfo().first);

	prob1 > prob2 ? markedIndexD = 0 : markedIndexD = 1;
	prob3 > prob2 ? markedIndexD = 2 : markedIndexD = 1;

	if (markedIndexD == 0)
	{
		return 2 * prob1;
	}
	else if (markedIndexD == 1)
	{
		return 2 * prob2;
	}
	else{
		return 2 * prob3;
	}

}

double bot::singleFlush(hand &cards){

	// Spade, Hearts, Clubs, Diamonds
	int numSuits[] = {0, 0, 0, 0};
	vector<double> probs;
	double max;

	probs.reserve(10);

	// Counting up the number of each suit in the user's hand
	for (size_t i = 0; i < 5; ++i)
	{
		numSuits[cards.getCards()[i].getCardInfo().second] += 1;
	}

	// Getting the probability of retrieving 5 - the number of suits we already have in the hand
	for (int i = 0; i < 4; ++i)
	{
		probs.push_back(probabilitySuit(i, 5 - numSuits[i]));
	}	

	// Here we're finding the highest probability, and also marking it so we can work with it later
	probs[0] > probs[1] ? max = probs[0], markedSuit = 0: max = probs[1], markedSuit = 1;
	max > probs[2] ? max = max : max = probs[2], markedSuit = 2;
	max > probs[3] ? max = max : max = probs[3], markedSuit = 3;

	return max;
}

// Possibility of getting two pairs
// Will make two bad indices good indices
// Inadvertantly checks for one pair, three of a kind, and full house
double bot::singleDPair(hand &cards){
	vector<double> probs;
	double max = 0, max2 = 0;

	// Get probability of finding each card
	for (size_t i = 0; i < 5; ++i)
	{
		probs.push_back(probabilityFace(cards.getCards()[i].getCardInfo().first));
	}

	// Getting the two greatest probabilities in the vector, and marking them
	for (int i = 0; i < 5; ++i)
	{
		max < probs[i] ? max = probs[i], keepPair.first = i : max = max;
		max2 < probs[i] && probs[i] < max ? max2 = probs[i], keepPair.first = i : max2 = max2;
	}

	// Finding the probability of getting both of those cards
	return max * max2;
}

/************************************
 * COMBINATION PROBABILITY END
 ************************************/

/************************************
 * COMBINATION ACTION BEGIN
 ************************************/

string bot::threeHouseAction(string tmpString){
	if (markedIndexH == -1)
	{
		return tmpString;
	}
	tmpString.replace(badIndices[markedIndexH], 1, "0");
	return tmpString;
}

// Made for the sake of balancing functions. Action is already as it should
// be though, so we're only returning the value
string bot::dupAction(string tmpString){
	return tmpString;
}

string bot::duoAction(string tmpString){
	tmpString.replace(badIndices[markedIndexD], 1, "0"); 
	return tmpString;

}

string bot::singleFlushAction(string tmpString, hand &cards){
	for (int i = 0; i < 5; ++i)
	{
		if (cards.getCards()[i].getCardInfo().second == markedSuit){ tmpString.replace(i, 1, "0"); }
	}
	return tmpString;
}

string bot::singleDPairAction(string tmpString){
	tmpString.replace(keepPair.first, 1, "0");
	tmpString.replace(keepPair.second, 1, "0");
	return tmpString;
}

/************************************
 * COMBINATION ACTION END
 ************************************/
