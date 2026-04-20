#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

using namespace std;

// Định nghĩa các hằng số khung hình [cite: 131, 133, 135, 137]
#define MINX 2
#define MINY 2
#define MAXX 35
#define MAXY 20

struct Point {
    int x, y;
};

class Snake {
public:
    Point A[100];
    int DoDai;

    Snake() {
        DoDai = 3;
        A[0] = {10, 10};
        A[1] = {9, 10};
        A[2] = {8, 10};
    }

    // Hàm di chuyển và xử lý ăn mồi [cite: 209, 212, 213, 214, 215, 216]
    void DiChuyen(int Huong, Point& Qua) {
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];

        if (Huong == 0) A[0].x = A[0].x + 1;      // Phải
        if (Huong == 1) A[0].y = A[0].y + 1;      // Xuống
        if (Huong == 2) A[0].x = A[0].x - 1;      // Trái
        if (Huong == 3) A[0].y = A[0].y - 1;      // Lên

        // Kiểm tra ăn mồi 
        if ((A[0].x == Qua.x) && (A[0].y == Qua.y)) {
            DoDai++;
            Qua.x = rand() % (MAXX - MINX) + MINX;
            Qua.y = rand() % (MAXY - MINY) + MINY;
        }
    }

    // Hàm vẽ con rắn và mồi [cite: 170, 175, 178, 183]
    void Ve(Point Qua) {
        for (int i = 0; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            if (i == 0) cout << "O"; // Đầu rắn
            else cout << "x";        // Thân rắn [cite: 178]
        }
        gotoxy(Qua.x, Qua.y);
        cout << "*"; // Quả mồi [cite: 183]
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
};

// Hàm vẽ khung trò chơi [cite: 139, 150, 151, 152, 154]
void VeKhung() {
    for (int i = MINX - 1; i <= MAXX + 1; i++) {
        for (int j = MINY - 1; j <= MAXY + 1; j++) {
            if (i == MINX - 1 || i == MAXX + 1 || j == MINY - 1 || j == MAXY + 1) {
                COORD coord = {(short)i, (short)j};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                printf("+"); [cite: 154]
            }
        }
    }
}

int main() {
    Snake r;
    Point Qua;
    int Huong = 0; // 0: Phải, 1: Xuống, 2: Trái, 3: Lên
    char t;

    srand((int)time(0)); [cite: 220]
    Qua.x = rand() % (MAXX - MINX) + MINX; [cite: 220]
    Qua.y = rand() % (MAXY - MINY) + MINY; [cite: 220]

    while (1) { [cite: 220]
        if (kbhit()) { [cite: 220]
            t = getch(); [cite: 220]
            if (t == 'a' && Huong != 0) Huong = 2;
            if (t == 'd' && Huong != 2) Huong = 0;
            if (t == 'w' && Huong != 1) Huong = 3;
            if (t == 's' && Huong != 3) Huong = 1;
            if (t == 'x') break; // Thoát [cite: 220]
        }

        system("cls"); [cite: 156]
        VeKhung(); [cite: 158]
        r.Ve(Qua); [cite: 220]
        r.DiChuyen(Huong, Qua); [cite: 220]

        // Kiểm tra va chạm khung
        if (r.A[0].x < MINX || r.A[0].x > MAXX || r.A[0].y < MINY || r.A[0].y > MAXY) {
            system("cls");
            cout << "GAME OVER!" << endl;
            break;
        }

        Sleep(200); [cite: 164]
    }

    return 0;
}
