#include <iostream>
#include <string>
#include <stdexcept>


#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace std;

class Piece {
protected:
    char color;
    string name;
    int position[2];
public:
    Piece(char c, string n, int x, int y) : color(c), name(n) {
        position[0] = x;
        position[1] = y;
    }
    // Конструктор без позиции — для инициализации позже
    Piece(char c, string n) : color(c), name(n) {
        position[0] = -1;
        position[1] = -1;
    }
    virtual ~Piece() {}
    char getColor() const { return color; }
    string getName() const { return name; }
    int getX() const { return position[0]; }
    int getY() const { return position[1]; }

    void setPosition(int x, int y) {
        position[0] = x;
        position[1] = y;
    }

    virtual bool isValidMove(int x, int y, Piece* target) = 0;

    friend ostream& operator<<(ostream& os, const Piece& p) {
        if (p.color == 'W')
            os << GREEN << p.name[0] << RESET;
        else
            os << RED << p.name[0] << RESET;
        return os;
    }
};

class Pawn : public Piece {
    bool firstMove;
public:
    Pawn(char c, int x, int y) : Piece(c, "Pawn", x, y), firstMove(true) {}
    Pawn(char c) : Piece(c, "Pawn"), firstMove(true) {}
        // Префиксный ++ — сбрасывает флаг firstMove в false
    Pawn& operator++() {
        firstMove = false;
        return *this;
    }

    bool isValidMove(int x, int y, Piece* target) {
        int dx = x - position[0];
        int dy = y - position[1];
        int dir = (color == 'W') ? -1 : 1;
        // Захват по диагонали
        if (target && dx == dir && abs(dy) == 1 && target->getColor() != color)
            return true;
        // Прямой ход
        if (!target) {
            if (dx == dir && dy == 0)
                return true;
            if (firstMove && dx == 2*dir && dy == 0)
                return true;
        }
        return false;
    }
};

class Rook : public Piece {
public:
    Rook(char c, int x, int y) : Piece(c, "Rook", x, y) {}
    Rook(char c) : Piece(c, "Rook") {}

    bool isValidMove(int x, int y, Piece* target) {
        // 1. Проверяем, движемся ли по той же строке или столбцу
        bool sameLine = false;
        if (x == position[0] || y == position[1]) {
            sameLine = true;
        } else {
            sameLine = false;
        }
        // 2. Проверяем, можно ли захватить фигуру на целевой клетке
        bool canCapture;
        if (target == nullptr) {
            canCapture = true;             // свободная клетка
        } else if (target->getColor() != color) {
            canCapture = true;             // вражеская фигура
        } else {
            canCapture = false;            // своя фигура
        }
        // 3. Объединяем оба условия
        bool result;
        if (sameLine && canCapture) {
            result = true;
        } else {
            result = false;
        }
        return result;
    }
};

class Nkight : public Piece {
public:
    Nkight(char c, int x, int y) : Piece(c, "Nkight", x, y) {}
    Nkight(char c) : Piece(c, "Nkight") {}

    bool isValidMove(int x, int y, Piece* target) {
        // 1. Вычисляем смещение по осям
        int dx = abs(x - position[0]);
        int dy = abs(y - position[1]);

        // 2. Проверяем, действительно ли это ход «буквой Г»
        bool isLshape;
        if (dx * dy == 2) {
            isLshape = true;
        } else {
            isLshape = false;
        }

        // 3. Проверяем, нет ли на целевой клетке своей фигуры
        bool canCapture;
        if (target == nullptr) {
            canCapture = true;
        } else if (target->getColor() != color) {
            canCapture = true;
        } else {
            canCapture = false;
        }

        // 4. Итог
        bool result;
        if (isLshape && canCapture) {
            result = true;
        } else {
            result = false;
        }
        return result;
    }
};

class Bishop : public Piece {
public:
    Bishop(char c, int x, int y) : Piece(c, "Bishop", x, y) {}
    Bishop(char c) : Piece(c, "Bishop") {}

    bool isValidMove(int x, int y, Piece* target) {
        int dx = abs(x - position[0]);
        int dy = abs(y - position[1]);

        // Диагональный ход?
        bool isDiagonal;
        if (dx == dy) {
            isDiagonal = true;
        } else {
            isDiagonal = false;
        }

        // Захват допустим?
        bool canCapture;
        if (target == nullptr) {
            canCapture = true;
        } else if (target->getColor() != color) {
            canCapture = true;
        } else {
            canCapture = false;
        }

        bool result;
        if (isDiagonal && canCapture) {
            result = true;
        } else {
            result = false;
        }
        return result;
    }
};

class Queen : public Piece {
public:
    Queen(char c, int x, int y) : Piece(c, "Queen", x, y) {}
    Queen(char c) : Piece(c, "Queen") {}

    bool isValidMove(int x, int y, Piece* target) {
        int dx = abs(x - position[0]);
        int dy = abs(y - position[1]);

        // Горизонталь или вертикаль?
        bool straight;
        if (x == position[0] || y == position[1]) {
            straight = true;
        } else {
            straight = false;
        }

        // Диагональ?
        bool diag;
        if (dx == dy) {
            diag = true;
        } else {
            diag = false;
        }

        // Захват
        bool canCapture;
        if (target == nullptr) {
            canCapture = true;
        } else if (target->getColor() != color) {
            canCapture = true;
        } else {
            canCapture = false;
        }

        // Итоговая валидация
        bool result;
        if ((straight || diag) && canCapture) {
            result = true;
        } else {
            result = false;
        }
        return result;
    }
};

class King : public Piece {
public:
    King(char c, int x, int y) : Piece(c, "King", x, y) {}
    King(char c) : Piece(c, "King") {}

    bool isValidMove(int x, int y, Piece* target) {
        int dx = abs(x - position[0]);
        int dy = abs(y - position[1]);

        // Один шаг в любом направлении?
        bool oneStep;
        if (dx <= 1 && dy <= 1) {
            oneStep = true;
        } else {
            oneStep = false;
        }

        // Захват
        bool canCapture;
        if (target == nullptr) {
            canCapture = true;
        } else if (target->getColor() != color) {
            canCapture = true;
        } else {
            canCapture = false;
        }

        bool result;
        if (oneStep && canCapture) {
            result = true;
        } else {
            result = false;
        }
        return result;
    }
};

// Проверка свободного пути для фигур (кроме коня)
bool isPathClear(Piece* board[8][8], int x1, int y1, int x2, int y2, Piece* piece) {
    if (piece->getName() == "Nkight") 
        return true;
    int dx, dy;
    if (x2 - x1 != 0) {
        dx = (x2 - x1) / abs(x2 - x1);
    } else {
        dx = 0;
    }
    if (y2 - y1 != 0) {
        dy = (y2 - y1) / abs(y2 - y1);
    } else {
        dy = 0;
    }
    int cx = x1 + dx, cy = y1 + dy;
    while (cx != x2 || cy != y2) {
        if (board[cx][cy]) return false;
        cx += dx; cy += dy;
    }
    return true;
}

class Board {
    Piece* squares[8][8];
    static int whiteCaptures;
    static int blackCaptures;

    Piece* findKing(char color) {
        for(int i=0;i<8;i++) for(int j=0;j<8;j++)
            if(squares[i][j] && squares[i][j]->getName()=="King" && squares[i][j]->getColor()==color)
                return squares[i][j];
        return nullptr;
    }

    bool isCheck(char color) {
        Piece* king = findKing(color);
        if(!king) return false;
        int kx = king->getX(), ky = king->getY();
        for(int i=0;i<8;i++) 
            for(int j=0;j<8;j++) {
                Piece* p = squares[i][j];
                if(p && (p->getColor()!=color) && p->isValidMove(kx,ky,squares[kx][ky]) && isPathClear(squares,i,j,kx,ky,p))
                    return true;
        }
        return false;
    }

public:
    Board() {
        for(int i=0;i<8;i++) 
            for(int j=0;j<8;j++) squares[i][j]=nullptr;
        for(int j=0;j<8;j++){
            squares[1][j]=new Pawn('B',1,j);
            squares[6][j]=new Pawn('W',6,j);
        }
        squares[0][0]=new Rook('B',0,0); 
        squares[0][7]=new Rook('B',0,7);
        squares[7][0]=new Rook('W',7,0); 
        squares[7][7]=new Rook('W',7,7);
        squares[0][1]=new Nkight('B',0,1); 
        squares[0][6]=new Nkight('B',0,6);
        squares[7][1]=new Nkight('W',7,1); 
        squares[7][6]=new Nkight('W',7,6);
        squares[0][2]=new Bishop('B',0,2); 
        squares[0][5]=new Bishop('B',0,5);
        squares[7][2]=new Bishop('W',7,2); 
        squares[7][5]=new Bishop('W',7,5);
        squares[0][3]=new Queen('B',0,3); 
        squares[7][3]=new Queen('W',7,3);
        squares[0][4]=new King('B',0,4); 
        squares[7][4]=new King('W',7,4);
    }

    ~Board() {
        for(int i=0;i<8;i++) 
            for(int j=0;j<8;j++) delete squares[i][j];
    }

    bool move(string from, string to, char currentColor) {
        if(from=="0"||to=="0") 
            throw runtime_error("Завершение игры");

        if(from.size()!=2||to.size()!=2) 
            throw invalid_argument("Формат хода, например E2 E4");

        int x1=8-(from[1]-'0'), y1=toupper(from[0])-'A';
        int x2=8-(to[1]-'0'),   y2=toupper(to[0])-'A';

        if(x1<0||x1>7||y1<0||y1>7||x2<0||x2>7||y2<0||y2>7) 
            throw invalid_argument("Координаты вне диапазона");

        Piece* p=squares[x1][y1]; 
        if(!p) throw invalid_argument("Нет фигуры");
        if(p->getColor()!=currentColor) 
            throw invalid_argument("Не ваш ход");

        Piece* tgt=squares[x2][y2];
        if(tgt&&tgt->getColor()==currentColor) 
            throw invalid_argument("Нельзя съесть свою фигуру");

        if(!p->isValidMove(x2,y2,tgt)) 
            throw invalid_argument("Недопустимый ход по правилам");

        if(!isPathClear(squares,x1,y1,x2,y2,p)) 
            throw invalid_argument("Путь перекрыт");

        // Учёт съеденной фигуры
        if(tgt) {
            if(tgt->getColor()=='W') whiteCaptures++;
            else blackCaptures++;
        }
        delete tgt; 
        squares[x2][y2]=p; 
        squares[x1][y1]=nullptr; 
        p->setPosition(x2,y2);
        // Если походила пешка — считаем, что первый ход она уже сделала
        if (p->getName() == "Pawn") {
            Pawn* pawn = static_cast<Pawn*>(p);
            ++(*pawn);   // префиксный ++ сбросит firstMove
        }
        // Проверка шаха
        char enemy;
        if (currentColor == 'W') {
            enemy = 'B';
        } else {
            enemy = 'W';
        }
        if(isCheck(enemy))
            cout << "Шах!" << endl;
            /*if(isCheckmate(enemy)) {
                cout << "Мат!" << endl;
                throw runtime_error("Мат");
            }*/

        return true;
    }

    static int getWhiteCaptures() { return whiteCaptures; }
    static int getBlackCaptures() { return blackCaptures; }

    void print() {
        cout<<"  A B C D E F G H\n";
        for(int i=0;i<8;i++){
            cout<<8-i<<" ";
            for(int j=0;j<8;j++){
                if(squares[i][j]) cout<<*squares[i][j]<<" "; else cout<<". ";
            }
            cout<<8-i<<"\n";
        }
        cout<<"  A B C D E F G H\n";
    }

    void printCaptures() {
        cout<<"Белых съедено: "<<whiteCaptures<<"   Черных съедено: "<<blackCaptures<<"\n";
    }
    void clearBoard() {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                squares[i][j] = nullptr;
    }

    void setupCustom() {
        clearBoard(); // удалить все фигуры с доски
        string color, type, coord;

        while (true) {
            cout << "Введите фигуру (например: W R A1) или \"done\": ";
            cin >> color;
            if (color == "done") break;
            cin >> type >> coord;
            if ((color != "W") && (color != "B")) {
                cout << "Формат цвета, например B\n";
                continue;
            }
            if(coord.size()!=2||coord.size()!=2){
                cout << "Формат координат, например А1\n";
                continue;;
            }
            int x = 8 - (coord[1] - '0');
            int y = toupper(coord[0]) - 'A';

            if((x <= 0) || (x > 7) || (y < 0) || (y > 7)) {
                cout << "Координаты вне диапазона\n";
                continue;;
            }

            Piece* p = nullptr;
            char c = (color == "W") ? 'W' : 'B';

            cout << x << "   "<<y<<endl;

            if (type == "K") p = new King(c);
            else if (type == "Q") p = new Queen(c);
            else if (type == "R") p = new Rook(c);
            else if (type == "B") p = new Bishop(c);
            else if (type == "N") p = new Nkight(c);
            else if (type == "P") p = new Pawn(c);
            else {
                cout << "Неверная фигура\n";
                continue;
            }
            if (p) {
                p->setPosition(x, y);
                squares[x][y] = p;
            }
        }
    }

};

int Board::whiteCaptures = 0;
int Board::blackCaptures = 0;

int main() {
    Board board;
    string from, to;
    char current = 'W';
    string mode;
    cout << "Выберите режим: default или create\n";
    cin >> mode;
    if (mode == "create")
        board.setupCustom();

    while(true){
        board.print();
        cout<<(current=='W'?"Ход белых: ":"Ход черных: ");
        cout<<"Введите ход (E2 E4) или 0 для выхода: ";
        cin>>from; if(from=="0") break;
        cin>>to;   if(to=="0") break;
        try{
            board.move(from,to,current);
            current = (current=='W'? 'B':'W');
        } catch(const exception& e){
            cout<<"Ошибка: "<<e.what()<<" Попробуйте снова.\n";
        }
    }
    cout<<"Игра завершена.\n";
    board.printCaptures();
    return 0;
}
