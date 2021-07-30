#pragma once

#ifndef SNAKES_H
#define SNAKES_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

template <class T>
class Queue {
	/*	An array based queue to implement player turns
		Implement additional functions as per requirement
	*/

	T* queueArr;
	int size;
	int ffront;
	int rear;
	int numItems;

public:

	Queue() {
		queueArr = NULL;
		ffront = -1;
		rear = -1;
		size = 0;
		numItems = 0;
	}

	Queue(int s) {
		queueArr = new T[s];

		ffront = -1;
		rear = -1;
		size = s;
		numItems = 0;
	}

	int getNumItems() const {
		return numItems;
	}

	void enqueue(T val) {
		if (numItems == size)
			return;

		else {
			if (ffront == -1)
				ffront++;

			rear = (rear + 1) % size;
			queueArr[rear] = val;
			numItems++;
		}
	}

	T deqeue() {
		if (is_empty())
			return -1;

		ffront = (ffront + 1) % size;
		numItems--;

		return queueArr[ffront];
	}

	T front() {
		if (ffront >= 0)
			return queueArr[ffront];
		return -1;
	}

	T back() {
		if (rear >= 0)
			return queueArr[rear];
		return -1;
	}

	bool is_empty() {
		return (numItems == 0);
	}

	bool is_full() {
		if (numItems < size)
			return false;
		return true;
	}

	void shiftQueue() {
		T temp = queueArr[0];

		for (int i = 0; i < numItems - 1; i++)
			queueArr[i] = queueArr[i + 1];

		queueArr[numItems - 1] = temp;
	}

	void reverseQueue() {
		int i = 0;
		int j = numItems - 1;

		while (i < j) {
			T temp = queueArr[i];
			queueArr[i] = queueArr[j];
			queueArr[j] = temp;
			i++;
			j--;
		}
	}

	~Queue() {
		delete[]queueArr;
	}
};

class RollStack {
	/* Implement a stack for your dice roll operations */

	struct node {
		int data;
		node* next;
	};

	node* head;
	node* topp;

public:
	RollStack() {
		topp = NULL;
		head = NULL;
	}

	void push_roll(int value) {
		/* Add a roll on top of the stack */

		node* temp = new node;
		temp->data = value;
		temp->next = NULL;

		if (head == NULL && topp == NULL) {
			head = temp;
			topp = temp;

			return;
		}

		topp->next = temp;
		topp = temp;
	}
	
	int top() {

		if (is_empty()) {
			return -1;
		}

		return topp->data;
	} 

	void pop() {

		if (is_empty()) {
			return;
		}

		if (head->next == NULL) {

			delete topp;

			head = NULL;
			topp = NULL;
		}

		node* current = head;

		while (current && current->next != topp)
			current = current->next;

		delete topp;

		if (current)
			current->next = NULL;

		topp = current;

	}

	bool is_empty() {
		if (topp == NULL)
			return true;

		return false;
	}

	void empty() {
		while (!is_empty()) {
			pop();
		}
	}
};


struct Tile {

	Tile *next, *previous, *up, *down;
	int pos;
	bool snakeHead;
	bool ladderTail;

	Tile() : next(NULL), previous(NULL), up(NULL), down(NULL) {
		pos = 0;
		snakeHead = false;
		ladderTail = false;
	}

	int getPos() const {
		return pos;
	}
};

class Game {

	Queue<int> turnQueue;
	Tile* head;
	bool* hasEnteredBoard;
	int* playerPositions;

public:
	Game(const std::string& filename, int num_players) : head(NULL), turnQueue(num_players) {
		/*
			Initialize the game board from the given file and setup players
		*/

		string arr[100];

		ifstream in(filename);

		if (in.is_open()) {

			int i = 0;

			in >> arr[i];

			while (!in.eof()) {
				i++;
				in >> arr[i];
			}

			in.close();
		}

		// reverses the whole array
		for (int i = 0; i < 50; i++) {
			string tmp = arr[i];
			arr[i] = arr[99 - i];
			arr[99 - i] = tmp;
		}
		
		// reverses every other row
		for (int i = 0; i < 100; i++) {
			if (i % 20 == 0) {
				for (int j = i, k = 0; j < i + 5; j++, k++) {
					string tmp = arr[j];
					arr[j] = arr[i + 9 - k];
					arr[i + 9 - k] = tmp;
				}
			}
		}

		head = new Tile;
		head->pos = 1;
		Tile* current = head;
		int count = 1;

		for (int i = 2; i <= 100; i++, count++) {
			Tile* tmp = new Tile;
			tmp->pos = i;

			if (count % 10 != 0 || count == 1) {
				tmp->previous = current;
				current->next = tmp;
				current = current->next;
			}

			else {
				tmp->down = current;
				current->up = tmp;
				current = current->up;
			}
		}

		bool ladderFlag = false;
		current = head;
		Tile* prev = current;
		count = 1;

		for (int i = 0; i < 100; i++, count++) {
			if (arr[i] == "L1") {
				if (!ladderFlag) {
					prev = current;
					prev->ladderTail = true;
					ladderFlag = true;
				}

				else if (ladderFlag) {
					prev->up = current;
					prev = current;
				}
			}

			if (count % 10 != 0 || count == 1)
				current = current->next;

			else
				current = current->up;
		}
		
		ladderFlag = false;
		current = head;
		prev = current;
		count = 1;

		for (int i = 0; i < 100; i++, count++) {
			if (arr[i] == "L2") {
				if (!ladderFlag) {
					prev = current;
					prev->ladderTail = true;
					ladderFlag = true;
				}

				else if (ladderFlag) {
					prev->up = current;
					prev = current;
				}
			}

			if (count % 10 != 0 || count == 1)
				current = current->next;

			else
				current = current->up;
		}

		bool snakeFlag = false;
		current = head;
		prev = current;
		count = 1;

		for (int i = 0; i < 100; i++, count++) {
			if (arr[i] == "S2") {
				if (!snakeFlag) {
					prev = current;
					snakeFlag = true;
					current->snakeHead = true;
				}

				else if (snakeFlag) {
					prev->snakeHead = false;
					current->down = prev;
					prev = current;
					current->snakeHead = true;
				}
			}

			if (count % 10 != 0 || count == 1)
				current = current->next;

			else
				current = current->up;
		}

		snakeFlag = false;
		current = head;
		prev = current;
		count = 1;

		for (int i = 0; i < 100; i++, count++) {
			if (arr[i] == "S1") {
				if (!snakeFlag) {
					prev = current;
					snakeFlag = true;
					current->snakeHead = true;
				}

				else if (snakeFlag) {
					prev->snakeHead = false;
					current->down = prev;
					prev = current;
					current->snakeHead = true;
				}
			}

			if (count % 10 != 0 || count == 1)
				current = current->next;

			else
				current = current->up;
		}

		// player queues
		hasEnteredBoard = new bool[num_players];
		playerPositions = new int[num_players];

		for (int i = 0; i < 4; i++) {
			turnQueue.enqueue(i + 1);
			playerPositions[i] = 0;
			hasEnteredBoard[i] = false;
		}

	}

	Queue<int>& GetTurnQueue() {
		/* Return your player turn queue */
		return turnQueue;
	}

	Tile* GetTile(int pos) {
		/* returns the Tile at the given position (head, bottom left, is at pos 1) */
		
		if (pos < 1 || pos > 100)
			return NULL;

		Tile* current = head;

		int count = 1; 

		while (current && current->pos != pos) {

			if (count % 10 != 0 || count == 1)
				current = current->next;

			else
				current = current->up;

			count++;
		}

		return current;
	}

	bool IsLadder(int pos) {
		/* returns true if there is a ladder tail at 'pos' */
		Tile* t = GetTile(pos);
		return t->ladderTail;
	}

	bool IsSnake(int pos) {
		/* returns true if there is a snake head at 'pos' */
		Tile* t = GetTile(pos);
		return t->snakeHead;
	}

	int TraverseBoard(int current_pos, RollStack& stack) {
		/*
			Traverse the board and return the resulting position
			beginning from 'current_pos' using the roll stack
		*/

		int new_pos = current_pos;

		Tile* tmp = GetTile(new_pos);

		while (!stack.is_empty()) {
			new_pos += stack.top();

			if (IsLadder(new_pos)) {
				Tile* tmp = GetTile(new_pos);

				while (tmp->up != NULL)
					tmp = tmp->up;

				new_pos = tmp->pos;
			}

			if (IsSnake(new_pos)) {
				Tile* tmp = GetTile(new_pos);

				while (tmp->down != NULL)
					tmp = tmp->down;

				new_pos = tmp->pos;
			}

			stack.pop();
		}

		if (IsLadder(new_pos)) {
			Tile* tmp = GetTile(new_pos);

			while (tmp->up != NULL)
				tmp = tmp->up;

			new_pos = tmp->pos;
		}

		if (IsSnake(new_pos) == true) {
			Tile* tmp = GetTile(new_pos);

			while (tmp->down != NULL)
				tmp = tmp->down;

			new_pos = tmp->pos;
		}

		return new_pos;
	}

	void RollDice(int value) {
		/* Add a roll for the current player, update turn queue accordingly */

		int index = turnQueue.front(); // number of current player

	/*	if (!diceStack.is_empty()) {
			diceStack.push_roll(value);
			return;
		} */

		if (value == 6) {
			if (!hasEnteredBoard[index-1]) {
				hasEnteredBoard[index - 1] = true;
				playerPositions[index - 1] = 1;
			}

			else {
			//	diceStack.push_roll(value);
				return;
			}
		}

		if (value == 3) {
			turnQueue.reverseQueue();
			return;
		}

		turnQueue.shiftQueue();
	}

	void Play() {
		/*	Run the game until the required number of players
			complete the game. Print all relevant outputs during play
		*/

		RollStack diceStack;

		srand(time(NULL));

		while (turnQueue.getNumItems() > 1) {
			// game runs while there is more than one player in the queue

			int index = turnQueue.front();

			if (playerPositions[index - 1] >= 100) { // player has won
				cout << "Player " << index << " has completed the game." << endl;
				turnQueue.deqeue();
			}

			index = turnQueue.front();

			cout << "It is player " << index << "'s turn: " << endl;

			int dice = 1 + ( rand() % 6 );
			int sixCount = 0;

			cout << "Dice roll is: " << dice << endl;

			if (dice != 6) {
				diceStack.push_roll(dice);
				int newP = TraverseBoard(playerPositions[index - 1], diceStack);
				playerPositions[index - 1] = newP;
				RollDice(dice);
			}

			else {
				while (dice == 6) {
					sixCount++;
					dice = 1 + (rand() % 6);
					diceStack.push_roll(dice);
					cout << "Dice roll is: " << dice << endl;
				}

				if (sixCount == 3) {
					turnQueue.shiftQueue();
				}

				else {
					int newP = TraverseBoard(playerPositions[index - 1], diceStack);
					playerPositions[index - 1] = newP;
					RollDice(dice);
				}
			}

			diceStack.empty();

			cout << "Player " << index << " is now at position " << playerPositions[index - 1] << endl;
		}
		
	}

	~Game() {
		/* deallocate and cleanup */

		delete []hasEnteredBoard;
		delete []playerPositions;
	}
};

#endif