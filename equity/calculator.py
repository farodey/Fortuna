class HoldemCalculator:

    def __int__(self):
        self.m_boardMask = 0
        self.m_deadMask = 0
        self.m_deadMaskDyn = 0

        self.m_numberOfBoardCards = 0
        self.m_numberOfRangedHands = 0
        self.m_numberOfSpecificHands = 0

        self.m_totalHands = 0
        self.m_collisions = 0
        self.m_pResults = 0
        self.m_dists = []

    def create_list_hand(self, hands):
        list_hands = []
        temp_hands = hands
        while (i := temp_hands.find('|')) != -1:
            list_hands.append(temp_hands[0:i])
            temp_hands = temp_hands[i + 1::]
        return list_hands

    def calculate_mc(self, hands, board, dead, number_of_trials):
        self.create_list_hand(hands)
