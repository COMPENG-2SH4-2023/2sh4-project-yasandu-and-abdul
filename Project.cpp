#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "GameMechs.h"  // Include the GameMechs header
#include "Player.h" 
#include "objPosArrayList.h"
#include <time.h>

using namespace std;

#define DELAY_CONST 100000

class Player;
GameMechs* myGM = nullptr;
Player* myPlayer = nullptr;

bool exitFlag;

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);



int main(void)
{

    Initialize();

    while(!myGM->getExitFlagStatus())  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}


void Initialize(void)
{
    srand((unsigned int)time(NULL));
    MacUILib_init();
    MacUILib_clearScreen();

    exitFlag = false;

    myGM = new GameMechs(26,13);
    myPlayer = new Player(myGM);
    
    objPos playerPosition;
    objPosArrayList* playerBody = myPlayer->getPlayerPosList();
    if (playerBody && playerBody->getSize() > 0) {
        playerBody->getHeadElement(playerPosition);
    }

    myGM->generateFood(&playerPosition, playerBody); // Pass both playerPosition and playerBody


}

void GetInput(void) {
    myGM->getInput();
}



void RunLogic(void) {
    myPlayer->updatePlayerDir();
    myPlayer->movePlayer();

    objPos headPos;
    objPosArrayList* playerBody = myPlayer->getPlayerPosList();
    if (playerBody && playerBody->getSize() > 0) {
        playerBody->getHeadElement(headPos);
    }

    objPos foodPos;
    myGM->getFoodPos(foodPos);
    
    if (headPos.x == foodPos.x && headPos.y == foodPos.y) {
        myGM->generateFood(&headPos, playerBody); 
        myPlayer->setHasEatenFood(true); 
    }
    myGM->clearInput();
}




 void DrawScreen(void) {
    MacUILib_clearScreen();

    objPosArrayList* playerBody = myPlayer->getPlayerPosList();
    objPos foodPos;
    myGM->getFoodPos(foodPos);

    for (int i = 0; i < myGM->getBoardSizeY(); i++) {
        for (int j = 0; j < myGM->getBoardSizeX(); j++) {

            bool drawn = false;

            for (int k = 0; k < playerBody->getSize(); k++) {
                objPos segment;
                playerBody->getElement(segment, k);

                if (j == segment.x && i == segment.y) {
                    MacUILib_printf("%c", segment.symbol);
                    drawn = true;
                    break;
                }

            }

            if (drawn) continue;

            if (i == 0 || i == myGM->getBoardSizeY() - 1 || j == 0 || j == myGM->getBoardSizeX() - 1) {
                MacUILib_printf("%c", '#');
            }
            else if (j == foodPos.x && i == foodPos.y) {
                MacUILib_printf("%c", foodPos.symbol);
            }
            // Draw empty space
            else {
                MacUILib_printf(" ");
            }
        }
        printf("\n");
    }   


    if(myGM->getLoseFlagStatus() == false){
        MacUILib_printf("Food Location: <%d, %d>\n", foodPos.x, foodPos.y);
        MacUILib_printf("Score: %d\n", myPlayer->getScore());
    }

    else if(myGM->getLoseFlagStatus() == true){
        MacUILib_printf("\n\nYou Lost");
        MacUILib_printf("\nFinal Score: %d\n", myPlayer->getScore());
        MacUILib_printf("\n\n--Press - to Quit--");
        MacUILib_Delay(5000000);
        myGM->setExitTrue();
        }

    // Additional info can be printed here if needed
}
void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}


void CleanUp(void)
{
    
    delete myPlayer;
    delete myGM;

    MacUILib_clearScreen();    
  
    MacUILib_uninit();
}