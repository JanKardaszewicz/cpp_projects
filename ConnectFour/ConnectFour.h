#include <iostream>
#include <vector>
#include <unordered_map>


enum DotColor{
    EMPTY,
    YELLOW,
    RED
};

class Grid{
private:
    int rows_;
    int cols_ ; 
    std::vector<std::vector<int>> grid_;

public:
    Grid(int rows, int cols) : rows_(rows), cols_(cols) {initGrid();}
    void initGrid();
    std::vector<std::vector<int>> getGrid() {return this -> grid_;}
    int getColumnCount(){return this -> cols_;}
    int PlacePiece(int col, DotColor piece);
    bool checkWin(int connectN, int row, int col, DotColor piece);
};

class Player{
private:
    std::string name_;
    DotColor color_;
public:
    Player(std::string name, DotColor color) : name_(name), color_(color) {}
    std::string getName() {return this -> name_;}
    DotColor getColor() {return this -> color_;}
};

class Game{
private:
    std::vector<Player*> Players_;
    Grid* grid_;
    int targetScore_;
    int connectN_;
    std::unordered_map<std::string,int> score_;
public:
    Game(Grid* gird, int targetScore, int connectN);
    void printBoard();
    std::pair<int,int> playMove(Player* Player);
    Player* playRound();
    void play();
};
