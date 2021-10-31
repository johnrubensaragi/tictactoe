#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cmath>
using namespace std;

char box[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};            //the box
char sign, signAI, first, turn, winner;                                 //for storing 'X's, 'O's, 'p'(player), and 'c'(computer)
int m, i;                                                               //m stands for move, i for picking who plays first
bool endGame;
void drawBox(){
    cout << "+-+-+-+" << endl;                                          // +-+-+-+
    cout << "+" <<box[0]<< "+" <<box[1]<< "+" <<box[2]<< "+" << endl;   // +0+1+2+
    cout << "+-+-+-+" << endl;                                          // +-+-+-+
    cout << "+" <<box[3]<< "+" <<box[4]<< "+" <<box[5]<< "+" << endl;   // +3+4+5+
    cout << "+-+-+-+" << endl;                                          // +-+-+-+
    cout << "+" <<box[6]<< "+" <<box[7]<< "+" <<box[8]<< "+" << endl;   // +6+7+8+
    cout << "+-+-+-+" << endl;                                          // +-+-+-+
}
bool pattern(char box[9], char sign){                                   //for searching SIGN pattern in a BOX
    if((box[0] == sign) && (box[1] == sign) && (box[2] == sign) ||
       (box[0] == sign) && (box[3] == sign) && (box[6] == sign) ||
       (box[0] == sign) && (box[4] == sign) && (box[8] == sign) ||
       (box[1] == sign) && (box[4] == sign) && (box[7] == sign) ||
       (box[2] == sign) && (box[4] == sign) && (box[6] == sign) ||
       (box[2] == sign) && (box[5] == sign) && (box[8] == sign) ||
       (box[3] == sign) && (box[4] == sign) && (box[5] == sign) ||
       (box[6] == sign) && (box[7] == sign) && (box[8] == sign)) return true;
    else return false;
}
void win(){         //procedure whether if someone is winning
    winner='t';                         //let it tie first
    for (int i = 0; i < 9; i++){
        if (box[i] == ' '){             //unless a box is not filled
            winner = ' ';
            break;
        }
    }
    if(pattern(box, 'X')){              //if X pattern is found
        if(first == 'p')winner = 'p';   //first player is the winner
        else winner = 'c';
        }
    if(pattern(box, 'O')){              //if O pattern is found
        if(first == 'p')winner = 'c';   //second player is the winner
        else winner = 'p';
        }
    if(winner != ' ')endGame = true;    //end the game if something win
}
bool valid(int m){         //checking validity of move
    if(box[m] == ' ') return true;
    else return false;
}
int evaluate(char box[9], bool &draw){  //evaluating position from AI's persepective
    draw = true;                        //tie
    for (int i = 0; i < 9; i++){
        if (box[i] == ' '){             //unless a box is not filled
            draw = false;
            break;
        }
    }
    if(pattern(box, sign))return -1;            //player winning yields -1
    else if(pattern(box, signAI))return 1;      //AI winning yields +1
    else return 0;                              //no pattern yields 0
}
void max(char box[9], int (&parentscore));  //minimax algorithm
/*  Minimax Algorithm uses a tree, simulating every possible move, both for the player and for the AI.
    Both players are expected for choosing the best move for themselves.
    For instance, AI will choose a move with highest evaluating score.
    Player will choose a move with lowest score(from AI persepective).
    And for that we will have nodes. Max node will simulate AI and min node will simulate player.
    AI will continue simulating itself until all the positions yield either a win, lose, draw, or the maximum desired depth reached.
    parentscore is an score output from every node, and childscore is an input from every node.
    More on this later. Let's find out!
*/
void min(char box[9], int (&parentscore)){  //simulating player
    int childscore[9] = {0,0,0,0,0,0,0,0,0};
    bool draw;
    parentscore = 2;
    for(int i = 0; i < 9; i++){
        if(box[i] == ' '){                                              //if box is empty
            box[i] = sign;                                          //picking every possible move for player
            childscore[i] = evaluate(box,draw);
            if(childscore[i] == 0 && !draw)max(box,childscore[i]);
            box[i] = ' ';                                           //erase it
        }else childscore[i] = 32767;                                    //if box is not empty, don't choose it!
    }
    for(int i = 0; i < 9; i++){
        if(childscore[i] < parentscore){
            parentscore = childscore[i];                                //report the lowest score to parent
        }
    }
}
void max(char box[9], int (&parentscore)){  //simulating ai
    int childscore[9] = {0,0,0,0,0,0,0,0,0};
    bool draw;
    parentscore = -2;
    for(int i = 0; i < 9; i++){
        if(box[i] == ' '){
            box[i] = signAI;                                          //picking every possible move for ai
            childscore[i] = evaluate(box,draw);
            if(childscore[i] == 0 && !draw)min(box,childscore[i]);
            box[i] = ' ';
        }else childscore[i] = -32767;
    }
    for(int i = 0; i < 9; i++){
        if(childscore[i] > parentscore){
            parentscore = childscore[i];                                //report the highest score to parent
        }
    }
}
int doai(char box[9]){
    bool draw;
    int score[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int best_score, i = 0, j = 0, k = 0;
    best_score = -2;
    endGame = false;
    turn = 'c';
    for(i = 0; i < 9; i++){
        if(box[i] == ' ')j++;
    }
    for(i = 0; i < 9; i++){
        if(box[i] == ' '){                              //simulating ai
            box[i] = signAI;
            score[i] = evaluate(box,draw);
            if(score[i] == 0 && !draw)min(box,score[i]);
            box[i] = ' ';
        }else score[i] = -32767;
        if(score[i] > best_score)best_score = score[i];
    }
    j = 0;
    for(i = 0; i < 9; i++){
        if(score[i] == best_score)j++;
    }
    i = -1;
    k = rand() % j + 1;
    for(i = 0; i < 9; i++){
        if(score[i] == best_score)k--;
        if(k == 0)return i;
    }
    return 0;
}
int main(){
    srand(time(0));
    drawBox();
    cout << "Who plays first?\n";
    cout << "p: player\n";
    cout << "c: computer\n";
    cout << "r: random\n";
    cout << "Your choice: ";
    cin >> turn;
    while(turn != 'p' && turn != 'c' && turn != 'r'){
        cout << "\nError! Try again: ";
        cin >> turn;
    }
    if(turn == 'r'){
        i = rand() % 2;
        if(i == 1)turn = 'p';
        else turn = 'c';
    }
    first = turn;
    if(first == 'p'){
        sign = 'X';
        signAI = 'O';
    }
    else{
        sign = 'O';
        signAI = 'X';
    }
    endGame = false;
    while(!endGame){
        if(turn == 'p'){
            cout << "Your turn! Enter your move from 1 to 9: ";
            cin >> m;
            while(!valid(m-1)){
                cout << "\nError! Try another: ";
                cin >> m;
            }
            box[m-1] = sign;
            system("CLS");
            drawBox();
            m = -1;
            win();
        }
        else{
            cout << "Computer's turn...\n";
            m = doai(box);
            box[m] = signAI;
            system("CLS");
            drawBox();
            cout << "Computer chooses: " << m + 1 << endl;
            m = -1;
            win();
        }
        if(turn == 'p')turn = 'c';
        else turn = 'p';
    }
    if(winner == 'p')cout<<"You win!!!";
    else if(winner == 'c')cout << "You lose...";
    else cout << "Draw!";
    cin.ignore();
    cin.get();
}