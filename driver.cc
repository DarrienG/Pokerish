#include "card.h"
#include "hand.h"
#include "deck.h"
#include "bot.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;
using std::cin;

ostream& operator<<(ostream& out, hand &cards);
void part1();
void part2();
void part3();
void part4();
void play();
vector<card> createHandVec(deck &deckCards);

void DEBUGMODE();

int main(int argc, char const *argv[])
{
	//part1();
	// part2();
	// part3();
	// part4();
	play();
	return 0;
}

void part1(){
	deck newDeck;
	for (int i = 0; i < 52; ++i)
	{
		cout << newDeck.dealCard().toString() << endl;
	}
}

void part2(){
	deck newDeck;
	
	hand player(createHandVec(newDeck));
	hand dealer(createHandVec(newDeck));

	cout << "Player's cards: \n" << player;
	cout << "Player rank: " << player.rankToString() << "\n\n";

	cout << "Dealer's cards: \n" << dealer;
	cout << "Dealer rank: " << dealer.rankToString() << "\n\n";		
}

void part3(){
	deck newDeck;
	
	hand player(createHandVec(newDeck));
	hand dealer(createHandVec(newDeck));

	cout << "Player's cards: \n" << player;
	cout << "Player rank: " << player.rankToString() << "\n\n";

	cout << "Dealer's cards: \n" << dealer;
	cout << "Dealer rank: " << dealer.rankToString() << "\n\n";			

	cout << (player > dealer ? "Player's " : "Dealer's ") << "hand is better.\n\n";
}

void part4(){
	deck newDeck;
	string replacement;

	hand player(createHandVec(newDeck));
	hand dealer(createHandVec(newDeck));

	cout << "Player's cards: \n" << player;
	cout << "Player rank: " << player.rankToString() << "\n\n";

	cout << "Which cards do you want to replace? 0 to keep card in slot, 1 to change.\n> ";
	cin >> replacement;

	player.replaceCards(replacement, newDeck);

	cout << "Player's new cards: \n" << player;
	cout << "Player rank: " << player.rankToString() << "\n\n";	

	cout << "Dealer's cards: \n" << dealer;
	cout << "Dealer rank: " << dealer.rankToString() << "\n\n";			


	cout << (player > dealer ? "Player's " : "Dealer's ") << "hand is better.\n\n";	
}

void play(){
	deck newDeck;
	string replacement;
	bool again;
	bot pokerBot;

	do{
		hand player(createHandVec(newDeck));
		hand dealer(createHandVec(newDeck));

		cout << "Player's cards: \n" << player;
		cout << "Player rank: " << player.rankToString() << "\n\n";

		cout << "Which cards do you want to replace? 0 to keep card in slot, 1 to change (max of 3).\n> ";
		cin >> replacement;

		player.replaceCards(replacement, newDeck);

		cout << "Player's new cards: \n" << player;
		cout << "Player rank: " << player.rankToString() << "\n\n";	

		cout << "____\n\n";

		/*
		* Uncomment to see dealer's cards before switching
		*
		* cout << "Dealer's cards: \n" << dealer;
		* cout << "Dealer rank: " << dealer.rankToString() << "\n\n";			
		*/

		// USE BOT TO UPGRADE DEALER'S HAND
		dealer.replaceCards(pokerBot.sculptHand(newDeck.getDeckSize(), dealer), newDeck);

		cout << "Dealer's cards: \n" << dealer;
		cout << "Dealer rank: " << dealer.rankToString() << "\n\n";			

		cout << (player > dealer ? "Player's " : "Dealer's ") << "hand is better.\n\n";	

		pokerBot.pushToDiscard(player);

		cout << "Want to play another round? 1 = yes, 0 = no\n> ";
		cin >> again;

		while(cin.fail()){
			cout << "Please only enter a 1 or a 0.\n> ";
			cin.clear();
			cin.ignore(4294967296, '\n');
			cin >> again;
		}
		// This ensures that the bot's calculations, under no circumstances, will ever be interrupted by a 
		// deck shuffle
		if(newDeck.getDeckSize() < 23){
			newDeck.toggleEmptyFlag();
		}

	}while(again);
}

void DEBUGMODE(){
	deck newDeck;
	string replacement;
	bot pokerBot;

	int i = 30;
		card c1(1, 1);
		card c2(1, 2);
		card c3(3, 3);
		card c4(3, 2);
		card c5(6, 1);
		vector<card> testVec;;
		testVec.push_back(c1);
		testVec.push_back(c2);
		testVec.push_back(c3);
		testVec.push_back(c4);
		testVec.push_back(c5);
		hand dealer(testVec);
		while (i > 0){

		cout << "Dealer's cards: \n" << dealer;
		cout << "Dealer rank: " << dealer.rankToString() << "\n\n";			

		replacement = pokerBot.sculptHand(newDeck.getDeckSize(), dealer);
		
		dealer.replaceCards(replacement, newDeck);

		cout << "Dealer's new cards: \n" << dealer;
		cout << "Dealer rank: " << dealer.rankToString() << "\n\n";	
		--i;
	}
}

ostream& operator<<(ostream& out, hand &cards){
	for (size_t i = 0; i < cards.getCards().size(); ++i)
	{
		out << cards.getCards()[i].toString() << "\n";
	}

	return out << "\n";
}

vector<card> createHandVec(deck &deckCards){
	vector<card> tmpVec;
	for (int i = 0; i < 5; ++i)
	{
		tmpVec.push_back(deckCards.dealCard());
	}
	return tmpVec;
}
