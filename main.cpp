#include <iostream>
#include <system_error>
#include "GameState.h"
#include "Graph.h"
#include "Vertex.h"
#include "Queue.h"
#include <limits>
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"

using namespace std;

void enter() {
    cout << "Enter to exit: ";
    //string userInput;
    //getline(std::cin, userInput);
    //cin.ignore();
    cout << "Enter to exit: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}

void winner(GameState game) {
    system("clear");
    cout << game << endl;
    cout << endl;
    if (game.hasWon(0)) {
        cout << "Player X has won!" << endl;
    }
    else if (game.hasWon(1)) {
        cout << "Player O has won!" << endl;
    }
    else {
        cout << "It's a tie!" << endl;
    }
    //return 0;
}

void againstHuman(GameState game) {
    game.reset();
    while (!game.done) {
        system("clear");
        cout << game << endl;

        cout << endl;
        cout << "Enter move for (" << (!game.currentTurn ? "X" : "0") << "): ";
        int x, y;
        cin >> x >> y;

        game.play(x, y);
    }
    //return 0;
    winner(game);
    enter();
}

Vec validMove(GameState game){
    for (int i = 0; i < game.size; i++){
        for (int j = 0; j < game.size; j++){
            if (game.grid[i][j] == -1){
                return Vec(i, j);
            }
        }
    }
    return Vec(0,0);
}

void againstWeak(GameState game) {
    game.reset();
    while(!game.done){
        system("clear");
        cout << game << endl;

        int x, y;

        if (game.currentTurn){
            Vec move = validMove(game);
            x = move.x;
            y = move.y;
        }
        else{
            cout << endl;
            cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            cin >> x >> y;
        }

        game.play(x, y);
    }
    winner(game);
    enter();
}

const int MAX_SCORE = numeric_limits<int>::max();
const int MIN_SCORE = numeric_limits<int>::min();

struct MoveScore {
    Vec move;
    int score;
};

int getReward(Vertex<GameState>* start, int player) {
    if (start->neighbors.size() == 0) {
        int currPlayer = start->data.currentTurn;
        if (start->data.hasWon(player)) {
            return 100;
        } else if (start->data.hasWon(!player)) {
            return -100;
        } else {
            return 50;
        }
    } else {
        int reward = getReward(start->neighbors[0]->location, player);
        for (int i = 1; i < start->neighbors.size(); i++) {
            int curr = getReward(start->neighbors[i]->location, player);
            if (start->data.currentTurn == player) {
                if (curr > reward) {
                    reward = curr;
                }
            } else {
                if (curr < reward) {
                    reward = curr;
                }
            }
        }
        return reward;
    }
}

MoveScore minimax(Vertex<GameState>* node, int depth, int player) {
    if (depth == 0 || node->data.done) {
        MoveScore result;
        result.move = Vec(0, 0);
        result.score = getReward(node, player);
        return result;
    }

    if (node->data.currentTurn == player) {
        MoveScore bestMove;
        bestMove.score = MIN_SCORE;

        for (int i = 0; i < node->neighbors.size(); i++) {
            MoveScore childScore = minimax(node->neighbors[i]->location, depth - 1, player);

            if (childScore.score > bestMove.score) {
                bestMove.score = childScore.score;
                bestMove.move = node->neighbors[i]->location->data.lastMove;
            }
        }

        return bestMove;
    } else {
        MoveScore bestMove;
        bestMove.score = MAX_SCORE;

        for (int i = 0; i < node->neighbors.size(); i++) {
            MoveScore childScore = minimax(node->neighbors[i]->location, depth - 1, player);

            if (childScore.score < bestMove.score) {
                bestMove.score = childScore.score;
                bestMove.move = node->neighbors[i]->location->data.lastMove;
            }
        }
        return bestMove;
    }
}

Vec findBestMove(GameState game) {
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);
    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);

    while (!frontier.isEmpty()) {
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done) {
            for (int i = 0; i < game.size; i++) {
                for (int j = 0; j < game.size; j++) {
                    if (curr->data.grid[i][j] == -1) {
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        successor->data.lastMove = Vec(i, j);
                        if (!successor->data.done) {
                            frontier.enqueue(successor);
                        }
                    }
                }
            }
        }
    }

    MoveScore bestMove = minimax(start, numeric_limits<int>::max(), 0);
    return bestMove.move;
}

void againstBest(GameState game) {
    game.reset();
    while(!game.done){
        system("clear");
        cout << game << endl;

        int x, y;

        if (game.currentTurn){
            Vec move = findBestMove(game);
            x = move.x;
            y = move.y;
        }
        else{
            cout << endl;
            cout << "Enter 'y-coordinate x-coordinate' move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            cin >> x >> y;
        }

        game.play(x, y);
    }
    winner(game);
    enter();
}


int main(){ 
    GameState game;
    //game.play(0, 0);
    //game.play(1, 1);
    //cout << game << endl;
    
    mainMenu();
    int option;
    int option3;
    option3 =3;
    cin >> option;
    string difficulty; 
    difficulty = "Easy";

    while (option != 4) {
        GameState game(option3);
        if (option == 1) {
            if (difficulty == "Easy") {
                againstWeak(game);
            }
            else {
                againstBest(game);
            }
        }
        else if (option == 2) {
            againstHuman(game);
        }
        else if (option == 3) {
            system("clear");
            cout << "Select AI Difficulty Level: Currently " << difficulty << endl;
            cout << endl;
            cout << "1. Easy" << endl;
            cout << "2. Hard" << endl;
            cout << endl;
            cout << "Enter choice: ";
            int option2;
            cin >> option2;
            if (option2 == 1) {
                
                difficulty = "Easy";
            }
            else if (option2 == 2) {

                difficulty = "Hard";
            }
            system("clear");
            cout << "Enter grid size (recommend [3-4]): ";
            cin >> option3;
        }

        mainMenu();
        cin >> option;
    }

    cout << endl << "Thank you for playing!" << endl;

    return 0;
}