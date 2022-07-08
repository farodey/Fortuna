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

    def CalculateMC(self, hands, board, dead, numberOfTrials):
        self.PreCalculate(hands, board, dead, numberOfTrials)
        self.CalculateMonteCarlo()
        return self.PostCalculate()

    def PreCalculate(self, hands, board, dead, numberOfTrials, results):
        self.Reset()
        Store(hands, board, dead, numberOfTrials, results)
        CreateHandDistributions(hands)
        if (numberOfTrials == 0)
            EstimatePossibleOutcomes()

    def PostCalculate(self):
        pass

    def Reset(self):
        self.StdDeck_CardMask_RESET(self.m_boardMask)
        StdDeck_CardMask_RESET(m_deadMask)
        StdDeck_CardMask_RESET(m_deadMaskDyn)
        m_numberOfBoardCards = 0
        m_numberOfRangedHands = 0
        m_numberOfSpecificHands = 0
        m_collisions = 0
        m_totalHands = 0
        m_possibleOutcomes = 1L
        m_pResults = NULL
        m_indicatedTrials = 0L
        m_actualTrials = 0L
        memset(m_tiedPlayerIndexes, 0, sizeof(m_tiedPlayerIndexes))
        memset(m_wins, 0, sizeof(m_wins))


    def Store(self, hands, board, dead, trialCount, outResults):

        m_boardMask = CardConverter::TextToPokerEval(board)
        m_deadMask = CardConverter::TextToPokerEval(dead)
        StdDeck_CardMask_OR(m_deadMask, m_deadMask, m_boardMask)

        m_pResults = outResults
        m_indicatedTrials = trialCount

        m_numberOfBoardCards = StdDeck_numCards(m_boardMask)


    def CalculateMonteCarlo(self):
        pass