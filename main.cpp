#include "Game.h"
#include <iostream>

using namespace std;

int main() {
    cout << "==============================================\n";
    cout << "   다빈치 코드 게임에 오신 것을 환영합니다!\n";
    cout << "==============================================\n\n";

    Game game;
    game.play();

    cout << "\n게임을 종료합니다. 감사합니다!\n";

    return 0;
}
