#pragma once

// Open Raise
char OR_MP2[] = "55+, ATs+, KTs+, QTs+, J9s+, T9s, 98s, 87s, 76s, 65s, AJo+, KQo";
char OR_MP3[] = "22+, A8s+, A5s-A4s, KTs+, QTs+, J9s+, T8s+, 97s+, 86s+, 76s, 65s, 54s, ATo+, KQo";
char OR_CO[] = "22+, A2s+, K6s+, Q7s+, J7s+, T7s+, 97s+, 86s+, 75s+, 64s+, 54s, ATo+, KJo+, QJo, JTo, T9o";
char OR_BU[] = "22+, A2s+, K2s+, Q2s+, J4s+, T6s+, 96s+, 85s+, 74s+, 64s+, 54s, A2o+, K7o+, Q8o+, J9o+, T8o+, 98o, 87o";
char OR_SB[] = "22+, A2s+, K2s+, Q4s+, J7s+, T7s+, 97s+, 86s+, 75s+, 64s+, 54s, A7o+, K9o+, Q9o+, J9o+, T9o, 98o";

//
// Cold - Call
//
// vs MP2
char CC_MP3vsMP2[] = "QQ-77, AJs+, KQs, JTs, T9s, 98s, AQo+";
char CC_COvsMP2[]  = "QQ-55, ATs+, KQs, JTs, T9s, 98s, 87s, AQo+";
char CC_BUvsMP2[]  = "QQ-33, ATs+, KQs, JTs, T9s, 98s, 87s, 76s, AQo+";
char CC_SBvsMP2[]  = "QQ-44, AQs+, AQo+";
char CC_BBvsMP2[]  = "QQ-22, ATs+, KQs, JTs, AQo+";

// vs MP3
char CC_COvsMP3[] = "QQ-55, ATs+, KJs+, QJs, JTs, T9s, 98s, 87s, 76s, AQo+";
char CC_BUvsMP3[] = "JJ-33, AQs-ATs, KTs+, QTs+, J9s+, T9s, 98s, 87s, 76s, 65s, AQo+";
char CC_SBvsMP3[] = "QQ-55, AQs, KQs, AQo+";
char CC_BBvsMP3[] = "QQ-22, ATs+, KJs+, QJs, JTs, AQo+";

// vs CO
char CC_BUvsCO[] = "TT-22, AJs-A9s, KTs+, QTs+, J9s+, T8s+, 98s, 87s, 76s, 65s, 54s, AQo-AJo, KQo";
char CC_SBvsCO[] = "TT-66, AJs-ATs, KJs+, QJs, AQo-AJo, KQo";
char CC_BBvsCO[] = "JJ-22, AJs-A9s, KTs+, QTs+, JTs, T9s, AQo-AJo, KQo";

// vs BU
char CC_SBvsBU[] = "TT-77, AJs-A9s, KTs+, QJs, JTs, AJo, KJo+, QJo";
char CC_BBvsBU[] = "TT-22, ATs-A2s, K5s+, Q7s+, J7s+, T8s+, 98s, 87s, AJo-A2o, K9o+, Q9o+, J9o+, T9o";

// vs SB
char CC_BBvsSB[] = "99-22, ATs-A2s, K5s+, Q6s+, J7s+, T7s+, 97s+, 86s+, 75s+, 65s, 54s, AJo-A5o, K8o+, Q9o+, J8o+, T8o+, 98o";

//
// 3BET
//
// vs MP2
char TB_MP3vsMP2[] = "KK+, A5s-A4s, AJo, KQo";
char TB_COvsMP2[] = "KK+, A5s-A4s, KJs, AJo, KQo";
char TB_BUvsMP2[] = "KK+, A5s-A3s, KJs, AJo, KQo";
char TB_SBvsMP2[] = "KK+, KQs, JTs, T9s, 98s, 87s";
char TB_BBvsMP2[] = "KK+, T9s, 98s, 87s, 76s, 65s";

// vs MP3
char TB_COvsMP3[] = "KK+, A5s-A3s, J9s, T8s, AJo, KQo";
char TB_BUvsMP3[] = "QQ+, AKs, A9s-A2s, T8s, 97s, AJo, KQo";
char TB_SBvsMP3[] = "KK+, AKs, JTs, T8s+, 98s, 87s";
char TB_BBvsMP3[] = "KK+, T8s+, 98s, 87s, 76s";

// vs CO
char TB_BUvsCO[] = "JJ+, AQs+, A8s-A2s, K9s-K6s, Q9s-Q8s, J8s-J7s, 97s, 86s, AKo, ATo-A9o, KJo-KTo, QJo";
char TB_SBvsCO[] = "JJ+, 55-22, AQs+, KTs, QTs, J9s+, T8s+, 97s+, 87s, 76s, 65s, 54s, AKo";
char TB_BBvsCO[] = "QQ+, AQs+, A5s-A4s, K9s, Q9s, J9s, T8s, 97s+, 86s+, 76s, 65s, 54s, AKo";

// vs BU
char TB_SBvsBU[] = "JJ+, 66-44, AQs+, A8s-A2s, K9s-K5s, QTs-Q8s, J9s-J8s, T8s+, 97s+, 86s+, 76s, 65s, 54s, AQo+";
char TB_BBvsBU[] = "JJ+, AJs+, K4s-K2s, Q6s-Q2s, J6s, T7s, 97s-96s, 86s-85s, 75s+, 64s+, 53s+, 43s, AQo+";

// vs SB
char TB_BBvsSB[] = "TT+, AJs+, K4s-K2s, Q5s-Q2s, J6s, T6s, 96s, 85s, 74s, 64s, AQo+, A4o-A2o, K7o-K5o, Q8o";

/*

vs 3BET
Sb vs 3b Call AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, QdQh, QdQs, QdQc, JdJh, JdJs, JdJc, TdTh, TdTs, TdTc, AJs + , KQs, JTs, T9s, AJo +
SB vs 3b 4bet AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, QhQs, QhQc, QsQc, JhJs, JhJc, JsJc, ThTs, ThTc, TsTc, AKs, ATs - A2s, KJs - K7s, Q9s + , AKo, ATo, KTo +
BTN vs 3b Call 99 - 77, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, QdQh, QdQs, QdQc, JdJh, JdJs, JdJc, TdTh, TdTs, TdTc, AQs - ATs, KTs + , QTs + , J9s + , T9s, 98s, AdKd, AhKh, Ad5d, Ah5h, Ad4d, Ah4h, Ad3d, Ah3h, Ad2d, Ah2h, AQo - AJo, KQo, AhKd, AsKd, AsKh, AcKd, AcKh, AcKs
BTN vs 3b 4bet AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, QhQs, QhQc, QsQc, JhJs, JhJc, JsJc, ThTs, ThTc, TsTc, A9s - A6s, K9s - K6s, Q9s - Q8s, AsKs, AcKc, ATo, KJo, AdKh, AdKs, AdKc, AhKs, AhKc, AsKc
CO vs 3b Call IP JJ - 99, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, QdQh, QdQs, QdQc, AQs - ATs, KJs + , JTs, T9s, AdKd, AhKh, AQo - AJo, KQo, AhKd, AsKd, AsKh, AcKd, AcKh, AcKs
CO vs 3b 4bet IP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, QhQs, QhQc, QsQc, A9s, A5s - A2s, KTs - K9s, QJs, AsKs, AcKc, ATo, AdKh, AdKs, AdKc, AhKs, AhKc, AsKc
CO vs 3b Call OOP JJ - TT, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, QdQh, QdQs, QdQc, AQs - AJs, KQs, AdKd, AhKh, AQo, AhKd, AsKd, AsKh, AcKd, AcKh, AcKs
CO vs 3b 4bet OOP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, QhQs, QhQc, QsQc, ATs - A9s, A5s - A2s, KJs - K9s, QTs + , AsKs, AcKc, AJo, KQo, AdKh, AdKs, AdKc, AhKs, AhKc, AsKc
MP vs 3b Call IP QQ - JJ, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, TdTh, TdTs, TdTc, 9d9h, 9d9s, 9d9c, 8d8h, 8d8s, 8d8c, AJs + , KQs, AQo +
MP vs 3b 4bet IP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, ATs - A9s, KJs, AJo, KQo
MP vs 3b Call OOP QQ, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, JdJh, JdJs, JdJc, TdTh, TdTs, TdTc, AQs + , AQo +
MP vs 3b 4bet OOP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, AJs, KQs, AJo, KQo
UTG vs 3b Call IP QQ - JJ, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, TdTh, TdTs, TdTc, 9d9h, 9d9s, 9d9c, 8d8h, 8d8s, 8d8c, AQs + , AKo
UTG vs 3b 4bet IP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, AJs - ATs, AQo - AJo
UTG vs 3b Call OOP QQ, AdAh, AdAs, AdAc, KdKh, KdKs, KdKc, JdJh, JdJs, JdJc, TdTh, TdTs, TdTc, 9d9h, 9d9s, 9d9c, AQs + , AKo
UTG vs 3b 4bet OOP AhAs, AhAc, AsAc, KhKs, KhKc, KsKc, AJs - ATs, AQo

*/

void Preflop();
void FirstAction();
void RankIntToChar(int rank, char* cRank);
int  RankCharToInt(char* cRank);
void Suit(int suit, char* cSuit);
void GetHand(char* hand);
void HandToHand169(char* hand, char* hand169);
extern void InitHand169(char* hand169[]);
unsigned int Rotr(unsigned int value, unsigned int size, unsigned int count);
int RightCalls();
bool CheckHand169Range(char* hand169, char* range);
bool CheckHand169Subrange(char* hand169, char* subrange);
int ReadSubrange(char* inputRange, char* outputSubrange);

int RightChairs();
double CurrentBet(int chair);
int DecChair(int chair);
bool RaiserSB();
bool RaiserBU();
bool RaiserCO();
bool RaiserMP3();
bool RaiserMP2();
int UserPosition();