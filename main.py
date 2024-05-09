from Blackjack import Hand, Shoe

# init
shoe = Shoe(100_000, True)

total = 0
win = 0
loss = 0
draw = 0
player_blackjacks = 0
dealer_blackjacks = 0
while len(shoe) > 52:
    player_hand = Hand()
    dealer_hand = Hand()

    player_hand.add(shoe)
    dealer_hand.add(shoe)
    player_hand.add(shoe)
    dealer_hand.add(shoe)

    dealer_upcard = dealer_hand[0][0]
    dealer_value = dealer_hand.get_value()
    dealer_blackjack = False

    # main round loop
    while True:
        is_active, active_indices = player_hand.is_active()
        if(not is_active): break

        current_index = active_indices[0]
        current_hand = player_hand[current_index]
        while(len(current_hand) < 2):
            player_hand.add(shoe, current_index)

        player_value = player_hand.get_value(current_index)

        #print(f"Dealer - {dealer_upcard}")
        ##print(f"Player - {player_value} - {current_hand}")

        if(dealer_value == 21):
            #print("Dealer blackjack.")
            dealer_blackjack = True
            break

        if(player_value == 21):
            current_hand.active = False
            #print("Player blackjack.")
            continue

        if(player_value > 21):
            current_hand.active = False
            #print("Player bust.")
            continue
        
        result = player_hand.hit(shoe, current_index)
        if(result != -1): player_hand.stand(current_index)

        # choice = ""
        # while True:
        #     choice = input("HIT, STAND, DOUBLE, SPLIT, or SURRENDER? ").lower()
        #     if(choice == "hit"):
        #         result = player_hand.hit(shoe, current_index)
        #         if(result != -1): break
        #     elif(choice == "stand"):
        #         result = player_hand.stand(current_index)
        #         if(result != -1): break
        #     elif(choice == "double"):
        #         result = player_hand.double(shoe, current_index)
        #         if(result != -1): break
        #     elif(choice == "split"):
        #         result = player_hand.split(current_index)
        #         if(result != -1): break
        #     elif(choice == "surrender"):
        #         result = player_hand.surrender(current_index)
        #         if(result != -1): break

        #     #print("Invalid Choice -- ", end="")

    #print("")
    # dealer hitting loop
    all_busted = player_hand.all_busted()
    #print("-----DEALER HITTING-----")
    #print(f"Dealer - {dealer_upcard}")
    #print(f"Dealer - {dealer_value} - {dealer_hand[0]}")
    while(dealer_value < 17):
        if(all_busted): break

        dealer_hand.hit(shoe)
        dealer_value = dealer_hand.get_value()
        #print(f"Dealer - {dealer_value} - {dealer_hand[0]}")
        if(dealer_hand[0].is_bust()):
            #print("Dealer busts.")
            break
    #print("-----DEALER HITTING-----")

    #print("")
    # player winning / losing loop
    for index, hand in enumerate(player_hand.hands):
        value = hand.get_value()
        #print(f"-----HAND {index+1}-----")
        #print(f"Dealer - {dealer_value} - {dealer_hand[0]}")
        #print(f"Hand {index+1} - {value} - {hand}")
        
        if(dealer_blackjack and hand.is_blackjack()):
            #print(f"Push on hand {index+1} with two Blackjacks!")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            draw += 1
            dealer_blackjacks += 1
            player_blackjacks += 1
            continue
        
        if(dealer_blackjack):
            #print(f"Dealer wins on hand {index+1} with a Blackjack!")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            loss += 1
            dealer_blackjacks += 1
            continue

        if(hand.is_blackjack()):
            #print(f"Player wins on hand {index+1} with a Blackjack!")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            win += 1
            player_blackjacks += 1
            continue

        if(hand.is_bust()):
            #print(f"Dealer wins on hand {index+1} from player's bust.")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            loss += 1
            continue

        if(dealer_hand[0].is_bust()):
            #print(f"Player wins on hand {index+1} from dealer's bust.")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            win += 1
            continue

        if(dealer_value == value):
            #print(f"Push on hand {index+1}.")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            draw += 1
            continue

        if(dealer_value < value):
            #print(f"Player wins on hand {index+1}.")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            win += 1
            continue

        if(dealer_value > value):
            #print(f"Dealer wins on hand {index+1}.")
            #print(f"-----HAND {index+1}-----\n")
            total += 1
            loss += 1
            continue

        # should never get here
        total += 1
        #print(f"Unknown Error has occured for hand {index+1}.")
        #print(f"-----HAND {index+1}-----\n")

print(f"TOTAL = {total}\nWINS = {win}; {win/total*100}%\nLOSS = {loss}; {loss/total*100}%")
print(f"\nDRAWS = {draw}; {draw/total*100}%\nDealer BJ = {dealer_blackjacks}; {dealer_blackjacks/total*100}%")
print(f"\nPlayer BJ = {player_blackjacks}; {player_blackjacks/total*100}%")