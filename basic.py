import sys
import math

# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.
class card():
    def __init__(self, id, instance_id, card_type, cost, attack, defense, abil, my_health, en_health, card_draw):
        self.id = id
        self.instance_id = instance_id
        self.card_type = card_type
        self.cost = cost
        self.attack = attack
        self.defense = defense
        self.abil = abil
        self.my_health = my_health
        self.en_health = en_health
        self.card_draw = card_draw
        self.B = 1 if "B" in abil else 0
        self.C = 1 if "C" in abil else 0
        self.D = 1 if "D" in abil else 0
        self.G = 1 if "G" in abil else 0
        self.L = 1 if "L" in abil else 0
        self.W = 1 if "W" in abil else 0
        self.absum = self.B + self.C + self.D + self.G + self.L + self.W
    
    def set_pos(self,pos):
        self.pos = pos
        
def choose_card(cards):
    if loop%2:
        cards.sort(key=lambda x: (x.card_type, -x.G, -x.absum, -x.attack, -x.cost))
    else:
        cards.sort(key=lambda x:(x.card_type, -x.G, x.cost,-x.attack))
    return cards[0]    

def summon(cards):
    global my_mana
    s = sum(x.G for x in my_desk_cards)
    print("s=",s,file=sys.stderr)
    ans = []
    if s > 1:
        cards.sort(key=lambda x: (x.card_type, x.G, -x.cost,-x.attack, -x.absum))
    else:
        cards.sort(key=lambda x: (x.card_type, -x.G, -x.cost,-x.attack, -x.absum))
    for x in cards:
        if x.cost <= my_mana:
            ans.append(x)
            my_mana = my_mana - x.cost
    return ans       
def use_green(cards):
    global my_mana
    ans = ['','']
    if len(my_desk_cards) > 0:
        for x in cards:
            if x.card_type == 1:
                ans[0] = x.instance_id
                m = -1
                for i in my_desk_cards:
                    if i.defense > m:
                        m = i.defense
                        ans[1] = i.instance_id
                        my_mana = my_mana - x.cost
    return ans                
                    
    
def use_red(cards):
    global my_mana
    ans = ['','']
    if len(en_desk_cards) > 0:
        for x in cards:
            if x.card_type == 2:
                ans[0] = x.instance_id
                m = -1
                for i in en_desk_cards:
                    if i.attack > m:
                        m = i.defense
                        ans[1] = i.instance_id
                        my_mana = my_mana - x.cost
    return ans                
def use_blue(cards):
    ans = ''
    for x in cards:
        if x.card_type == 3:
            return x.instance_id
    return ans
    
my_hand_cards = []
my_desk_cards = []
en_desk_cards = []
my_cards = []
my_items =[]
loop = 30
my_mana = 0
# game loop
while True:
    my_hand_cards = []
    my_items = []
    my_desk_cards = []
    en_desk_cards = []
    draft_cards = []
    draft = True if loop > 0 else False
    my_health, my_mana, my_deck, my_rune = [int(j) for j in input().split()]
    en_health, en_mana, en_deck, en_rune = [int(j) for j in input().split()]
    opponent_hand = int(input())
    card_count = int(input())
    for i in range(card_count):
        card_number, instance_id, location, card_type, cost, attack, defense, abilities, my_health_change, opponent_health_change, card_draw = input().split()
        card_number = int(card_number)
        instance_id = int(instance_id)
        #print(instance_id,file=sys.stderr)
        location = int(location)
        card_type = int(card_type)
        cost = int(cost)
        attack = int(attack)
        defense = int(defense)
        my_health_change = int(my_health_change)
        opponent_health_change = int(opponent_health_change)
        card_draw = int(card_draw)
        temp = card(card_number, instance_id, card_type, cost, attack, defense, abilities, my_health_change, opponent_health_change, card_draw)
    
        if draft:
            temp.set_pos(i)
            draft_cards.append(temp)
        else:
            if location == 0: my_hand_cards.append(temp)
            if location == 1: my_desk_cards.append(temp)
            if location == -1: en_desk_cards.append(temp)
    loop -= 1 
    ans = ''
    if draft:
        temp = choose_card(draft_cards)
        my_cards.append(temp)
        print("PICK",temp.pos)
    else:
        temp = use_blue(my_hand_cards)
        if str(temp) > '':
            ans += "USE " + str(temp) + " -1;"
        temp = use_red(my_hand_cards)
        if str(temp[0]) > '':
            ans += "USE " + str(temp[0]) + " " + str(temp[1]) + " ;"
        temp = summon(my_hand_cards)
        for a in temp:
            ans += "SUMMON " + str(a.instance_id) + " ;"
            if a.C:
                my_hand_cards.append(a)
        temp = use_green(my_hand_cards)
        if str(temp[0]) > '':
            ans += "USE " + str(temp[0]) + " " + str(temp[1]) + " ;"
        my_desk_cards.sort(key = lambda x: (-x.L))      
        for c in my_desk_cards:
            for x in range(len(en_desk_cards)):
                if en_desk_cards[x].G and en_desk_cards[x].defense > 0:
                    ans += "ATTACK " + str(c.instance_id) + " " + str(en_desk_cards[x].instance_id) + ";"
                    if en_desk_cards[x].W: 
                        en_desk_cards[x].W = 0 
                        break
                    if c.L:
                        en_desk_cards[x].defense = 0
                        break
                    en_desk_cards[x].defense -= c.attack
                    break
            ans += "ATTACK " + str(c.instance_id) + " -1;" 
        print (ans)
        
