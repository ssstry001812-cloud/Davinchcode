#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

Game::Game() {
    srand(time(0));
    playerHasDrawn = false;
    computerHasDrawn = false;
    initDeck();
    dealInitialTiles();
}

void Game::initDeck() {
    for (int i = 0; i <= 11; i++) {
        deck.push_back(Tile('B', i));
    }
    for (int i = 0; i <= 11; i++) {
        deck.push_back(Tile('W', i));
    }
    random_shuffle(deck.begin(), deck.end());
}

void Game::dealInitialTiles() {
    for (int i = 0; i < 4; i++) {
        playerTiles.push_back(deck.back());
        deck.pop_back();
    }

    for (int i = 0; i < 4; i++) {
        computerTiles.push_back(deck.back());
        deck.pop_back();
    }

    sort(playerTiles.begin(), playerTiles.end());
    sort(computerTiles.begin(), computerTiles.end());
}

void Game::displayTiles(const vector<Tile>& tiles, bool showAll) {
    int size = tiles.size();

    // 번호 줄 (카드 중앙 정렬)
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "[" << (i + 1) << "]  ";
    }
    cout << "\n";

    // 위쪽 테두리
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "-----";
    }
    cout << "\n";

    // 색깔 줄
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "| " << tiles[i].color << " |";
    }
    cout << "\n";

    // 중간 테두리
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "|---|";
    }
    cout << "\n";

    // 숫자 줄
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "|";
        if (tiles[i].revealed || showAll) {
            if (tiles[i].number < 10) {
                cout << " " << tiles[i].number << " ";
            }
            else {
                cout << tiles[i].number << " ";
            }
        }
        else {
            cout << " ? ";
        }
        cout << "|";
    }
    cout << "\n";

    // 아래쪽 테두리
    cout << "     ";
    for (int i = 0; i < size; i++) {
        cout << "-----";
    }
    cout << "\n";
}

void Game::displayBoard() {
    cout << "\n==============================================\n";
    cout << "플레이어 패:\n";
    displayTiles(playerTiles, true);

    cout << "\n컴퓨터 패:\n";
    displayTiles(computerTiles, false);

    cout << "\n남은 덱: " << deck.size() << "장\n";
    cout << "==============================================\n";
}

bool Game::isGameOver() {
    bool playerAllRevealed = true;
    for (const auto& tile : playerTiles) {
        if (!tile.revealed) {
            playerAllRevealed = false;
            break;
        }
    }

    bool computerAllRevealed = true;
    for (const auto& tile : computerTiles) {
        if (!tile.revealed) {
            computerAllRevealed = false;
            break;
        }
    }

    return playerAllRevealed || computerAllRevealed;
}

int Game::countHiddenTiles(const vector<Tile>& tiles) {
    int count = 0;
    for (const auto& tile : tiles) {
        if (!tile.revealed) count++;
    }
    return count;
}

void Game::playerTurn() {
    cout << "\n================ 플레이어 턴 =================\n";

    if (deck.size() > 0) {
        lastDrawnPlayer = deck.back();
        deck.pop_back();
        playerHasDrawn = true;

        cout << "타일을 뽑았습니다: " << lastDrawnPlayer.color
            << lastDrawnPlayer.number << "\n";

        playerTiles.push_back(lastDrawnPlayer);
        sort(playerTiles.begin(), playerTiles.end());

        cout << "\n플레이어 패:\n";
        displayTiles(playerTiles, true);
    }

    cout << "\n컴퓨터 패:\n";
    displayTiles(computerTiles, false);

    int choice;
    do {
        cout << "\n맞출 타일 번호 (1~" << computerTiles.size() << "): ";
        cin >> choice;
        choice--;
    } while (choice < 0 || choice >= computerTiles.size() ||
        computerTiles[choice].revealed);

    int guess;
    cout << "숫자 추측 (0~11): ";
    cin >> guess;

    if (computerTiles[choice].number == guess) {
        cout << "\n✓ 정답입니다!\n";
        computerTiles[choice].revealed = true;

        if (isGameOver()) return;

        char cont;
        cout << "계속 추리하시겠습니까? (y/n): ";
        cin >> cont;

        if (cont == 'y' || cont == 'Y') {
            cout << "\n컴퓨터 패:\n";
            displayTiles(computerTiles, false);

            do {
                cout << "\n맞출 타일 번호 (1~" << computerTiles.size() << "): ";
                cin >> choice;
                choice--;
            } while (choice < 0 || choice >= computerTiles.size() ||
                computerTiles[choice].revealed);

            cout << "숫자 추측 (0~11): ";
            cin >> guess;

            if (computerTiles[choice].number == guess) {
                cout << "\n✓ 또 정답입니다!\n";
                computerTiles[choice].revealed = true;
            }
            else {
                cout << "\n✗ 틀렸습니다!\n";
                if (playerHasDrawn) {
                    for (auto& tile : playerTiles) {
                        if (tile.color == lastDrawnPlayer.color &&
                            tile.number == lastDrawnPlayer.number &&
                            !tile.revealed) {
                            tile.revealed = true;
                            cout << "방금 뽑은 타일 " << tile.color << tile.number
                                << "을 공개합니다.\n";
                            break;
                        }
                    }
                }
            }
        }
    }
    else {
        cout << "\n✗ 틀렸습니다!\n";
        if (playerHasDrawn) {
            for (auto& tile : playerTiles) {
                if (tile.color == lastDrawnPlayer.color &&
                    tile.number == lastDrawnPlayer.number &&
                    !tile.revealed) {
                    tile.revealed = true;
                    cout << "방금 뽑은 타일 " << tile.color << tile.number
                        << "을 공개합니다.\n";
                    break;
                }
            }
        }
    }

    playerHasDrawn = false;
}

void Game::computerTurn() {
    cout << "\n================= 컴퓨터 턴 ==================\n";

    if (deck.size() > 0) {
        lastDrawnComputer = deck.back();
        deck.pop_back();
        computerHasDrawn = true;

        cout << "컴퓨터가 타일을 뽑았습니다.\n";

        computerTiles.push_back(lastDrawnComputer);
        sort(computerTiles.begin(), computerTiles.end());
    }

    vector<int> hidden;
    for (int i = 0; i < playerTiles.size(); i++) {
        if (!playerTiles[i].revealed) {
            hidden.push_back(i);
        }
    }

    if (hidden.empty()) return;

    int target = hidden[rand() % hidden.size()];
    int guess = rand() % 12;

    cout << "컴퓨터가 타일 [" << (target + 1) << "]을 "
        << playerTiles[target].color << guess << "로 추측합니다.\n";

    if (playerTiles[target].number == guess) {
        cout << "\n✓ 컴퓨터가 맞췄습니다!\n";
        playerTiles[target].revealed = true;

        if (isGameOver()) return;

        if (rand() % 2 == 1) {
            cout << "컴퓨터가 계속 추리합니다.\n";

            hidden.clear();
            for (int i = 0; i < playerTiles.size(); i++) {
                if (!playerTiles[i].revealed) {
                    hidden.push_back(i);
                }
            }

            if (!hidden.empty()) {
                target = hidden[rand() % hidden.size()];
                guess = rand() % 12;

                cout << "컴퓨터가 타일 [" << (target + 1) << "]을 "
                    << playerTiles[target].color << guess << "로 추측합니다.\n";

                if (playerTiles[target].number == guess) {
                    cout << "\n✓ 컴퓨터가 또 맞췄습니다!\n";
                    playerTiles[target].revealed = true;
                }
                else {
                    cout << "\n✗ 컴퓨터가 틀렸습니다!\n";
                    if (computerHasDrawn) {
                        for (auto& tile : computerTiles) {
                            if (tile.color == lastDrawnComputer.color &&
                                tile.number == lastDrawnComputer.number &&
                                !tile.revealed) {
                                tile.revealed = true;
                                cout << "컴퓨터의 타일 " << tile.color << tile.number
                                    << "을 공개합니다.\n";
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        cout << "\n✗ 컴퓨터가 틀렸습니다!\n";
        if (computerHasDrawn) {
            for (auto& tile : computerTiles) {
                if (tile.color == lastDrawnComputer.color &&
                    tile.number == lastDrawnComputer.number &&
                    !tile.revealed) {
                    tile.revealed = true;
                    cout << "컴퓨터의 타일 " << tile.color << tile.number
                        << "을 공개합니다.\n";
                    break;
                }
            }
        }
    }

    computerHasDrawn = false;
}

void Game::showResult() {
    cout << "\n==============================================\n";
    cout << "                 게임 종료!\n";
    cout << "==============================================\n\n";

    cout << "최종 결과:\n\n";
    cout << "플레이어 패:\n";
    displayTiles(playerTiles, true);

    cout << "\n컴퓨터 패:\n";
    displayTiles(computerTiles, true);

    int playerHidden = countHiddenTiles(playerTiles);
    int computerHidden = countHiddenTiles(computerTiles);

    cout << "\n플레이어 남은 타일: " << playerHidden << "개\n";
    cout << "컴퓨터 남은 타일: " << computerHidden << "개\n\n";

    if (playerHidden > computerHidden) {
        cout << "🏆 플레이어 승리!\n";
    }
    else if (computerHidden > playerHidden) {
        cout << "💻 컴퓨터 승리!\n";
    }
    else {
        cout << "🤝 무승부!\n";
    }
}

void Game::play() {
    cout << "==============================================\n";
    cout << "              다빈치 코드 게임\n";
    cout << "==============================================\n";
    cout << "게임 규칙:\n";
    cout << "1. 자기 차례에 타일 1개를 뽑습니다\n";
    cout << "2. 상대방 타일의 숫자를 추리합니다\n";
    cout << "3. 맞추면: 상대 타일 공개 + 계속 추리 가능\n";
    cout << "4. 틀리면: 방금 뽑은 타일 공개 + 턴 종료\n";
    cout << "5. 모든 타일이 공개된 사람이 패배!\n";
    cout << "==============================================\n";

    bool playerTurn = true;

    while (!isGameOver() && deck.size() > 0) {
        displayBoard();

        if (playerTurn) {
            this->playerTurn();
        }
        else {
            this->computerTurn();
        }

        playerTurn = !playerTurn;

        if (!isGameOver()) {
            cout << "\nEnter를 눌러 계속...";
            cin.ignore();
            cin.get();
        }
    }

    showResult();
}

