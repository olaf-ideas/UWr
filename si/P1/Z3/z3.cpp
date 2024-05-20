/*
	Wybieranie zwyciezce pomiedzy rekami figuranta i blotkarza:
		Figurant moze miec tylko uklady zwiazane z iloscia tej samej rangi
		(bo ma tylko 4 rozne karty), wiec jego mozliwe uklady to
			wysoka karta, para, dwie pary, trojka oraz kareta
		Drugi moze miec cokolwiek ale posiada karty z gorsza ranga.

		1) Jesli blotkarz posiada wyzszy uklad rangowy to odrazu wygrywa
									     -> BLOTKARZ WYGRYWA

		2) Jesli blotkarz posiada uklad rangowy mniejszy badz rowny dwom
			parom to odrazu przegrywa, bo figurant ma przynajmniej tak
			samo dobry uklad rang i lepsze ranki kart
									     -> FIGURANT WYGRYWA

		3) Jesli blotkarz ma pokera      -> BLOTKARZ WYGRYWA

		4) Jesli figurant ma full house  -> FIGURANT WYGRYWA

		(jesli figurant nie ma full house, a blotkarz go ma to jest to wpradzane w kroku 1)

		5) Jesli blotkarz ma flush       -> BLOTKARZ WYGYRWA

		6) Jesli blotkarz ma straight    -> BLOTKARZ WYGYRWA

		7) WPP figurant wygrywa bo posiada niegorszy uklad rankingowy od blotkarza
		i wyzsze ranki rank.             -> FIGURANT WYGRYWA

	Eksperymenty:
		Bez zmian: 
			random: 0.915351
			wins: 1507239360 all: 1646701056
			p: 0.915308

		Same piki:
			random: 0.0633961
			wins: 34848 all: 550368
			p: 0.0633176

		Piki i jedna dziesiatka karo:
			random: 0.528391
			wins: 581136 all: 1100736
			p: 0.527952

		Piki i kara:
			random: 0.935316
			wins: 35007840 all: 37425024
			p: 0.935413

		wynika z tego ze optymalnie jest brac jeden kolor	
			-> przegrywamy wtedy tylko z kareta
*/

#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <omp.h>

enum Suit : int {
    SUIT_C = 0, // CLUBS
    SUIT_D,     // DIAMONDS
    SUIT_H,     // HEARTS
    SUIT_S,     // SPADES
    SUIT_NB
};

enum Rank : int {
	RANK_2 = 0,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
    RANK_8,
	RANK_9,
    RANK_T,
    RANK_J,
    RANK_Q,
    RANK_K,
    RANK_A,
    RANK_NB
};

struct Card {
	Suit suit;
	Rank rank;
};

typedef std::vector<Card> Deck;

void debug(const Deck &deck) {
	for (Card card : deck)
		std::cout << "<" << card.suit << ',' << card.rank << "> ";
}

// +1 -> figurant
// -1 -> blotkarz
int fight(Deck F, Deck B) {
	// std::cerr << "F: "; debug(F); std::cerr << '\n';
	// std::cerr << "B: "; debug(B); std::cerr << '\n';

	std::vector<int> rank_cnt_F(RANK_NB);
	std::vector<int> rank_cnt_B(RANK_NB);

	for (const Card& card : F)
		rank_cnt_F[card.rank]++;
	for (const Card& card : B)
		rank_cnt_B[card.rank]++;
	
	std::sort(rank_cnt_F.rbegin(), rank_cnt_F.rend());
	std::sort(rank_cnt_B.rbegin(), rank_cnt_B.rend());

	// F must have better or equal cards in rank to win
	if (rank_cnt_F < rank_cnt_B) {
		return -1;
	}
	
	// B can't have color bets so automatically loses
	if (rank_cnt_B[0] >= 2)
		return +1;

	std::sort(B.begin(), B.end(),
		[&](const Card &a, const Card &b) {
			return a.rank < b.rank;
		}
	);

	bool flush = true;
	for (int i = 0; i + 1 < 5; i++)
		if (B[i].suit != B[i + 1].suit)
			flush = false;
	
	bool straight = true;
	for (int i = 0; i + 1 < 5; i++)
		if (B[i].rank + 1 != B[i + 1].rank)
			straight = false;

	// poker for B
	if (flush && straight)
		return -1;
	
	// full house for F
	if (rank_cnt_F[0] == 3 && rank_cnt_F[1] == 2)
		return +1;
	
	// flush or straight for B
	if (flush || straight)
		return -1;

	return +1;
}

std::mt19937 rng(420);

Deck random_hand(Deck deck) {
	std::shuffle(deck.begin(), deck.end(), rng);
	return std::vector<Card>(deck.begin(), deck.begin() + 5);
}

double get_winrate(Deck F, Deck B) {
	int sum = 0, all = 10000000;

	#pragma omp parallel for reduction(+:sum)
	for (int rep = 0; rep < all; rep++) {
		if (fight(random_hand(F), random_hand(B)) == +1)
			sum++;
	}

	return (double) sum / all;
}

Deck current_hand;
void gen(const Deck &deck, int i, int left, std::vector<Deck> &res) {
	if (left == 0) {
		res.push_back(current_hand);
		return;
	}

	if (i == (int) deck.size()) {
		return;
	}

	current_hand.push_back(deck[i]);
	gen(deck, i + 1, left - 1, res);
	current_hand.pop_back();
	gen(deck, i + 1, left, res);
}

int main() {

	Deck figurant_deck;
	for (int rank = RANK_J; rank <= RANK_A; rank++)
		for (int suit = SUIT_C; suit <= SUIT_S; suit++)
			figurant_deck.push_back(Card{Suit(suit), Rank(rank)});
	
	Deck blotkarz_deck;
	for (int rank = RANK_2; rank <= RANK_T; rank++)
		for (int suit = SUIT_H; suit <= SUIT_S; suit++)
			blotkarz_deck.push_back(Card{Suit(suit), Rank(rank)});
	
	std::cerr << "random: " << get_winrate(figurant_deck, blotkarz_deck) << '\n';

	std::vector<Deck> all_figurant;
	std::vector<Deck> all_blotkarz;

	gen(figurant_deck, 0, 5, all_figurant);
	gen(blotkarz_deck, 0, 5, all_blotkarz);
	
	int win = 0;
	int all = (int) all_figurant.size() * all_blotkarz.size();
	
	#pragma omp parallel for reduction(+:win)
	for (int i = 0; i < (int) all_figurant.size(); i++) {
		for (int j = 0; j < (int) all_blotkarz.size(); j++) {
			if (fight(all_figurant[i], all_blotkarz[j]) == +1)
				win++;
		}
	}

	std::cout << "wins: " << win << " all: " << all << '\n';
	std::cout << "p: " << (double) win / all << '\n';

	return 0;
}
