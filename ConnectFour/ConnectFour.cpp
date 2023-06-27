#include "ConnectFour.h"

void Grid::initGrid(){
    this->grid_ = std::vector<std::vector<int>>();
    for(int i = 0; i < rows_; i++){
        this -> grid_.push_back(std::vector<int>());
        for(int j = 0; j < cols_; j++){
            grid_[i].push_back(DotColor::EMPTY);
        }
    }
}

int Grid::PlacePiece(int col, DotColor piece){
    if(col < 0 || col >= this -> cols_) {throw "Invalid column";}
    if(piece == DotColor::EMPTY) {throw "Invalid piece";}

    for(int r = this->rows_ - 1; r >=0; r--){
        if(this -> grid_[r][col] == DotColor::EMPTY){
            this->grid_[r][col] = piece;
            return r;
        }
    }
    return -1;
}

bool Grid::checkWin(int connectN, int row, int col, DotColor piece){
    //check rows
    int count = 0;
    for(int r = 0; r < this -> rows_; r++){
        if(this -> grid_[r][col] == piece) {
            count++;
        }
        else{
            count = 0;
        }
        if(count == connectN) {
            return true;
        }
    }

    //check cols
    count = 0;
    for(int c = 0; c < this -> cols_; c++){
        if (this -> grid_[row][c] == piece){
            count++;
        }
        else{ 
            count = 0;
        }
        if(count == connectN){
            return true;
        }
    }

    //check diag
    count = 0;
    for(int r = 0; r < this->rows_; r++){
        int c = col - row + r; 
        if(c >= 0 && c < this->cols_ && this->grid_[r][c] == piece){
            count++;
        }
        else{
            count = 0;
        }

        if(count == connectN){
            return true;
        }
    }

    //check anti diag
    count = 0;
    for(int r = 0; r < this->rows_; r++){
        int c = col + row - r; 
        if(c >= 0 && c < this->cols_ && this -> grid_[r][c] == piece){
            count++;
        }
        else{
            count = 0;
        }

        if(count == connectN){
            return true;
        }
    }
    return false;
}

Game::Game(Grid* gird, int targetScore, int connectN){
    this->grid_ = gird;
    this->targetScore_ = targetScore;
    this->connectN_ = connectN;

    this->Players_ = std::vector<Player*>{
        new Player("Player 1", YELLOW),
        new Player("Player 2", RED)
    };
    this->score_ = std::unordered_map<std::string, int>();
    for(Player* p : this ->Players_){
        this -> score_[p -> getName()] = 0;
    }
}

void Game::printBoard(){
    std::cout << "Board" << std::endl;

    std::vector<std::vector<int>> grid = this -> grid_ -> getGrid();
    for(int i = 0; i < grid.size(); i++){
        std::string row = "";
        for(auto piece : grid[i]){ 
            if(piece == DotColor::EMPTY)
                row += "0 ";
            else if(piece == DotColor::YELLOW)
                row +=  "Y ";
            else if(piece == DotColor::RED)
                row += "R ";
        }
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

std::pair<int,int> Game::playMove(Player* Player){
    int move_col = 0;
    int move_row = 0;
    printBoard();
    std::cout << "Player's: " << Player -> getName() << " turn" << std::endl;
    int col = this-> grid_ -> getColumnCount();
    std::cout << "Enter kolumn beetwen 0 and " << col - 1 <<": " << std::endl;
    std::cin >> move_col;
    move_row = this -> grid_ -> PlacePiece(move_col, Player -> getColor());
    return std::pair{move_row,move_col};
}

Player* Game::playRound(){
    while(1){
        for(Player* p : this -> Players_){
            std::pair<int,int> rc = playMove(p);
            DotColor color = p -> getColor();
            if(this-> grid_ -> checkWin(this -> connectN_, rc.first, rc.second, color)){
                this -> score_[p -> getName()]++;
                return p;
            }
        }
    }
}

void Game::play(){
    int maxScore = 0;
    Player* winner = nullptr;
    while (maxScore < targetScore_){
        winner = playRound();
        std::cout << winner -> getName() << " won the round" << std::endl;
        for(Player* p : Players_){
            std::cout << p -> getName() << " score: " << this->score_[p->getName()] << std::endl; 
        }
        maxScore = std::max(this -> score_[winner -> getName()], maxScore);
        this -> grid_ -> initGrid();
    }
    std::cout << winner -> getName() << "won the game";
}

int main() {
    Grid* grid = new Grid(6, 7);
    Game* game = new Game(grid, 10, 4);
    game->play();
    return 0;
}