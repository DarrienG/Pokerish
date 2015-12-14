  _____      _             _     _     
 |  __ \    | |           (_)   | |    
 | |__) |__ | | _____ _ __ _ ___| |__  
 |  ___/ _ \| |/ / _ \ '__| / __| '_ \ 
 | |  | (_) |   <  __/ |  | \__ \ | | |
 |_|   \___/|_|\_\___|_|  |_|___/_| |_|
                                       
                                       
Pokerish is a Poker-like game, where the user and the dealer are given 5 cards a piece.
From there, both players are allowed to remove up to 3 cards if they'd like from their 
hand, and replace them with new cards from the top of the deck. 

After one swap each, the game then determines which is the better hand, using the same
logic as standard Poker. 

In this game, it is a human player, versus a bot, in an endless battle for the best hand. 

All cards are pulled from the same deck until there aren't enough cards for each player to take 
the maximum number of cards they would need on a given turn, in which case, all cards from the 
discard pile are put back into the deck, and the deck is reshuffled before the following turn.

The bot also cannot cheat. It only performs calculations based off of cards put into the discard
pile. It does so by keeping active track of all cards discarded in both the user, and its own hand.

Why isn't it real Poker? To be honest, I had no idea how to actually play Poker before this 
assignment, so I pieced together as much as I could from online tutorials, and followed the spec 
to a tee, to try and get the game. In doing so, I didn't make a Poker game, but a "Pokerish" game.

_______________


  _    _                 _          _____  _             
 | |  | |               | |        |  __ \| |            
 | |__| | _____      __ | |_ ___   | |__) | | __ _ _   _ 
 |  __  |/ _ \ \ /\ / / | __/ _ \  |  ___/| |/ _` | | | |
 | |  | | (_) \ V  V /  | || (_) | | |    | | (_| | |_| |
 |_|  |_|\___/ \_/\_/    \__\___/  |_|    |_|\__,_|\__, |
                                                    __/ |
                                                   |___/ 

The rules are incredibly simple. Even easier than Poker. At least I'd say that if I had a full
understanding of how Poker was meant to be played.

The user and dealer are each given 5 cards from a standard 52 card deck. The user can (obviously)
see their hand, and they cannot see the dealer's hand. Likewise, the dealer cannot "see" the user's
hand either. 

As soon as the user begins the game, they are given a prompt asking if they'd like to replace any 
of the cards in their hand, with the prompt:

	Player's cards: 
	2 of Clubs ♣
	3 of Spades ♠
	3 of Hearts ❤
	5 of Diamonds ♦
	7 of Clubs ♣

	Player rank: One Pair


	Which cards do you want to replace? 0 to keep card in slot, 1 to change.
	> 

Right off the bat, the user is able to see their own cards, and are immediately given a rank for
their hand. No thinking has to be done at all by the user to decide if they have a pair, or three
of a kind. For people like myself who are still learning what each of the groups mean, this is a 
godsend. 

To replace or keep cards, the user needs to input a string of 1's and 0's.
	-A 1 will remove said card, while a 0 will keep it.
	-The program will only read up to the 5th character
	-Anything that isn't a 1, (including nothing) is a 0

Example input: 
	10011 

This will remove the first card, keep the following two and remove the final two.

Similarly, this input:
	1

Will remove the first element, and keep the following elements. 

In comparison, this input:
	1abcdeghijklmnopqrstuvwxyz

Will do the same thing as the the previous command, by keeping everything except for the first 
element.

_______________


  _____                 _                           _        _   _             
 |_   _|               | |                         | |      | | (_)            
   | |  _ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _| |_ _  ___  _ __  
   | | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` | __| |/ _ \| '_ \ 
  _| |_| | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | |_| | (_) | | | |
 |_____|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\__|_|\___/|_| |_|
                 | |                                                           
                 |_|                                                           

Pokerish is implemented using four classes. Two are outlined in the spec, those being
"deck," and "card." The following two are classes I created myself, those being "hand," and 
"bot."

 __        __   __  
/  `  /\  |__) |  \ 
\__, /~~\ |  \ |__/ 

card is a fairly simple class that contains a constructor that takes two integers, and initializes
a card using the two. One for a face, and for a suit.

As outlined in the spec, there is also a toString method. getPair() works as a way to getFace() 
and getSuit(), while lowering the number of method calls. Super handy.

__

 __   ___  __       
|  \ |__  /  ` |__/ 
|__/ |___ \__, |  \ 
                    
deck is also a fairly simple class as well. Taking no arguments in its constructor, it immediately
creates a vector of cards. The deck is then shuffled, and ready for use. 

Along with a deck being a data member, there is also another data member, lowFlag. This is in 
place to know when to reshuffle the deck. Having it so that the deck reshuffles when there are
no more cards is not a strategy I've implemented, because by putting cards from the discard
pile back into the deck in the middle of a turn, we're messing up the bot's percentages. As such,
the lowFlag is set to true when the deck size hits 23. 10 total cards for the player, 10 total
cards for the dealer, and 3 extra in case as a buffer.

__

                __  
|__|  /\  |\ | |  \ 
|  | /~~\ | \| |__/ 
                    
hand is the first class that has finally become complicated. It was expected at one point or 
another I suppose. 

hand is initialized with a vector of cards. It will take no more than 5 cards, as that is the 
amount of cards that a user has in my Pokerish game. 

In the constructor, it immediately sorts the user's hand, and calculates the hand's rank. 

Explanation of rank:
	-rank is an integer (0-8) denoting the how powerful the given hand is
	-0 is given to hands with a high card, and 8 is given to hands with a straight flush

Rank is calculated using a bit of complex logic, with a sort of "top down" approach, optimized
to make as few calculations as possible.

For example: when calculating a full house, even if the user does not have both a pair and three
of a kind, a flag is set if they do have either a pair, or three of a kind. 

Operators > and < have also been overloaded to allow for easy hand comparison between player and
dealer. Incredibly simple, and effective. 

__

 __   __  ___ 
|__) /  \  |  
|__) \__/  |  

              
And now the most grandiose class of all, the bot that the player faces while they are running the 
program. 

Despite giving the computer enough sentience to face off against the player, this is arguably less
complex than the logic required for ranking hands. Or at least shorter, maybe complex is the wrong
word to use.

When the bot is created, it takes no arguments. It immediately sets the its internal counter of 
the size of the deck to zero, and sets its counts of faces and suits to zero. 
	-All three of these data members are required entirely for probability checks

Of all of the many functions outlined in bot, there are only two the user will ever touch in 
the client code. The first is sculptHand(). sculptHand() returns a string that mimics the same
type of string the user inputs for when they want to replace cards. 

The second is pushToDiscard(). This is necessary so that the user's hand can be pushed to the 
discard pile once a turn is finished, which allows the bot to continue to get accurate 
probabilities for its predictions.

_


Probability Calculations:

Probabilities are calculated on a card by card basis. Using a combination of the probabilities of 
getting x cards with y suits or z faces, we are then able to get the overall probabilities of 
getting a certain hand. 

Before the bot goes about checking probabilities, it checks to see if the dealer's hand has a rank
greater than 3, which is a straight or higher. If it does, there's no point in modifying the 
dealer's hand, as the other player has a 1/254 chance to get a better hand at best (.3%!). 

For reference, whenever I say "gets the probability" I am referring to the two probability 
functions defined that allow the bot to get the probability of finding a suit of a face using the
discard pile. This is imperfect, because it can only check the discard pile, and not the player's
hand, but it's the best we've got.

_

For three of a kind, the bot shoots entirely for a full house, and inadvertantly shoots for four
of a kind at the same time. The bot gets the probability of receiving second copy of either of 
the two cards it has that are not part of its three of a kind, along with getting two of any 
other cardthat is not either of those two. 

From there, the bot finds the highest probability, and outputs a string accordingly to try and 
get what it perceives to be the best hand.

_

For two pairs, the bot will always replace the lone "bad" card it has. There are very few negative
consequences to losing it, and a much higher chance of possibly getting a full house. 

_

For one pair, the bot calculates the probability of getting two pairs, calculates the probability 
of getting three of a kind, and four of a kind (dupDuo()), and calculates the probability of 
getting two pairs (duoDuo()) of any of the "bad cards" it has in its hand.

If the probability of getting two pairs is higher, the bot will "protect" the one card that has 
the highest chance of getting a duplicate of, and then return a string that suggests every other
card that has not been marked/is part of the pair will be replaced.

If the bot decides that getting three of a kind, four of a kind, or a full house, is easier, it 
will simply mark all cards that are not in the pair as bad, and return the accompanying string.

_

For high card/singles, the only thing we can do is go up, and while there are two things we 
calculate for, the is efectively shooting for everything above it.

The bot first calculates the probability of getting a flush with any of the suits in the hand
It returns the highest probability of this, and marks the suit with the highest probability. 

The bot then calculates the probability of getting two pairs, which inadvertantly also checks for
for three of a kind, and a full house. The possibility of getting four of a kind is too high, and
there is (almost always) no point in checking for it.

The bot decides which two cards have the highest likelihood of getting duplicates of, and marks
those cards in a pair.

The bot finally compares the two probabilities calculated, and returns an accompanying string 
upon the best course of action.

__

  _______________________   ____  ___________ __  ____  ___________
 /_  __/ ____/ ___/_  __/  / __ \/ ____/ ___// / / / / /_  __/ ___/
  / / / __/  \__ \ / /    / /_/ / __/  \__ \/ / / / /   / /  \__ \ 
 / / / /___ ___/ // /    / _, _/ /___ ___/ / /_/ / /___/ /  ___/ / 
/_/ /_____//____//_/    /_/ |_/_____//____/\____/_____/_/  /____/  
                                                                   
Because of the nature of me not knowing how to play Poker very well, and the element of random 
chance inherently in Poker, the best we can hope for is a > 50% win rate. 

That being said, my bot was fairly successful in its venture to win every time, and had a 65% 
win rate.

__

After 20 playthroughs:

Results:

Win: 13

Loss: 7

one pair -> one pair 	W
two pairs -> two pairs	L
one pair -> one pair 	W
one pair -> one pair 	L
toc -> toc		W
one pair -> two pairs	W
one pair -> one pair 	W
one pair -> two pairs	W
singles -> singles	L
one pair -> one pair 	L
singles -> toc		W
one pair -> one pair 	L
two pairs -> two pairs	W
one pair -> one pair 	W
singles -> singles 	L
one pair -> toc		W
two pairs -> two pairs 	W
singles -> two pairs 	W
singles -> singles 	L
toc -> foc 		W

toc = three of a kind
foc = four of a kind

For the full log of hands, see battle full_battle_log.txt


    ____                 ______                                          __    
   / __ \__  ______     / ____/___  ____ ___  ____ ___  ____ _____  ____/ /____
  / /_/ / / / / __ \   / /   / __ \/ __ `__ \/ __ `__ \/ __ `/ __ \/ __  / ___/
 / _, _/ /_/ / / / /  / /___/ /_/ / / / / / / / / / / / /_/ / / / / /_/ (__  ) 
/_/ |_|\__,_/_/ /_/   \____/\____/_/ /_/ /_/_/ /_/ /_/\__,_/_/ /_/\__,_/____/  
                                                                               
Compile the program using the included makefile with a quick:

make


The makefile does more than compile the code though, it also autogenerates a shell script that allows the program to silently restart if anything fails, called poker. This short script is necessary to get full functionality out of the program, and as such, the executable itself is hidden upon compilation.

To run the program, use


./poker


If you're feeling adventurous, or just want to mess around, you can run the executable, rather than the shell script. Just know your experience may not be perfect. 
