#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>
#include <cstring> // Thư viện cần thiết để dùng memset sửa lỗi -Wnonnull

using namespace std;

// Định nghĩa các hằng số khung hình
#define MINX 2
#define MINY 2
#define MAXX 35
#define MAXY 20

struct Point {
    int x, y;
};

// Hàm mô phỏng gotoxy cho môi trường terminal Unix
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Hàm mô phỏng kbhit() cho Linux/Codespaces
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, STDIN_FILENO);
        return 1;
    }
    return 0;
}

// Hàm mô phỏng getch() - Đã sửa lỗi argument 2 null
char getch(void) {
    char buf = 0;
    struct termios old;
    // Sửa lỗi: Khởi tạo bộ nhớ sạch cho struct thay vì gán {0}
    memset(&old, 0, sizeof(struct termios)); 
    
    if (tcgetattr(0, &old) < 0) return 0;
    
    struct termios new_settings = old;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    
    tcsetattr(0, TCSANOW, &new_settings);
    if (read(0, &buf, 1) < 0) buf = 0;
    tcsetattr(0, TCSANOW, &old);
    
    return buf;
}

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
    
    void DiChuyen(int Huong, Point& Qua) {
        for (int i = DoDai - 1; i > 0; i--) {
            A[i] = A[i - 1];
        }
        
        if (Huong == 0) A[0].x++; // Phải
        if (Huong == 1) A[0].y++; // Xuống
        if (Huong == 2) A[0].x--; // Trái
        if (Huong == 3) A[0].y--; // Lên
        
        // Kiểm tra ăn mồi
        if (A[0].x == Qua.x && A[0].y == Qua.y) {
            DoDai++;
            Qua.x = rand() % (MAXX - MINX + 1) + MINX;
            Qua.y = rand() % (MAXY - MINY + 1) + MINY;
        }
    }
    
    void Ve(Point Qua) {
        for (int i = 0; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            if (i == 0) cout << "O";
            else cout << "x";
        }
        gotoxy(Qua.x, Qua.y);
        cout << "*";
    }
};

void VeKhung() {
    for (int i = MINX - 1; i <= MAXX + 1; i++) {
        for (int j = MINY - 1; j <= MAXY + 1; j++) {
            if (i == MINX - 1 || i == MAXX + 1 || j == MINY - 1 || j == MAXY + 1) {
                gotoxy(i, j);
                printf("+");
            }
        }
    }
}

int main() {
    Snake r;
    Point Qua;
    int Huong = 0; // 0: Phải, 1: Xuống, 2: Trái, 3: Lên
    char t;
    
    srand(time(0));
    Qua.x = rand() % (MAXX - MINX + 1) + MINX;
    Qua.y = rand() % (MAXY - MINY + 1) + MINY;
    
    // Xóa màn hình ban đầu
    printf("\033[2J"); 
    
    while (1) {
        if (kbhit()) {
            t = getch();
            if (t == 'a' && Huong != 0) Huong = 2;
            else if (t == 'd' && Huong != 2) Huong = 0;
            else if (t == 'w' && Huong != 1) Huong = 3;
            else if (t == 's' && Huong != 3) Huong = 1;
            else if (t == 'x') break;
        }
        
        printf("\033[2J"); // Xóa màn hình chuẩn ANSI
        VeKhung();
        r.Ve(Qua);
        r.DiChuyen(Huong, Qua);
        
        // Kiểm tra va chạm khung
        if (r.A[0].x < MINX || r.A[0].x > MAXX || r.A[0].y < MINY || r.A[0].y > MAXY) {
            gotoxy(1, MAXY + 2);
            cout << "GAME OVER!" << endl;
            break;
        }
        
        // Đẩy dữ liệu ra terminal ngay lập tức
        fflush(stdout);
        // Tốc độ chơi (150ms)
        usleep(150000); 
    }
    
    return 0;
}
