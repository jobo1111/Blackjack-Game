#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <ctime>
#include <chrono>


using namespace std;
// create rank and suit for all cards
enum rank {ACE=1,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING};
enum  type {CLUBS, DIAMONDS, HEARTS, SPADES};

class Card{
public:
    Card(int num, type suit);
    int getValue();
    string displayCard();
    ~Card();


protected:
    int rank;
    type suit;
};

// constructor
Card::Card(int num, type suit){
    this->rank = num;
    this->suit = suit;
}
Card::~Card(){}

// return the value of a card, if it is a face card then return 10
int Card::getValue(){
    int num;
    return (this->rank > 10 ? 10 : this->rank);
}

string Card::displayCard(){
    // convert the card to a string
    // although this goes against the instructions, I found it more useful to return a string
    string num;
    string su;
    switch (this->rank){
        case JACK:
            num = "J";
            break;
        case QUEEN:
            num = "Q";
            break;
        case KING:
            num = "K";
            break;
        default:
            num = to_string(this->rank);
            break;
    }
    switch (this->suit)
    {
    case HEARTS:
        su = "H";
        break;
    case DIAMONDS:
        su = "D";
        break;
    case CLUBS:
        su = "C";
        break;
    case SPADES:
        su = "S";
        break;
    default:
        break;
    }
    return num + su;
}

class Hand{
public:
    Hand();
    ~Hand();
    void add(Card card);
    void clear();
    int getTotal(bool final);
    string getHand();
protected:
    vector<Card> hand;
};
Hand::Hand(){
    // keep the cards in a vector as the memory is dynamic
    this->hand = vector<Card>() ;
}

Hand::~Hand(){}
void Hand::add(Card card){
    // add card to the back of the deck
    (this->hand).push_back(card);
}
void Hand::clear(){
    // remove all cards from a hand
    (this->hand).clear();
}
int Hand::getTotal(bool final){
    // sum all the cards of the hand together
    int sum = 0;
    int val = 0;
    int ones = 0;
    for (int i=0;i<(this->hand).size();i++){
        val = (this->hand).at(i).getValue();
        val == 1 ? ones++ : ones;
        sum += val;
    }
    if (sum == 11 && ones >= 1) {
        sum += 10;
    }
    // check if it wants the highest sum possible
    if (final && ones >= 1 && sum <= 10){
        sum += 10;
    }
    
    return sum;
}
string Hand::getHand(){
    // returns a string in the form *card* *card* *card*
    string cards;
    for (int i=0;i<(this->hand.size());i++){
        cards += this->hand.at(i).displayCard() + " ";
    }
    return cards;
}
class Deck{
public:
    Deck();
    ~Deck();
    void populate();
    void shuffle();
    void deal(Hand &player);
protected:
    vector<Card> deck;
};
Deck::Deck(){
    // hold the cards in a vector
    this->deck = vector<Card>();
}
Deck::~Deck(){}
void Deck::populate(){
    // iterate through each number and suit and add all 52 cards to the deck
    this->deck.clear();

    for (int i=1;i<14;i++){
        for (int j=0;j<4;j++){
            switch (j)
            {
            case 0:
                (this->deck).push_back(Card(i, CLUBS));
                break;
            case 1:
                (this->deck).push_back(Card(i, DIAMONDS));
                break;
            case 2:
                (this->deck).push_back(Card(i, HEARTS));
                break;
            case 3:
                (this->deck).push_back(Card(i, SPADES));
                break;
            default:
                break;
            }
        }
    }
}

void Deck::shuffle(){
    // set the seed based on the time
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    // shuffle the deck using the shuffle method and the random engine
    std::shuffle((this->deck).begin(), (this->deck).end(), default_random_engine(seed));
}

void Deck::deal(Hand &player){
    // add the last card of the deck to the hand then remove that card from the deck
    Card card = (this->deck).back();
    player.add(card);
    (this->deck).pop_back();
}

class AbstractPlayer{
public:
    AbstractPlayer();
    ~AbstractPlayer();
    virtual bool isDrawing()  = 0;
    bool isBusted();
    void drawing(Deck &deck);
    string getCards();
    int getScore();
    void reset();
protected:
    Hand hand;
};
AbstractPlayer::AbstractPlayer(){
    // set this->hand to an object of Hand
    this->hand = Hand();
}
AbstractPlayer::~AbstractPlayer(){}
bool AbstractPlayer::isBusted(){
    // check if the player went over 21 points
    return this->hand.getTotal(false) > 21;
}
void AbstractPlayer::drawing(Deck &deck){
    // deal a card to the specified hand
    deck.deal(this->hand);
}
string AbstractPlayer::getCards(){
    // returns a string in the form *card* *card* [value of hand]
    return this->hand.getHand() + " " + "[" + to_string(this->hand.getTotal(false)) + "]";
}
int AbstractPlayer::getScore(){
    // returns the score of the hand
    return this->hand.getTotal(true);
}
void AbstractPlayer::reset(){
    // resets the hand for future games
    this->hand.clear();
}
class HumanPlayer: public AbstractPlayer{
public:
    HumanPlayer();
    ~HumanPlayer();
    bool isDrawing();

};
HumanPlayer::HumanPlayer(){}
HumanPlayer::~HumanPlayer(){}
bool HumanPlayer::isDrawing(){
    // ask user if they would like to draw again
    // return true if they want to 
    char answer;
    cout << "Would you like to draw? (y/n)" << endl;
    cin >> answer;
    return (answer == 'y' ? true : false);
}

class ComputerPlayer: public AbstractPlayer{
public:
    ComputerPlayer();
    ~ComputerPlayer();
    bool isDrawing();
};
ComputerPlayer::ComputerPlayer(){}
ComputerPlayer::~ComputerPlayer(){}
bool ComputerPlayer::isDrawing(){
    // only draw again if the total of the hand is less than or equal to 16
    return (this->hand.getTotal(false) <= 16);
}

class BlackJackGame{
public:
    BlackJackGame();
    ~BlackJackGame();
    bool play(string name);
protected:
    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer joe;
};
BlackJackGame::BlackJackGame(){
    this->m_casino = ComputerPlayer();
    this->joe = HumanPlayer();
    this->m_deck = Deck();
}
BlackJackGame::~BlackJackGame(){}

bool BlackJackGame::play(string name){
    int win = 0;
    // reset both players so their hands are empty
    this->joe.reset();
    this->m_casino.reset();

    // populate the deck and shuffle it
    this->m_deck.populate();
    this->m_deck.shuffle();

    // deal two cards to player and one to dealer
    this->joe.drawing(this->m_deck);
    this->joe.drawing(this->m_deck);
    this->m_casino.drawing(this->m_deck);

    // print out the cards that the player and casino have
    cout << name << ": " << this->joe.getCards() << endl;
    cout << "Casino: " << this->m_casino.getCards() << endl;

    bool draw = false;
    // do while loop for human player
    do{
        // check if the user wants to draw a card
        // if they do, draw a card 
        draw = this->joe.isDrawing();
        if (draw){
            this->joe.drawing(this->m_deck);
        }
        // check if the user went over 21
        if (this->joe.isBusted()){
            cout << name << ": " << this->joe.getCards() << endl;
            cout << name << " Busts" << endl;
            break;
            
        }
        // print out the users hand and their score
        cout << name << ": "  << this->joe.getCards() << endl;
    }
    while (draw);

    
    // check to see if the user went over, if they did then they lose automatically
    if (! this->joe.isBusted()){

        // do while loop for casino
        do{
            // check if the casino can draw a card
            // if they can, then the casino draws a card
            draw = this->m_casino.isDrawing();
            if (draw){
                this->m_casino.drawing(this->m_deck);
            }else{
                break;
            }
            // check if the casino went over 21
            if (this->m_casino.isBusted()){
                cout << "Casino: " << this->m_casino.getCards() << endl;
                cout << "Casino Busts" << endl;
                break;
            }
            // print out the casinos hand and score
            cout << "Casino: " << this->m_casino.getCards() << endl;
            
        }
        while (draw);

        // check to see who wins
        if (this->m_casino.isBusted()){
            cout << "You win!!!!" << endl;
            win = 2;
        }else{
            if (this->joe.getScore() > this->m_casino.getScore()){
                win = 2;
                if (this->joe.getScore() == 21){
                    cout << "Winner winner chicken dinner!" << endl;
                }else{
                    cout << "You win!!!!" << endl;
                }
            }else if (this->joe.getScore() == this->m_casino.getScore()){
                cout << "Push: No one wins." << endl;
                win = 1;
            }else{
                cout << "Casino wins" << endl;
                win = 0;
            }
        }
    }else{
        // since the user went over the casino wins
        cout << "Casino wins :(" << endl;
        win = 0;
    }
    return win;
}

int main(){
    string name;
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    cout << "Please enter your name: ";
    cin >> name;
    cout << endl << endl;
    cout << "You have 100 tokens" << endl;
    BlackJackGame game;
    
    bool playAgain = true;
    bool outOfPoints = false;
    char answer = 'y';
    int points = 100;
    int wager = 0;
    int result;
    
    while (playAgain){
        cout << "Please enter a wager" << endl;
        cin >> wager;
        while (wager > points){
            cout << "Invalid wager, select a smaller number" << endl;
            cin >> wager;
        }
        result = game.play(name);
        if (result == 2){
            points = points + wager;
        }else if (result == 0){
            points = points - wager;
            if (points <= 0){
                outOfPoints = true;
            }
        }
        if (outOfPoints){
            break;
        }

        cout << "You now have " << points << " points" << endl;
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout << "Game over!";
    return 0;

}

