#include <bits/stdc++.h>

#define all(x) begin(x),end(x)

using namespace std;

struct Card {
    int cardNumber;
    int id;
    int location;
    int cardType;
    int mana;
    int attack;
    int defense;
    bool breakthrough = 0;
    bool charge = 0;
    bool guard = 0;
};

struct Player {
    int health;
    int mana;
    int deck;
    int rune;
};

void initCard(Card &card, int cardNumber, int instanceId, int location, int cardType, int cost, int attack, int defense, string abilities) {
    card.cardNumber = cardNumber;
    card.id = instanceId;
    card.location = location;
    card.cardType = cardType;
    card.mana = cost;
    card.attack = attack;
    card.defense = defense;
    for(int i = 0; i < 6; i++) {
        if(abilities[i] == 'B') card.breakthrough = 1;
        if(abilities[i] == 'C') card.charge = 1;
        if(abilities[i] == 'G') card.guard = 1;
    }
}

void initPlayer(Player &player, int playerHealth, int playerMana, int playerDeck, int playerRune) {
    player.health = playerHealth;
    player.mana = playerMana;
    player.deck = playerDeck;
    player.rune = playerRune;
}

int getCard(vector<Card> cards, int iteration) {
    const double coeffAttack = 6;
    const double coeffDefense = 5;
    const double coeffMana = 12;
    const double coeffGuard = ((iteration % 2) ? 100 : 0);
    int draftNumber = 0;
    double bestVal = cards[0].attack * coeffAttack + cards[0].defense * coeffDefense - coeffMana * cards[0].mana;
    if(cards[0].guard) {
        bestVal += coeffGuard;   
    }
    if(cards[0].cardType > 0) {
        bestVal -= 200;
    }
    cerr << cards[0].cardType << endl;
    cerr << bestVal << " ";
    for(int i = 1; i < cards.size(); i++) {
        double tempVal = cards[i].attack * coeffAttack + cards[i].defense * coeffDefense - coeffMana * cards[i].mana;
        if(cards[i].guard) {
            tempVal += coeffGuard;   
        }
        if(cards[i].cardType > 0) {
            tempVal -= 200;
        }
        cerr << tempVal << " ";
        if(tempVal > bestVal) {
            draftNumber = i;
            bestVal = tempVal;
        }
    }
    cerr << endl;
    return draftNumber;
}

bool byMana(Card a, Card b) {
    return a.mana < b.mana;   
}

bool byAttack(Card a, Card b) {
    return a.attack < b.attack;   
}

bool summon(vector<Card> cards, vector<Card> guards, vector<Card> hand, Player me, Player he) {
    sort(all(guards), byMana);
    sort(all(hand), byMana);
    int cur = 0;
    bool isFirst = 1;
    while(cur < guards.size() && cur < 2) {
        if(me.mana - guards[cur].mana < 0) {
            break;
        }
        if(!isFirst) cout << "; ";
        isFirst = 0;
        cout << "SUMMON " << guards[cur].id;
        me.mana -= guards[cur].mana;
        cur++;
    }
    cur = 0;
    while(cur < hand.size()) {
        if(me.mana - hand[cur].mana < 0) {
            break;
        }
        if(!isFirst) cout << "; ";
        isFirst = 0;
        cout << "SUMMON " << hand[cur].id;
        me.mana -= hand[cur].mana;
        cur++;
    }
    return isFirst;
}

bool attack(vector<Card> myCards, vector<Card> cards, Player me, Player he, bool flag) {
    bool isFirst = flag;
    vector<Card> enemyGuards;
    vector<Card> enemy;
    int maxEnemyAttack = 0;
    for(int i = 0; i < cards.size(); i++) {
        if(cards[i].location == -1 && cards[i].guard) {
            enemyGuards.push_back(cards[i]);
        }
        //if(cards[i].location == -1) {
        //   maxEnemyAttack = max(maxEnemyAttack, cards[i].attack);
        //    enemy.push_back(cards[i]);
    //    }
    }
    //sort(all(enemy), byAttack);
    
    sort(all(enemyGuards), byAttack);
    reverse(all(enemyGuards));
    int pos = 0;
    vector<Card> onTable;
    for(int i = 0; i < myCards.size(); i++) {
        if(myCards[i].location == 1) {
            onTable.push_back(myCards[i]);
        }
    }
    sort(all(onTable), byAttack);
    //if(maxEnemyAttack > 5) {
    //    if(!isFirst) cout << "; ";
    //    isFirst = 0;
    //    cout << "ATTACK " << onTable[0].id << " " << enemy[enemy.size() - 1].id;
    //}
    for(int i = 0; i < onTable.size(); i++) {
        if(pos < enemyGuards.size()) {   
            enemyGuards[pos].defense -= onTable[i].attack;
            if(!isFirst) cout << "; ";
            isFirst = 0;
            cout << "ATTACK " << onTable[i].id << " " << enemyGuards[pos].id;
            if(enemyGuards[pos].defense <= 0) {
                pos++;   
            }
        }
        else {
            if(!isFirst) cout << "; ";
            isFirst = 0;
            cout << "ATTACK " << onTable[i].id << " -1";
        }
    }
    return isFirst;
}

int main()
{
    int pickUpTime = 0;
    
    while (1) {
        Player me;
        Player he;
        int playerHealth;
        int playerMana;
        int playerDeck;
        int playerRune;
        cin >> playerHealth >> playerMana >> playerDeck >> playerRune; cin.ignore();
        initPlayer(me, playerHealth, playerMana, playerDeck, playerRune);
        cin >> playerHealth >> playerMana >> playerDeck >> playerRune; cin.ignore();
        initPlayer(he, playerHealth, playerMana, playerDeck, playerRune);
        int opponentHand;
        cin >> opponentHand; cin.ignore();
        int cardCount;
        cin >> cardCount; cin.ignore();
        vector<Card> cards(cardCount);
        
        for (int i = 0; i < cardCount; i++) {
            int cardNumber;
            int instanceId;
            int location;
            int cardType;
            int cost;
            int attack;
            int defense;
            string abilities;
            int myHealthChange;
            int opponentHealthChange;
            int cardDraw;
            cin >> cardNumber >> instanceId >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw; cin.ignore();
            Card card;
            initCard(card, cardNumber, instanceId, location, cardType, cost, attack, defense, abilities);
            cards[i] = card;
        }
        
        if(pickUpTime < 30) {
            int cardNumber = getCard(cards, pickUpTime);
            cout << "PICK " << cardNumber << endl;
        }
        else {
            vector<Card> myCards;
            vector<Card> guards;
            vector<Card> hand;
            for(int i = 0; i < cards.size(); i++) {
                if(cards[i].location != -1) {
                    myCards.push_back(cards[i]);
                }
                if(cards[i].guard && cards[i].location == 0) {
                    guards.push_back(cards[i]);
                }
                if(cards[i].location == 0 && cards[i].cardType == 0) {
                    hand.push_back(cards[i]);
                }
            }
            bool flag = summon(myCards, guards, hand, me, he);
            //if(flag) cout << "PASS";
            //cout << endl;
            //cerr << flag << endl;
            bool flag2 = attack(myCards, cards, me, he, flag);
            if(flag2) {
                cout << "PASS";   
            }
            cout << " For Lorderon";
            cout << endl;
        }
        
        pickUpTime++;
    }
}
