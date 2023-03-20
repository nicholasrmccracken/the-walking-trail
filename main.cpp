// Created by Nicholas McCracken

// To play, follow the tutorial at:
// u.osu.edu/fehproteus/vs-code-environment/installation-instructions

// Credits to: u.osu.edu/fehproteus, cplusplus.com, visitoregon.com

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <FEHLCD.h>
#include <FEHImages.h>
#include <FEHUtility.h>

#define X_MAX 320
#define Y_MAX 240
#define MAX_HEALTH 128
#define DEAD 0
#define CIG_INCREMENTER 25
#define FOOD_INCREMENTER 15
#define BULLETS_INCREMENTER 3
#define MEDS_INCREMENTER 1
#define TRUE 1
#define FALSE -1

struct MainMenuStats
{
    int wins;
    int losses;

    MainMenuStats ()
    {
        wins = 0;
        losses = 0;
    }
} mainMenuStatsObject;

struct GameResources
{
    int cigs;
    int fuel;
    int food;
    int water;
    int meds;
    int bullets;
    int resources[6] = {cigs, fuel, food, water, bullets, meds};
    int resourceIncrementAmounts[6] = {25, 5, 15, 15, 3, 1};
    char resourceNamesLowerCase[6][20] = {"cigs", "fuel", "food", "water", "bullets", "meds"};
    char resourceNamesUpperCase[6][20] = {"Cigs", "Fuel", "Food", "Water", "Bullets", "Meds"};
} gameResourcesObject;

struct PartyMembers
{
    int survivorCount;
    int maximumPartyHealth[5];
    int partyHealth[5];
    char partyNames[5][20];
    char survivors[29][20] = {"Rick", "Lori", "Carl", "Daryl", "Carol", "Beth",
    "Rosita", "Jesus", "Eugene", "Glenn", "Maggie", "Merle", "Alpha", "Beta", 
    "Dale", "T-Dog", "Morgan", "Judith", "Negan", "Aaron", "Shane", "Lydia",
    "Henry", "Lance", "Mercer", "Dwight", "Jerry", "Sasha", "Bob"};
} partyMembersObject;

struct Incrementers
{
    int pace;
    float foodRations;
    float waterRations;
} incrementersObject;

struct Date
{
    int currentMonth;
    int currentDay;
    int currentYear;
    int endMonth;
    int endDay;
    int endYear;
} dateObject;

struct CyclicalTrackers
{
    int dayCycleTracker;
    int recentDayCycle;
} cyclicalTrackersObject;

struct OnOffLevers
{
    int programLever;
    int gameplayLever;

    OnOffLevers ()
    {
        programLever = TRUE;
    }
} onOffLeversObject;

class Utility
{
    public:
    void clearScreen ()
    {
        LCD.Clear();
        LCD.ClearBuffer();
        LCD.Update();
        LCD.SetFontColor(WHITE);
    }

    void highlightPressedButton (FEHIcon::Icon placeholder, int horizontalCoordinate, int verticalCoordinate)
    {
        placeholder.WhilePressed(horizontalCoordinate, verticalCoordinate);
        placeholder.Deselect(); 
    }

    void printReturnButton ()
    {   
        FEHIcon::Icon returnButton[1];
        char returnLabel[1][20] = {"Return"};
        FEHIcon::DrawIconArray(returnButton, 1, 1, 200, 0, 20, 20, returnLabel, WHITE, DARKRED);

        int x, y, xTrash, yTrash;
        int returnLever = TRUE;

        while (returnLever == TRUE)
        {
            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (returnButton[0].Pressed(x, y, 0))
            {
                highlightPressedButton(returnButton[0], x, y);

                returnLever = FALSE;
            }
        }
    }
};

class Shop
{
    private:
    int x, y, xTrash, yTrash;

    public:
    void printShopInterface ()
    {
        Utility utilityObject;

        FEHIcon::Icon shopWaterButton[1];
        FEHIcon::Icon shopFuelButton[1];
        FEHIcon::Icon shopFoodButton[1];
        FEHIcon::Icon shopMedsButton[1];
        FEHIcon::Icon shopBulletsButton[1];
        FEHIcon::Icon shopMultiplierButtons[3];
        FEHIcon::Icon shopPurchaseButtons[2];
        FEHIcon::Icon shopLeaveButton[1];

        char shopFuelLabel[1][20] = {""};
        char shopFoodLabel[1][20] = {""};
        char shopWaterLabel[1][20] = {""};
        char shopMedsLabel[1][20] = {""};
        char shopBulletsLabel[1][20] = {""};
        char shopMultiplierLabels[3][20] = {"x1", "x3", "x5"};
        char shopPurchaseLabels[2][20] = {"+", "-"};
        char shopLeaveLabel[1][20] = {"Done"};

        int selectedResource = 1, selectedMultiplier = 1;
        int shopLever = TRUE;

        utilityObject.clearScreen ();

        while (shopLever == TRUE)
        {
            FEHImage shop;
            shop.Open("shop_backdrop.pic");
            shop.Draw(0, 0);
            shop.Close();

            FEHIcon::DrawIconArray(shopFuelButton, 1, 1, 43, 133, 121, 130, shopFuelLabel, WHITE, WHITE);
            FEHIcon::DrawIconArray(shopFoodButton, 1, 1, 48, 157, 17, 225, shopFoodLabel, WHITE, WHITE);
            FEHIcon::DrawIconArray(shopWaterButton, 1, 1, 160, 70, 112, 179, shopWaterLabel, WHITE, WHITE);
            FEHIcon::DrawIconArray(shopMedsButton, 1, 1, 94, 115, 220, 32, shopMedsLabel, WHITE, WHITE);
            FEHIcon::DrawIconArray(shopBulletsButton, 1, 1, 14, 214, 236, 43, shopBulletsLabel, WHITE, WHITE);
            FEHIcon::DrawIconArray(shopMultiplierButtons, 1, 3, 172, 44, 224, 0, shopMultiplierLabels, WHITE, DARKRED);
            FEHIcon::DrawIconArray(shopPurchaseButtons, 1, 2, 195, 22, 224, 0, shopPurchaseLabels, WHITE, DARKRED);
            FEHIcon::DrawIconArray(shopLeaveButton, 1, 1, 218, 0, 224, 0, shopLeaveLabel, WHITE, DARKRED);

            LCD.SetFontColor(WHITE);

            LCD.WriteAt("Cigs:", 0, 176);
            LCD.WriteAt(gameResourcesObject.cigs, 58, 176);
            LCD.WriteAt("Fuel:", 0, 199);
            LCD.WriteAt(gameResourcesObject.fuel, 58, 199);
            LCD.WriteAt("Food:", 0, 222);
            LCD.WriteAt(gameResourcesObject.food, 58, 222);
            LCD.WriteAt("Water:", 106, 176);
            LCD.WriteAt(gameResourcesObject.water, 182, 176);
            LCD.WriteAt("Meds:", 111, 199);
            LCD.WriteAt(gameResourcesObject.meds, 169, 199);
            LCD.WriteAt("Bullets:", 106, 222);
            LCD.WriteAt(gameResourcesObject.bullets, 198, 222);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}
            
            if (shopFuelButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopFuelButton[0], x, y);
            
                selectedResource = 1;
            }
            else if (shopFoodButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopFoodButton[0], x, y);

                selectedResource = 2;
            }
            else if (shopWaterButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopWaterButton[0], x, y);

                selectedResource = 3;
            }
            else if (shopBulletsButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopBulletsButton[0], x, y);

                selectedResource = 4;
            }
            else if (shopMedsButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopMedsButton[0], x, y);

                selectedResource = 5;
            }
            else if (shopMultiplierButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopMultiplierButtons[0], x, y);   

                selectedMultiplier = 1;
            }
            else if (shopMultiplierButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopMultiplierButtons[1], x, y);   

                selectedMultiplier = 3;
            }
            else if (shopMultiplierButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopMultiplierButtons[2], x, y);   

                selectedMultiplier = 5;
            }
            else if (shopPurchaseButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopPurchaseButtons[0], x, y);

                buyResource(selectedResource, selectedMultiplier);   
            }
            else if (shopPurchaseButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopPurchaseButtons[1], x, y);   

                sellResource(selectedResource, selectedMultiplier); 
            }
            else if (shopLeaveButton[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(shopLeaveButton[0], x, y);

                utilityObject.clearScreen ();

                shopLever = FALSE;
            }

            LCD.Update();
        }
    }

    void buyResource (int item, int multiplier)
    {
        if (gameResourcesObject.cigs >= (25 * multiplier))
        {
            switch (item)
            {
                case 1:
                gameResourcesObject.fuel += 5 * multiplier;
                gameResourcesObject.cigs -= 25 * multiplier; 
                break;

                case 2:
                gameResourcesObject.food += 15 * multiplier;
                gameResourcesObject.cigs -= 25 * multiplier;
                break;

                case 3:
                gameResourcesObject.water += 15 * multiplier;
                gameResourcesObject.cigs -= 25 * multiplier; 
                break;

                case 4:
                gameResourcesObject.bullets += 3 * multiplier;
                gameResourcesObject.cigs -= 25 * multiplier; 
                break;

                case 5:
                gameResourcesObject.meds += 1 * multiplier;
                gameResourcesObject.cigs -= 25 * multiplier; 
                break;
            }
        }
    }

    void sellResource (int item, int multiplier)
    {
        switch (item)
        {
            case 1:
            if (gameResourcesObject.fuel >= (5 * multiplier))
            {
                gameResourcesObject.fuel -= 5 * multiplier;
                gameResourcesObject.cigs += 25 * multiplier; 
            }
            break;

            case 2:
            if (gameResourcesObject.food >= (15 * multiplier))
            {
                gameResourcesObject.food -= 15 * multiplier;
                gameResourcesObject.cigs += 25 * multiplier; 
            }
            break;

            case 3:
            if (gameResourcesObject.water >= (15 * multiplier))
            {
                gameResourcesObject.water -= 15 * multiplier;
                gameResourcesObject.cigs += 25 * multiplier; 
            }
            break;

            case 4:
            if (gameResourcesObject.bullets >= (3 * multiplier))
            {
                gameResourcesObject.bullets -= 3 * multiplier;
                gameResourcesObject.cigs += 25 * multiplier; 
            }
            break;

            case 5:
            if (gameResourcesObject.meds >= (1 * multiplier))
            {
                gameResourcesObject.meds -= 1 * multiplier;
                gameResourcesObject.cigs = 25 * multiplier; 
            }
            break;
        }
    }
};

class GameEvents
{
    int x, y, xTrash, yTrash;

    public:
    int calculateNumberPosition (int randomInteger)
    {
        int integerPosition = 1;

        if ((randomInteger / 10) >= 1)
        {
            integerPosition++;
        }

        return integerPosition;
    }

    void resourceIncrementer (int item, int multiplier)
    {
        struct GameResources gameResources;

        switch (item)
        {
            case 0:
            gameResourcesObject.cigs += 25 * multiplier;
            break;

            case 1:
            gameResourcesObject.fuel += 5 * multiplier;
            break;

            case 2:
            gameResourcesObject.food += 25 * multiplier;
            break;

            case 3:
            gameResourcesObject.water += 25 * multiplier;
            break;

            case 4:
            gameResourcesObject.meds += 1 * multiplier;
            break;

            case 5:
            gameResourcesObject.bullets += 3 * multiplier;
            break;
        }
    }

    void resourceDecrementer (int item, int multiplier)
    {
        switch (item)
        {
            case 0:
            gameResourcesObject.cigs -= 25 * multiplier;
            if (gameResourcesObject.cigs < 0)
            {
                gameResourcesObject.cigs = 0;
            }
            break;

            case 1:
            gameResourcesObject.fuel -= 5 * multiplier;
            if (gameResourcesObject.fuel < 0)
            {
                gameResourcesObject.fuel = 0;
            }
            break;

            case 2:
            gameResourcesObject.food -= 25 * multiplier;
            if (gameResourcesObject.food < 0)
            {
                gameResourcesObject.food = 0;
            }
            break;

            case 3:
            gameResourcesObject.water -= 25 * multiplier;
            if (gameResourcesObject.water < 0)
            {
                gameResourcesObject.water = 0;
            }
            break;

            case 4:
            gameResourcesObject.meds -= 1 * multiplier;
            if (gameResourcesObject.meds < 0)
            {
                gameResourcesObject.meds = 0;
            }
            break;

            case 5:
            gameResourcesObject.bullets -= 3 * multiplier;
            if (gameResourcesObject.bullets < 0)
            {
                gameResourcesObject.bullets = 0;
            }
            break;
        }
    }

    int countLivingPartyMembers ()
    {
        int memberCount = 0;

        for (int index = 0; index < 5; index++)
        {
            if (partyMembersObject.partyHealth[index] > DEAD)
            {
                memberCount++;
            }
        }

        return memberCount;
    }

    int findLivingMember ()
    {
        int lever = TRUE;
        int member;

        while (lever == TRUE) 
        {
            member = rand() % 5;

            if (partyMembersObject.partyHealth[member] > DEAD)
            {
                lever = FALSE;
            }
        } 
        return member;
    }

    int findSecondLivingMember (int firstLivingMember)
    {
        int lever = TRUE;
        int member;

        while (lever == TRUE)
        {
            member = rand() % 5;

            if (partyMembersObject.partyHealth[member] > DEAD && member != firstLivingMember)
            {
                lever = FALSE;
            }
        }
        return member;
    }

    int findDeadMember ()
    {
        int deadMemberIndex = FALSE;
        int checkForDeadMember = FALSE;

        for (int index = 0; index < 5; index++)
        {
            if (partyMembersObject.partyHealth[index] == DEAD)
            {
                checkForDeadMember = TRUE;
            }
        }

        while (checkForDeadMember == TRUE)
        {
            int randomPartyMemberIndex = rand() % 5;

            if (partyMembersObject.partyHealth [randomPartyMemberIndex] == DEAD)
            {
                deadMemberIndex = randomPartyMemberIndex;

                checkForDeadMember = FALSE;
            }
        }
        return deadMemberIndex;
    }

    int findSecondResource (int firstResource)
    {
        int lever = TRUE;
        int secondResource;

        while (lever == TRUE)
        {
            secondResource = rand() % 6;

            if (secondResource != firstResource)
            {
                lever = FALSE;
            }
        }
        return secondResource;
    }

    void sortUsedSurvivor (int survivor)
    {
        for (; survivor < partyMembersObject.survivorCount; survivor++)
        {
            char temp[20];
            strcpy(temp, partyMembersObject.survivors[survivor]);
            strcpy(partyMembersObject.survivors[survivor], partyMembersObject.survivors[survivor + 1]);
            strcpy(partyMembersObject.survivors[survivor + 1], temp);
        }
        partyMembersObject.survivorCount--;
    }

    void eventGenerator ()
    {
        int event_chance = 1 + (rand() % 100);

        if (event_chance >= 92)
        {
            biteEvent();
        }
        else if (event_chance >= 84 && event_chance < 92)
        {
            survivorEvent();
        }
        else if (event_chance >= 76 && event_chance < 84)
        {
            groupEvent();
        }
        else if (event_chance >= 68 && event_chance < 76)
        {
            lootEvent ();
        }
        else if (event_chance >= 60 && event_chance < 68)
        {
            if (countLivingPartyMembers() > 1)
            {
                hoardEvent();
            }
        }
        else if (event_chance >= 52 && event_chance < 60)
        {
            if (countLivingPartyMembers() > 1)
            {
                partyMemberFightEvent();
            }
        }
    }

    void biteEvent ()
    {
        Utility utilityObject;

        int successChance = rand() % 100 + 1;
        int randomMember = findLivingMember ();
        int charLengthOfMember = strlen(partyMembersObject.partyNames[randomMember]);
        int lever = TRUE;

        utilityObject.clearScreen ();

        while (lever == TRUE)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 0);
            LCD.WriteRC("'s arm was bitten by", 0, 0 + strlen(partyMembersObject.partyNames[randomMember]));
            LCD.WriteRC("a zombie! They don't have", 1, 0);
            LCD.WriteRC("much time left.", 2, 0); 

            FEHIcon::Icon biteMenuButtons[3];
            char biteMenuLabels[3][20] = {"Try to Heal", "Amputate Limb", "Put to Rest"};
            FEHIcon::DrawIconArray (biteMenuButtons, 3, 1, (Y_MAX / 2), 0, 20, 20, biteMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (biteMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(biteMenuButtons[0], x, y);
                utilityObject.clearScreen ();

                if (gameResourcesObject.meds > 0 && successChance > 25)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 0);
                    LCD.WriteRC("survived! They're", 0, 1 + charLengthOfMember);
                    LCD.WriteRC("very grateful to you, and", 1, 0);
                    LCD.WriteRC("promise they will repay", 2, 0);
                    LCD.WriteRC("the favor one day. You", 3, 0);
                    LCD.WriteRC("used 1 med.", 4, 0);

                    gameResourcesObject.meds--;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else if (gameResourcesObject.meds > 0 && successChance <= 25)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 0);
                    LCD.WriteRC("didn't make it. Your", 0, 1 + charLengthOfMember);
                    LCD.WriteRC("party feels hopeless. You", 1, 0);
                    LCD.WriteRC("used 1 med.", 2, 0);

                    partyMembersObject.partyHealth[randomMember] = DEAD;
                    gameResourcesObject.meds--;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You do not have any meds", 0, 0);
                    LCD.WriteRC("available to heal", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 1, 18);
                    LCD.WriteRC(".", 1, 18 + charLengthOfMember);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
            else if (biteMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(biteMenuButtons[1], x, y);
                utilityObject.clearScreen ();

                if (successChance > 50)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 0);
                    LCD.WriteRC("survived! They're", 0, 1 + charLengthOfMember);
                    LCD.WriteRC("grateful to you, but", 1, 0);
                    LCD.WriteRC("afraid of what comes next", 2, 0);
                    LCD.WriteRC("with their new ailment.", 3, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 4, 0);
                    LCD.WriteRC("'s max health has", 4, charLengthOfMember);
                    LCD.WriteRC("decreased.", 5, 0);

                    partyMembersObject.maximumPartyHealth[randomMember] = partyMembersObject.maximumPartyHealth[randomMember] * 7 / 8;
                    partyMembersObject.partyHealth[randomMember] = partyMembersObject.partyHealth[randomMember] * 7 / 8;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Despite the cut,", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 17);
                    LCD.WriteRC("didn't make it. Your", 1, 0);
                    LCD.WriteRC("party feels hopeless.", 2, 0);

                    partyMembersObject.partyHealth[randomMember] = DEAD;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
            }
            else if (biteMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(biteMenuButtons[2], x, y);
                utilityObject.clearScreen ();

                if (gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You put some lead between", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 1, 0);
                    LCD.WriteRC("'s eyes. Your party", 1, charLengthOfMember);
                    LCD.WriteRC("is afraid of what you're", 2, 0);
                    LCD.WriteRC("capable of. You used 1", 3, 0);
                    LCD.WriteRC("bullet.", 4, 0);

                    partyMembersObject.partyHealth[randomMember] = DEAD;
                    gameResourcesObject.bullets--;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You have no bullets left", 0, 0);
                    LCD.WriteRC("in the chamber.", 1, 0);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
        }
    }

    void survivorEvent ()
    {
        Utility utilityObject;

        int randomSurvivor = rand() % partyMembersObject.survivorCount;
        sortUsedSurvivor(randomSurvivor);

        int randomResource1 = rand() % 6;
        int deadMember = findDeadMember ();
        int charLengthOfSurvivor = strlen(partyMembersObject.survivors[randomSurvivor]);
        int charLengthOfResource = strlen(gameResourcesObject.resourceNamesLowerCase[randomResource1]);
        int lever = TRUE;

        FEHIcon::Icon survivorMenuButtons[4];
        char suvivorMenuLabels[4][20] = {"Let Them Go", "Rob", "Kill", "Invite to Party"};

        utilityObject.clearScreen();
        
        while (lever == TRUE)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You find a survivor on", 0, 0);
            LCD.WriteRC("the road. They look to be", 1, 0);
            LCD.WriteRC("malnourished and afraid.", 2, 0);
            LCD.WriteRC("They tell you their name", 3, 0);
            LCD.WriteRC("is", 4, 0);
            LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 4, 3);
            LCD.WriteRC(".", 4, 3 + charLengthOfSurvivor);

            FEHIcon::DrawIconArray (survivorMenuButtons, 4, 1, (Y_MAX / 2), 0, 20, 20, suvivorMenuLabels, WHITE, DARKRED); 

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (survivorMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(survivorMenuButtons[0], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You allow", 0, 0);
                LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 0, 10);
                LCD.WriteRC("to walk", 0, 11 + charLengthOfSurvivor);
                LCD.WriteRC("away with their life.", 1, 0);
                LCD.WriteRC("They're relieved, and", 2, 0);
                LCD.WriteRC("thank you for your mercy", 3, 0);
                LCD.WriteRC("as they leave to walk", 4, 0);
                LCD.WriteRC("their own path.", 5, 0);

                lever = FALSE;

                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
            else if (survivorMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(survivorMenuButtons[1], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You search", 0, 0);
                LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 0, 11);
                LCD.WriteRC("'s bag,", 0, 11 + charLengthOfSurvivor);
                LCD.WriteRC("despite their begging to", 1, 0);
                LCD.WriteRC("be left alone. You find", 2, 0);
                LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1], 2, 24);
                LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 3, 0);
                LCD.WriteRC(".", 3, charLengthOfResource);
                LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 3, 2 + charLengthOfResource);
                LCD.WriteRC("curses you", 3, 3 + charLengthOfResource + charLengthOfSurvivor);
                LCD.WriteRC("and runs off.", 4, 0);

                resourceIncrementer(randomResource1, 1);
                lever = FALSE;

                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
            else if (survivorMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(survivorMenuButtons[2], x, y);
                utilityObject.clearScreen();

                if (gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You blow", 0, 0);
                    LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 0, 9);
                    LCD.WriteRC("'s brains", 0, 9 + charLengthOfSurvivor);
                    LCD.WriteRC("out all over the road.", 1, 0);
                    LCD.WriteRC("Your party is horrified", 2, 0);
                    LCD.WriteRC("by your violence. You", 3, 0);
                    LCD.WriteRC("find", 4, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1], 4, 5);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 4, 6 + calculateNumberPosition(gameResourcesObject.resourceIncrementAmounts[randomResource1]));
                    LCD.WriteRC("in their", 4, 7 + charLengthOfResource + calculateNumberPosition(gameResourcesObject.resourceIncrementAmounts[randomResource1]));
                    LCD.WriteRC("bag.", 5, 0);

                    resourceIncrementer(randomResource1, 1);
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}    
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You have no bullets left", 0, 0);
                    LCD.WriteRC("in the chamber.", 1, 0);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
            else if (survivorMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(survivorMenuButtons[3], x, y);
                utilityObject.clearScreen();

                if (deadMember != FALSE)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You could use", 0, 0);
                    LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 0, 14);
                    LCD.WriteRC("on", 0, 15 + charLengthOfSurvivor);
                    LCD.WriteRC("the road. You invite them", 1, 0);
                    LCD.WriteRC("to join your party. They", 2, 0);
                    LCD.WriteRC("seem overjoyed. They must", 3, 0);
                    LCD.WriteRC("have been alone for a", 4, 0);
                    LCD.WriteRC("while.", 5, 0);
                    
                    strcpy(partyMembersObject.partyNames[deadMember], partyMembersObject.survivors[randomSurvivor]);
                    partyMembersObject.maximumPartyHealth[deadMember] = MAX_HEALTH;
                    partyMembersObject.partyHealth[deadMember] = MAX_HEALTH;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You have no room in your", 0, 0);
                    LCD.WriteRC("party for", 1, 0);
                    LCD.WriteRC(partyMembersObject.survivors[randomSurvivor], 1, 10);
                    LCD.WriteRC(".", 1, 10 + charLengthOfSurvivor);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
        }
    }

    void groupEvent ()
    {
        Utility utilityObject;

        int successChance =  1 + (rand() % 100);
        int randomMember = findLivingMember();
        int randomResource1 = rand() % 6;
        int randomResource2 = findSecondResource(randomResource1);
        int sumOfResources = 0;
        int charLengthOfMember = strlen(partyMembersObject.partyNames[randomMember]);
        int charLengthOfResource1 = strlen(gameResourcesObject.resourceNamesLowerCase[randomResource1]);
        int charLengthOfResource2 = strlen(gameResourcesObject.resourceNamesLowerCase[randomResource2]);
        int charLengthOfResource1Incrementer = calculateNumberPosition(gameResourcesObject.resourceIncrementAmounts[randomResource1]);
        int charLengthOfResource2Incrementer = calculateNumberPosition(gameResourcesObject.resourceIncrementAmounts[randomResource2]);
        int lever = TRUE;

        for (int index = 0; index < 6; index++)
        {
            sumOfResources += gameResourcesObject.resources[index];
        }
    
        utilityObject.clearScreen ();
        
        while (lever == TRUE)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You encounter another", 0 ,0);
            LCD.WriteRC("group on the road. While", 1, 0);
            LCD.WriteRC("appearing friendly, they", 2, 0); 
            LCD.WriteRC("take", 3, 0);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember], 3, 5);
            LCD.WriteRC("as a hostage.", 3, 6 + charLengthOfMember);
                
            FEHIcon::Icon groupMenuButtons[3];
            char groupMenuLabels[3][20] = {"Attack", "Reason With", "Surrender"};
            FEHIcon::DrawIconArray (groupMenuButtons, 3, 1, (Y_MAX / 2), 0, 20, 20, groupMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (groupMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(groupMenuButtons[0], x, y);
                utilityObject.clearScreen();

                if (successChance > 50 && gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Your shot hit", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 14);
                    LCD.WriteRC("'s ", 0, 14 + charLengthOfMember);
                    LCD.WriteRC("captor. Your party", 1, 0);
                    LCD.WriteRC("follows your lead,", 2, 0);
                    LCD.WriteRC("burying lead in the", 3, 0);
                    LCD.WriteRC("other group's members one", 4, 0);
                    LCD.WriteRC("by one. Your party is", 5, 0);
                    LCD.WriteRC("shaken by the carnage.", 6, 0);
                    LCD.WriteRC("You find", 7, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1] * 2, 7, 9);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 7, 10 + charLengthOfResource1Incrementer);
                    LCD.WriteRC("and", 7, 11 + charLengthOfResource1 + charLengthOfResource1Incrementer);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource2], 7, 15 + charLengthOfResource1 + charLengthOfResource1Incrementer);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource2], 8, 0);
                    LCD.WriteRC("in their bags.", 8, 1 + charLengthOfResource2);

                    resourceIncrementer(randomResource1, 2);
                    resourceIncrementer(randomResource2, 1);
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else if (successChance <= 50 && gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Your shot missed, but", 0 ,0);
                    LCD.WriteRC("theirs didn't.", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 1, 15);
                    LCD.WriteRC("'s", 1, 15 + charLengthOfMember);
                    LCD.WriteRC("brains now littered the", 2, 0);
                    LCD.WriteRC("street. Your party", 3, 0);
                    LCD.WriteRC("attacks, butchering the", 4, 0);
                    LCD.WriteRC("enemy group. Your party", 5, 0);
                    LCD.WriteRC("blames you for", 6, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 6, 15);
                    LCD.WriteRC("'s", 6, 15 + charLengthOfMember);
                    LCD.WriteRC("death. At least you find", 7, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1] * 2, 8, 0);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 8, 1 + charLengthOfResource1Incrementer);
                    LCD.WriteRC("and", 8, 2 + charLengthOfResource1 + charLengthOfResource1Incrementer);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource2], 8, 6 + charLengthOfResource1 + charLengthOfResource1Incrementer);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource2], 8, 7 + charLengthOfResource1 + charLengthOfResource1Incrementer + charLengthOfResource2Incrementer);
                    LCD.WriteRC("in their bags.", 9, 0);

                    partyMembersObject.partyHealth[randomMember] = DEAD;
                    resourceIncrementer(randomResource1, 2);
                    resourceIncrementer(randomResource2, 1);
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}  
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You have no bullets left", 0, 0);
                    LCD.WriteRC("in the chamber.", 1, 0);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
            else if (groupMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(groupMenuButtons[1], x, y);
                utilityObject.clearScreen();

                if (successChance > 75)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You try to appeal to", 0, 0);
                    LCD.WriteRC("their humanity, begging", 1, 0);
                    LCD.WriteRC("for them to see that we", 2, 0);
                    LCD.WriteRC("are all the same. You ask", 3, 0);
                    LCD.WriteRC("if they really want to", 4, 0);
                    LCD.WriteRC("bloody their hands. The", 5, 0);
                    LCD.WriteRC("group lets", 6, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 6, 11);
                    LCD.WriteRC("go", 6, 12 + charLengthOfMember);
                    LCD.WriteRC("free, ashamed of what", 7, 0);
                    LCD.WriteRC("they've become.", 8, 0);

                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You try to appeal to", 0, 0);
                    LCD.WriteRC("their humanity, but they", 1, 0);
                    LCD.WriteRC("have no patience.", 2, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 2, 18);
                    LCD.WriteRC("'s", 2, 18 + charLengthOfMember);
                    LCD.WriteRC("skull burst into hundreds", 3, 0);
                    LCD.WriteRC("of pieces with the pull", 4, 0);
                    LCD.WriteRC("of a trigger. Enranged,", 5, 0);
                    LCD.WriteRC("your party slaughters the", 6, 0);
                    LCD.WriteRC("group like cattle. At", 7, 0);
                    LCD.WriteRC("least you find", 8, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1] * 2, 8, 15);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 8, 16 + charLengthOfResource1Incrementer);
                    LCD.WriteRC("and", 9, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource2], 9, 4);
                    LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource2], 9, 5 + charLengthOfResource2Incrementer);
                    LCD.WriteRC("in their bags.", 9, 6 + charLengthOfResource2Incrementer + charLengthOfResource2);

                    partyMembersObject.partyHealth[randomMember] = DEAD;
                    resourceIncrementer(randomResource1, 2);
                    resourceIncrementer(randomResource2, 1);
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
            }
            else if (groupMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(groupMenuButtons[2], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You lay down your guns", 0, 0);
                LCD.WriteRC("and kneel. The group", 1, 0);
                LCD.WriteRC("searches your bags and", 2, 0);

                if (sumOfResources > 0)
                {
                    LCD.WriteRC("steals", 3, 0);
                    LCD.WriteRC(gameResourcesObject.resourceIncrementAmounts[randomResource1] , 3, 7);
                    LCD.WriteRC(gameResourcesObject.resources[randomResource1], 3, 8 + charLengthOfResource1Incrementer);
                    LCD.WriteRC("from your", 3, 9 + charLengthOfResource1Incrementer + charLengthOfResource1);
                    LCD.WriteRC("party. They leave you on", 4, 0);
                    LCD.WriteRC("your knees, humiliated.", 5, 0);

                    resourceDecrementer(randomResource1, 1);
                    lever = FALSE;
                }
                else
                {
                    LCD.WriteRC("laughs at you for having", 3, 0);
                    LCD.WriteRC("nothing to steal. They", 4, 0);
                    LCD.WriteRC("pop a cap into", 5, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 5, 15);
                    LCD.WriteRC("'s", 5, 15 + charLengthOfMember);
                    LCD.WriteRC("knee for their own", 6, 0);
                    LCD.WriteRC("sadistic pleasure.", 7, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember], 8, 0);
                    LCD.WriteRC("'s health has", 8, charLengthOfMember);
                    LCD.WriteRC("decreased.", 9, 0);

                    partyMembersObject.partyHealth[randomMember] /= 2;
                    lever = FALSE;
                }
                
                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
        }
    }

    void lootEvent ()
    {
        int randomMember = findLivingMember();
        int randomResource1 = (rand() % 6);
        int lever = TRUE;

        while (lever == TRUE)
        {
            Utility utilityObject;
            utilityObject.clearScreen ();

            LCD.SetFontColor(WHITE);
            LCD.WriteRC("While exploring,", 0, 0);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember], 0, 17);
            LCD.WriteRC("stumbled upon the", 1, 0);
            LCD.WriteRC("remains of a survivor.", 2, 0); 
            LCD.WriteRC("Their hands are cold.", 3, 0);
            LCD.WriteRC("You could tell that they", 4, 0);
            LCD.WriteRC("made sure they would not", 5, 0);
            LCD.WriteRC("rise again. Next to the", 6, 0);
            LCD.WriteRC("scene lies their bag.", 7, 0);
                
            FEHIcon::Icon lootMenuButtons[2];
            char lootMenuLabels[2][20] = {"Loot", "Bury"};
            FEHIcon::DrawIconArray (lootMenuButtons, 2, 1, (Y_MAX * 2 / 3), 0, 20, 20, lootMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (lootMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(lootMenuButtons[0], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You fight your humanity", 0, 0);
                LCD.WriteRC("and decide to search the", 1, 0);
                LCD.WriteRC("bag. You found", 2, 0);
                LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 2, 16);
                LCD.WriteRC("to be put to good use", 3, 0);
                LCD.WriteRC("At least they died for", 4, 0);
                LCD.WriteRC("something, you tell", 5, 0);
                LCD.WriteRC("yourself.", 6, 0);

                resourceIncrementer (randomResource1, 1);
                lever = FALSE;

                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
            else if (lootMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(lootMenuButtons[1], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You pick up a nearby", 0, 0);
                LCD.WriteRC("shovel and start to dig.", 1, 0);
                LCD.WriteRC("They deserve better, after", 2, 0);
                LCD.WriteRC("all. Your party follows", 3, 0);
                LCD.WriteRC("your lead. Later that day,", 4, 0);
                LCD.WriteRC("you find a lot of", 5, 0);
                LCD.WriteRC(gameResourcesObject.resourceNamesLowerCase[randomResource1], 5, 19);
                LCD.WriteRC("to be put to good use.", 6, 0);
                LCD.WriteRC("Maybe there is still some", 7 ,0);
                LCD.WriteRC("light in this cruel, cold", 8, 0);
                LCD.WriteRC("world.", 9, 0);

                resourceIncrementer (randomResource1, 2);
                lever = FALSE;

                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
        }
    }

    void hoardEvent ()
    {
        Utility utilityObject;

        int successChance = 1 + (rand() % 100);
        int randomMember1 = findLivingMember();
        int randomMember2 = findSecondLivingMember(randomMember1);
        int charLengthOfMember1 = strlen(partyMembersObject.partyNames[randomMember1]);
        int charLengthOfMember2 = strlen(partyMembersObject.partyNames[randomMember1]);
        int lever = TRUE;

        FEHIcon::Icon hoardMenuButtons[3];
        char hoardMenuLabels[3][20] = {"Fight", "Sneak Through", "Sacrifice "};
        strcat(hoardMenuLabels[2], partyMembersObject.partyNames[randomMember1]);

        utilityObject.clearScreen();

        while (lever == TRUE)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You have encountered a", 0, 0);
            LCD.WriteRC("massive hoard. You try to", 1, 0);
            LCD.WriteRC("remain calm but", 2, 0);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 2, 16);
            LCD.WriteRC("can't stop panicking. They", 3, 0);
            LCD.WriteRC("are putting the entire", 4, 0);
            LCD.WriteRC("party at risk.", 5, 0);

            FEHIcon::DrawIconArray (hoardMenuButtons, 3, 1, (Y_MAX / 2), 0, 20, 20, hoardMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (hoardMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hoardMenuButtons[0], x, y);
                utilityObject.clearScreen();

                if (successChance > 25 && gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Your shot startled the", 0, 0);
                    LCD.WriteRC("hoard. Your party soon", 1, 0);
                    LCD.WriteRC("followed your lead. Even", 2, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 3, 0);
                    LCD.WriteRC("relaxed enough", 3, 1 + charLengthOfMember1);
                    LCD.WriteRC("to aim and fire. Your", 4, 0);
                    LCD.WriteRC("boldness kept your party", 5, 0);
                    LCD.WriteRC("alive. You used 1 bullet.", 6, 0);

                    gameResourcesObject.bullets--;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else if (successChance <= 25 && gameResourcesObject.bullets > 0)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Your shot startled your", 0, 0);
                    LCD.WriteRC("party. Clumsily, the party", 1, 0);
                    LCD.WriteRC("follows your lead, except", 2, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 3, 0);
                    LCD.WriteRC(", who desperately", 3, charLengthOfMember1);
                    LCD.WriteRC("clings to", 4, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 4, 10);
                    LCD.WriteRC("for life.", 4, 11 + charLengthOfMember2);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 5, 0);
                    LCD.WriteRC("'s cowardice gets", 5, charLengthOfMember1);
                    LCD.WriteRC("them both severely wounded.", 6, 0);
                    LCD.WriteRC("Each of them loses half of", 7, 0);
                    LCD.WriteRC("their health. You used 1", 8, 0);
                    LCD.WriteRC("bullet.", 9, 0);

                    partyMembersObject.partyHealth[randomMember1] /= 2;
                    partyMembersObject.partyHealth[randomMember2] /= 2;
                    gameResourcesObject.bullets--;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You have no bullets left", 0, 0);
                    LCD.WriteRC("in the chamber.", 1, 0);

                    utilityObject.printReturnButton();
                    utilityObject.clearScreen();
                }
            }
            else if (hoardMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hoardMenuButtons[1], x, y);
                utilityObject.clearScreen();

                if (successChance > 50)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You mutilate a lone zombie", 0, 0);
                    LCD.WriteRC("with your bare hands. By", 1, 0);
                    LCD.WriteRC("spreading their guts all", 2, 0);
                    LCD.WriteRC("over your party and", 3, 0);
                    LCD.WriteRC("and vehicle, you safely", 4, 0);
                    LCD.WriteRC("sneak by the hoard.", 5, 0);

                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("You try to kidnap a lone", 0, 0);
                    LCD.WriteRC("zombie to use their guts", 1, 0);
                    LCD.WriteRC("for camoflauge, but it", 2, 0);
                    LCD.WriteRC("lunges at", 3, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 3, 10);
                    LCD.WriteRC(", injuring", 3, 10 + charLengthOfMember1);
                    LCD.WriteRC("them severely. Eventually,", 4, 0);
                    LCD.WriteRC("you win the fight, and use", 5, 0);
                    LCD.WriteRC("the guts to safely sneak", 6, 0);
                    LCD.WriteRC("by the hoard.", 7, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 7, 14);
                    LCD.WriteRC("loses", 7, 15 + charLengthOfMember1);
                    LCD.WriteRC("half of their health.", 8, 0);

                    partyMembersObject.partyHealth[randomMember1] /= 2;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
            }
            else if (hoardMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hoardMenuButtons[2], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("You open a car door", 0, 0);
                LCD.WriteRC("and shove", 1, 0);
                LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 1, 10);
                LCD.WriteRC("out of", 1, 11 + charLengthOfMember1);
                LCD.WriteRC("the moving car. Their", 2, 0);
                LCD.WriteRC("screams are buried as the", 3, 0);
                LCD.WriteRC("zombies feast on the fresh", 4, 0);
                LCD.WriteRC("meat. The distraction", 5, 0);
                LCD.WriteRC("allows for safe passage,", 6, 0);
                LCD.WriteRC("to the horror of your", 7, 0);
                LCD.WriteRC("party.", 8, 0);
                LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 8, 7);
                LCD.WriteRC("is dead.", 8, 8 + charLengthOfMember1);

                partyMembersObject.partyHealth[randomMember1] = DEAD;
                lever = FALSE;

                while (!LCD.Touch(&x, &y)) {}
                while (LCD.Touch(&xTrash, &yTrash)) {}
            }
        }
    }

    void partyMemberFightEvent ()
    {
        Utility utilityObject;

        int successChance = 1 + (rand() % 100);
        int randomMember1 = findLivingMember();
        int randomMember2 = findSecondLivingMember(randomMember1);
        int charLengthOfMember1 = strlen(partyMembersObject.partyNames[randomMember1]);
        int charLengthOfMember2 = strlen(partyMembersObject.partyNames[randomMember2]);
        int lever = TRUE;

        FEHIcon::Icon partyMemberFightMenuButtons[3];
        char partyMemberFightMenuLabels[2][20] = {"Side with ", "Side with "};
        strcat(partyMemberFightMenuLabels[0], partyMembersObject.partyNames[randomMember1]);
        strcat(partyMemberFightMenuLabels[1], partyMembersObject.partyNames[randomMember2]);

        utilityObject.clearScreen();

        while (lever == TRUE)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 0, 0);
            LCD.WriteRC("accuses", 0, 1 + charLengthOfMember1);
            LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 0, 9 + charLengthOfMember1);
            LCD.WriteRC("of", 0, 10 + charLengthOfMember1 + charLengthOfMember2);
            LCD.WriteRC("stealing their rations.", 1, 0);
            LCD.WriteRC("Chaos is unleashed as", 2, 0);
            LCD.WriteRC("they both draw weapons.", 3, 0);

            FEHIcon::DrawIconArray (partyMemberFightMenuButtons, 2, 1, (Y_MAX / 2), 0, 20, 20, partyMemberFightMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (partyMemberFightMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(partyMemberFightMenuButtons[0], x, y);
                utilityObject.clearScreen();

                if (successChance >= 50)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Siding with", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 0, 12);
                    LCD.WriteRC("leads", 0, 13 + charLengthOfMember1);
                    LCD.WriteRC("them to pull the trigger.", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 2, 0);
                    LCD.WriteRC("dies instantly.", 2, 1 + charLengthOfMember2);
                    LCD.WriteRC("After searching their", 3, 0);
                    LCD.WriteRC("corpse, you find 15", 4, 0);
                    LCD.WriteRC("stolen food and water.", 5, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 6, 0);
                    LCD.WriteRC("was truthful.", 6, 1 + charLengthOfMember1);

                    partyMembersObject.partyHealth[randomMember2] = DEAD;
                    gameResourcesObject.food += 15;
                    gameResourcesObject.water += 15;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Siding with", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 0, 12);
                    LCD.WriteRC("calms", 0, 13 + charLengthOfMember1);
                    LCD.WriteRC("the tension.", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 1, 13);
                    LCD.WriteRC("admits", 1, 14 + charLengthOfMember2);
                    LCD.WriteRC("they stole the rations,", 2, 0);
                    LCD.WriteRC("and returns them.", 3, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 3, 18);
                    LCD.WriteRC("gains some health after", 4, 0);
                    LCD.WriteRC("eating their rations.", 5, 0);

                    partyMembersObject.partyHealth[randomMember1] += (MAX_HEALTH - partyMembersObject.partyHealth[randomMember1]) / 2;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
            }
            else if (partyMemberFightMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(partyMemberFightMenuButtons[1], x, y);
                utilityObject.clearScreen();

                if (successChance >= 50)
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Siding with", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 0, 12);
                    LCD.WriteRC("leads", 0, 13 + charLengthOfMember2);
                    LCD.WriteRC("them to pull the trigger.", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 2, 0);
                    LCD.WriteRC("dies instantly.", 2, 1 + charLengthOfMember1);
                    LCD.WriteRC("After searching their", 3, 0);
                    LCD.WriteRC("corpse, you find 15", 4, 0);
                    LCD.WriteRC("unstolen food and water.", 5, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 6, 0);
                    LCD.WriteRC("was lying.", 6, 1 + charLengthOfMember1);

                    partyMembersObject.partyHealth[randomMember1] = DEAD;
                    gameResourcesObject.food += 15;
                    gameResourcesObject.water += 15;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
                else
                {
                    LCD.SetFontColor(WHITE);
                    LCD.WriteRC("Siding with", 0, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 0, 12);
                    LCD.WriteRC("calms", 0, 13 + charLengthOfMember2);
                    LCD.WriteRC("the tension.", 1, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 1, 13);
                    LCD.WriteRC("admits to", 1, 14 + charLengthOfMember1);
                    LCD.WriteRC("fabricating the story,", 2, 0);
                    LCD.WriteRC("and begs for forgiveness.", 3, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 4, 0);
                    LCD.WriteRC("shows mercy, but", 4, 0);
                    LCD.WriteRC("takes some rations from", 5, 0);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember1], 6, 18);
                    LCD.WriteRC(".", 6, 19 + charLengthOfMember1);
                    LCD.WriteRC(partyMembersObject.partyNames[randomMember2], 7, 0);
                    LCD.WriteRC("gains some health", 7, 1 + charLengthOfMember1);
                    LCD.WriteRC("after eating the rations.", 8, 0);

                    partyMembersObject.partyHealth[randomMember2] += (MAX_HEALTH - partyMembersObject.partyHealth[randomMember2]) / 2;
                    lever = FALSE;

                    while (!LCD.Touch(&x, &y)) {}
                    while (LCD.Touch(&xTrash, &yTrash)) {}
                }
            }
        }
    }
};

class Turns
{
    private:
    int x, y, xTrash, yTrash;
    
    public:
    void checkForWinLoss ()
    {
        Utility utilityObject;

        if (dateObject.currentMonth >= dateObject.endMonth && dateObject.currentYear >= dateObject.endYear)
        {
            utilityObject.clearScreen ();

            FEHImage win;
            win.Open("win.pic");
            win.Draw(0, 0);
            win.Close();

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            utilityObject.clearScreen ();

            mainMenuStatsObject.wins++;
            onOffLeversObject.gameplayLever = FALSE;
        }
        
        if (calculateNumberLivingPartyMembers() == 0)
        {
            utilityObject.clearScreen ();

            FEHImage loss;
            loss.Open("loss.pic");
            loss.Draw(0, 0);
            loss.Close();

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            utilityObject.clearScreen ();

            mainMenuStatsObject.losses++;
            onOffLeversObject.gameplayLever = FALSE;
        }
    }

    void cycleDayBackdrop ()
    {
        cyclicalTrackersObject.dayCycleTracker++;

        if (cyclicalTrackersObject.dayCycleTracker > 21)
        {
            cyclicalTrackersObject.dayCycleTracker = 1;
        }

        if (cyclicalTrackersObject.dayCycleTracker > 0 && cyclicalTrackersObject.dayCycleTracker <= 7)
        {
            cyclicalTrackersObject.recentDayCycle = 1;
        }
        else if (cyclicalTrackersObject.dayCycleTracker > 7 && cyclicalTrackersObject.dayCycleTracker <= 14)
        {
            cyclicalTrackersObject.recentDayCycle = 2;
        }
        else if (cyclicalTrackersObject.dayCycleTracker > 14 && cyclicalTrackersObject.dayCycleTracker <= 21)
        {   
            cyclicalTrackersObject.recentDayCycle = 3;  
        }
    }

    void printRecentBackdrop ()
    {
        FEHImage day_backdrop;
        FEHImage evening_backdrop;
        FEHImage night_backdrop;

        switch (cyclicalTrackersObject.recentDayCycle)
        {
            case 1:
            day_backdrop.Open("day_backdrop.pic");
            day_backdrop.Draw(0, 0);
            day_backdrop.Close();
            break;

            case 2:
            evening_backdrop.Open("evening_backdrop.pic");
            evening_backdrop.Draw(0, 0);
            evening_backdrop.Close();
            break;

            case 3:
            night_backdrop.Open("night_backdrop.pic");
            night_backdrop.Draw(0, 0);
            night_backdrop.Close();  
            break;
        }
    }

    void turnIncrementer ()
    {
        if (gameResourcesObject.fuel > 0)
        {
            gameResourcesObject.fuel--;
            dateObject.currentDay += incrementersObject.pace;

            if (dateObject.currentDay > 30)
            {
                dateObject.currentDay -= 30;
                dateObject.currentMonth++;
            }

            if (dateObject.currentMonth > 12)
            {
                dateObject.currentMonth -= 12;
                dateObject.currentYear++;
            }

            if (incrementersObject.pace == 9)
            {
                decrementPartyHealth(1);
            }
            else if (incrementersObject.pace == 5)
            {
                incrementPartyHealth(1);
            }
        }

        if (gameResourcesObject.food >= (incrementersObject.foodRations * calculateNumberLivingPartyMembers()))
        {
            gameResourcesObject.food -= incrementersObject.foodRations * calculateNumberLivingPartyMembers();
            checkForNegativeResources(gameResourcesObject.food);

            if (incrementersObject.foodRations == 1.5)
            {
                incrementPartyHealth(1);
            }
            else if (incrementersObject.foodRations == 0.5)
            {
                decrementPartyHealth(1);
            }
        }
        else
        {
            decrementPartyHealth(3);
        }

        if (gameResourcesObject.water >= (incrementersObject.waterRations * calculateNumberLivingPartyMembers()))
        {
            gameResourcesObject.water -= incrementersObject.waterRations * calculateNumberLivingPartyMembers();
            checkForNegativeResources(gameResourcesObject.water);

            if (incrementersObject.waterRations == 1.5)
            {
                incrementPartyHealth(1);
            }
            else if (incrementersObject.waterRations == 0.5)
            {
                decrementPartyHealth(1);
            }
        }
        else
        {
            decrementPartyHealth(3);
        }
    }

    void decrementPartyHealth (float decrementer)
    {
        for (int index = 0; index < 5; index++)
        {
            if (partyMembersObject.partyHealth[index] != DEAD)
            {
                partyMembersObject.partyHealth[index] -= decrementer;

                if (partyMembersObject.partyHealth[index] < 0)
                {
                    partyMembersObject.partyHealth[index] = DEAD;
                }
            }
        }
    }

    void incrementPartyHealth (float incrementer)
    {
        for (int index = 0; index < 5; index++)
        {
            if (partyMembersObject.partyHealth[index] != DEAD)
            {
                partyMembersObject.partyHealth[index] += incrementer;

                if (partyMembersObject.partyHealth[index] > partyMembersObject.maximumPartyHealth[index])
                {
                    partyMembersObject.partyHealth[index] = partyMembersObject.maximumPartyHealth[index];
                }
            }
        }
    }

    void checkForNegativeResources (int resource)
    {
        if (resource < 0)
        {
            resource = 0;
        }
    }

    int calculateNumberLivingPartyMembers ()
    {
        int numberOfLivingPartyMembers = 0;

        for (int index = 0; index < 5; index++)
        {
            if (partyMembersObject.partyHealth[index] != DEAD)
            {
                numberOfLivingPartyMembers++;
            }
        }

        return numberOfLivingPartyMembers;
    }
};

class GameMenu
{
    private:
    int x, y, xTrash, yTrash;

    public:
    void printHealMenu ()
    {
        Utility utilityObject;

        FEHIcon::Icon healMemberMenuButtons[6];
        char healMemberMenuLabels[6][20] = {"Heal ", "Heal ", "Heal ", "Heal ", "Heal ", "Return"};
        int lever = TRUE;

        for (int index = 0; index < 5; index++)
        {
            strcat(healMemberMenuLabels[index], partyMembersObject.partyNames[index]);
        }

        utilityObject.clearScreen();
        
        while (lever == TRUE)
        {
            FEHIcon::DrawIconArray(healMemberMenuButtons, 6, 1, 20, 20, 20, 20, healMemberMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (healMemberMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[0], x, y);
                healMenuButtons(0);
            }
            else if (healMemberMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[1], x, y);
                healMenuButtons(1);
            }
            else if (healMemberMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[2], x, y);
                healMenuButtons(2);
            }
            else if (healMemberMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[3], x, y);
                healMenuButtons(3);
            }
            else if (healMemberMenuButtons[4].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[4], x, y);
                healMenuButtons(4);
            }
            else if (healMemberMenuButtons[5].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(healMemberMenuButtons[5], x, y);
                utilityObject.clearScreen();

                lever = FALSE;
            }
        }  
    }

    void healMenuButtons (int index)
    {
        Utility utilityObject;

        utilityObject.clearScreen ();

        if (partyMembersObject.partyHealth[index] > DEAD && partyMembersObject.partyHealth[index] < MAX_HEALTH && gameResourcesObject.meds > 0)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You used 1 med to heal", 0, 0);
            LCD.WriteRC(partyMembersObject.partyNames[index], 1, 0);
            LCD.WriteRC(".", 1, strlen(partyMembersObject.partyNames[index]));

            partyMembersObject.partyHealth[index] = partyMembersObject.maximumPartyHealth[index];
            gameResourcesObject.meds--;

            utilityObject.printReturnButton();
        }
        else if (partyMembersObject.partyHealth[index] > DEAD && partyMembersObject.partyHealth[index] < MAX_HEALTH && gameResourcesObject.meds == 0)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You do not have any meds", 0, 0);
            LCD.WriteRC("available to heal", 1, 0);
            LCD.WriteRC(partyMembersObject.partyNames[index], 1, 18);
            LCD.WriteRC(".", 1, 18 + strlen(partyMembersObject.partyNames[index]));

            utilityObject.printReturnButton();
        }
        else if (partyMembersObject.partyHealth[index] == partyMembersObject.maximumPartyHealth[index])
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC(partyMembersObject.partyNames[index], 0, 0);
            LCD.WriteRC("already has max", 0, 1 + strlen(partyMembersObject.partyNames[index]));
            LCD.WriteRC("health.", 1, 0);

            utilityObject.printReturnButton();
        }
        else if (partyMembersObject.partyHealth[index] == DEAD)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You cannot bring", 0, 0);
            LCD.WriteRC(partyMembersObject.partyNames[index], 0, 17);
            LCD.WriteRC("back to life.", 1, 0);

            utilityObject.printReturnButton();
        }

        utilityObject.clearScreen ();
    }

    void printRestMessage ()
    {
        Utility utilityObject;

        utilityObject.clearScreen();

        if (gameResourcesObject.fuel > 2)
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("Your party is well rested.", 0, 0);
            LCD.WriteRC("Each regained half of", 1, 0);
            LCD.WriteRC("their health. You used 3", 2, 0);
            LCD.WriteRC("fuel.", 3, 0); 

            for (int index = 0 ; index < 5; index++)
            {
                partyMembersObject.partyHealth[index] += (MAX_HEALTH - partyMembersObject.partyHealth[index]) / 2;
            }
            gameResourcesObject.fuel -= 3;

            utilityObject.printReturnButton();
        }
        else
        {
            LCD.SetFontColor(WHITE);
            LCD.WriteRC("You need at least 3 fuel", 0, 0);
            LCD.WriteRC("to rest.", 1, 0);

            utilityObject.printReturnButton();
        }

        utilityObject.clearScreen();
    }

    void printPaceMenu () 
    {
        Utility utilityObject;

        FEHIcon::Icon paceMenuButtons[4];
        char pace_menu_labels[4][20] = {"Quick", "Steady", "Slow", "Return"};
        int lever = TRUE;

        utilityObject.clearScreen ();

        while (lever == TRUE)
        {
            FEHIcon::DrawIconArray(paceMenuButtons, 4, 1, 20, 20, 20, 20, pace_menu_labels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (paceMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(paceMenuButtons[0], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Your party will advance 9", 0, 0);
                LCD.WriteRC("days for each fuel. The", 1, 0);
                LCD.WriteRC("quick pace will decay", 2, 0); 
                LCD.WriteRC("their health.", 3, 0);

                incrementersObject.pace = 9;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (paceMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(paceMenuButtons[1], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Your party will advance 7", 0, 0);
                LCD.WriteRC("days for each fuel.", 1, 0);

                incrementersObject.pace = 7;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (paceMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(paceMenuButtons[2], x, y);
                utilityObject.clearScreen();

                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Your party will advance 5", 0, 0);
                LCD.WriteRC("days for each fuel. The", 1, 0);
                LCD.WriteRC("slow pace will grow", 2, 0); 
                LCD.WriteRC("their health.", 3, 0);

                incrementersObject.pace = 5;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (paceMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(paceMenuButtons[3], x, y);
                utilityObject.clearScreen();

                lever = FALSE;
            }
        }
    }

    void printFoodRationMenu () 
    {
        Utility utilityObject;

        FEHIcon::Icon foodMenuButtons[4];
        char foodMenuLabels[4][20] = {"Gluttonous", "Filling", "Meager", "Return"};
        int lever = TRUE;

        utilityObject.clearScreen();

        while (lever == TRUE)
        {
            FEHIcon::DrawIconArray(foodMenuButtons, 4, 1, 20, 20, 20, 20, foodMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (foodMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(foodMenuButtons[0], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will eat", 0, 0);
                LCD.WriteRC("1.5 food each turn. The", 1, 0);
                LCD.WriteRC("gluttonous portions will", 2, 0); 
                LCD.WriteRC("grow their health.", 3, 0);

                incrementersObject.foodRations = 1.5;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (foodMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(foodMenuButtons[0], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will eat", 0, 0);
                LCD.WriteRC("1 food each turn.", 1, 0);

                incrementersObject.foodRations = 1;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (foodMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(foodMenuButtons[0], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will eat", 0, 0);
                LCD.WriteRC("0.5 food each turn. The", 1, 0);
                LCD.WriteRC("meager portions will decay", 2, 0); 
                LCD.WriteRC("their health.", 3, 0);

                incrementersObject.foodRations = 0.5;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (foodMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(foodMenuButtons[3], x, y);
                utilityObject.clearScreen();

                lever = FALSE;
            }
        }
    }

    void printWaterRationMenu () 
    {
        Utility utilityObject;

        FEHIcon::Icon waterMenuButtons[4];
        char waterMenuLabels[4][20] = {"Drowning", "Hydrated", "Parched", "Return"};
        int lever = TRUE;

        utilityObject.clearScreen();

        while (lever == TRUE)
        {
            FEHIcon::DrawIconArray(waterMenuButtons, 4, 1, 20, 20, 20, 20, waterMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (waterMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(waterMenuButtons[0], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will", 0, 0);
                LCD.WriteRC("drink 1.5 water each turn.", 1, 0);
                LCD.WriteRC("The drowning portions will", 2, 0); 
                LCD.WriteRC("grow their health.", 3, 0);

                incrementersObject.waterRations = 1.5;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (waterMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(waterMenuButtons[1], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will", 0, 0);
                LCD.WriteRC("drink 1 water each turn.", 1, 0);

                incrementersObject.waterRations = 1;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (waterMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(waterMenuButtons[2], x, y);
                utilityObject.clearScreen();
                
                LCD.SetFontColor(WHITE);
                LCD.WriteRC("Each party member will", 0, 0);
                LCD.WriteRC("drink 0.5 water each turn.", 1, 0);
                LCD.WriteRC("The parched portions will", 2, 0); 
                LCD.WriteRC("decay their health.", 3, 0);

                incrementersObject.waterRations = 0.5;

                utilityObject.printReturnButton();
                utilityObject.clearScreen();
            }
            else if (waterMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(waterMenuButtons[3], x, y);
                utilityObject.clearScreen();

                lever = FALSE;
            }
        }
    }
};

class GameHudButtons
{
    private:
    int x, y, xTrash, yTrash;

    public:
    void healthButton ()
    {
        for (int index = 0; index < 5; index++)
        {
            int yCoordBarOutline = 132 + 23 * index; 
            int yCoordBarColor = 133 + 23 * index;

            if (partyMembersObject.partyHealth[index] >= (partyMembersObject.maximumPartyHealth[index] * 3 / 4))
            {
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(partyMembersObject.partyNames[index], 0, yCoordBarOutline);
                LCD.DrawRectangle(80, yCoordBarOutline, partyMembersObject.maximumPartyHealth[index] + 2, 14);

                LCD.SetFontColor(GREEN);
                LCD.FillRectangle(81, yCoordBarColor, partyMembersObject.partyHealth[index], 12);
            }
            else if (partyMembersObject.partyHealth[index] >= (partyMembersObject.maximumPartyHealth[index] / 4) && 
            partyMembersObject.partyHealth[index] < (partyMembersObject.maximumPartyHealth[index] * 3 / 4))
            {
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(partyMembersObject.partyNames[index], 0, yCoordBarOutline);
                LCD.DrawRectangle(80, yCoordBarOutline, partyMembersObject.maximumPartyHealth[index] + 2, 14);

                LCD.SetFontColor(YELLOW);
                LCD.FillRectangle(81, yCoordBarColor, partyMembersObject.partyHealth[index], 12);
            }
            else if (partyMembersObject.partyHealth[index] > DEAD && 
            partyMembersObject.partyHealth[index] < (partyMembersObject.maximumPartyHealth[index]/ 4))
            {
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(partyMembersObject.partyNames[index], 0, yCoordBarOutline);
                LCD.DrawRectangle(80, yCoordBarOutline, partyMembersObject.maximumPartyHealth[index] + 2, 14);

                LCD.SetFontColor(RED);
                LCD.FillRectangle(81, yCoordBarColor, partyMembersObject.partyHealth[index], 12);
            }
            else
            {
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(partyMembersObject.partyNames[index], 0, yCoordBarOutline);
                LCD.WriteAt("DEAD", 120, yCoordBarOutline);
            }
        }
    }

    void suppliesButton ()
    {
        LCD.SetFontColor(BLACK);

        LCD.WriteAt("Cigs:", 36, 131);
        LCD.WriteAt(gameResourcesObject.cigs, 94, 131);

        LCD.WriteAt("Fuel:", 36, 150);
        LCD.WriteAt(gameResourcesObject.fuel, 94, 150);

        LCD.WriteAt("Food:", 36, 169);
        LCD.WriteAt(gameResourcesObject.food, 94, 169);

        LCD.WriteAt("Water:", 24, 187);
        LCD.WriteAt(gameResourcesObject.water, 94, 187);

        LCD.WriteAt("Meds:", 36, 206);
        LCD.WriteAt(gameResourcesObject.meds, 94, 206);

        LCD.WriteAt("Bullets:", 0, 225);
        LCD.WriteAt(gameResourcesObject.bullets, 94, 225);   
    }

    void scoutButton ()
    {
        LCD.SetFontColor(BLACK);

        LCD.WriteAt("Date:", 0, 131);
        LCD.WriteAt(dateObject.currentMonth, 58, 131);
        LCD.WriteAt("/", 83, 131);
        LCD.WriteAt(dateObject.currentDay, 96, 131);
        LCD.WriteAt("/", 121, 131);
        LCD.WriteAt(dateObject.currentYear, 133, 131);

        LCD.WriteAt("End:", 12, 155);
        LCD.WriteAt(dateObject.endMonth, 58, 155);
        LCD.WriteAt("/", 83, 155);
        LCD.WriteAt(dateObject.endDay, 96, 155);
        LCD.WriteAt("/", 121, 155);
        LCD.WriteAt(dateObject.endYear, 133, 155);

        LCD.WriteAt("Pace:", 96, 178);
        LCD.WriteAt(incrementersObject.pace, 154, 178);

        LCD.WriteAt("Food Ration:", 12, 201);
        if (incrementersObject.foodRations == 1.5)
        {
            LCD.WriteAt("1.5", 153, 201);    
        }
        else if (incrementersObject.foodRations == 1)
        {
            LCD.WriteAt("1", 153, 201);    
        }
        else if (incrementersObject.foodRations == 0.5)
        {
            LCD.WriteAt("0.5", 153, 201);    
        }

        LCD.WriteAt("Water Ration:", 0, 225);
        if (incrementersObject.waterRations == 1.5)
        {
            LCD.WriteAt("1.5", 153, 225);    
        }
        else if (incrementersObject.waterRations == 1)
        {
            LCD.WriteAt("1", 153, 225);    
        }
        else if (incrementersObject.waterRations == 0.5)
        {
            LCD.WriteAt("0.5", 153, 225);    
        }
    }

    void stopButton ()
    {
        Utility utilityObject;
        GameMenu gameMenuObject;
        Turns turnsObject;
        Shop shopObject;

        FEHIcon::Icon stopMenuButtons[7];
        char stopMenuLabels[7][20] = {"Stop to Rest", "Set Pace", "Set Food Rations", 
        "Set Water Rations", "Heal Party Member", "Visit Shop", "Return"};

        utilityObject.clearScreen ();

        int stopLever = TRUE;

        while (stopLever == TRUE)
        {
            FEHIcon::DrawIconArray(stopMenuButtons, 7, 1, 20, 20, 20, 20, stopMenuLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (stopMenuButtons[0].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[0], x, y);

                gameMenuObject.printRestMessage();
            }
            else if (stopMenuButtons[1].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[1], x, y);

                gameMenuObject.printPaceMenu();
            }
            else if (stopMenuButtons[2].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[2], x, y);

                gameMenuObject.printFoodRationMenu();
            }
            else if (stopMenuButtons[3].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[3], x, y);

                gameMenuObject.printWaterRationMenu();
            }
            else if (stopMenuButtons[4].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[4], x, y);

                gameMenuObject.printHealMenu();
            }
            else if (stopMenuButtons[5].Pressed(x, y, 0)) 
            {
                utilityObject.highlightPressedButton(stopMenuButtons[5], x, y);
                
                shopObject.printShopInterface();
            }
            else if (stopMenuButtons[6].Pressed(x, y, 0))  
            {
                utilityObject.highlightPressedButton(stopMenuButtons[6], x, y);
                utilityObject.clearScreen();

                stopLever = FALSE;
            }
        }
    }
};

class Gameplay
{
    private:
    int x, y, xTrash, yTrash;
    int recentButton;

    public:
    Gameplay()
    {
        recentButton = 1;
    }

    void initializeGameVariables ()
    {
        GameEvents gameEventsObject;

        gameResourcesObject.fuel = 0;
        gameResourcesObject.food = 0;
        gameResourcesObject.water = 0;
        gameResourcesObject.meds = 0;
        gameResourcesObject.bullets = 0;

        partyMembersObject.survivorCount = 29;

        for (int index = 0; index < 5; index++)
        {
            partyMembersObject.maximumPartyHealth[index] = MAX_HEALTH;
            partyMembersObject.partyHealth[index] = MAX_HEALTH;

            int randomSurvivor = rand() % partyMembersObject.survivorCount;
            strcpy(partyMembersObject.partyNames[index], partyMembersObject.survivors[randomSurvivor]);

            gameEventsObject.sortUsedSurvivor(randomSurvivor);
        }

        incrementersObject.pace = 7;
        incrementersObject.foodRations = 1;
        incrementersObject.waterRations = 1;

        dateObject.currentMonth = 10;
        dateObject.currentDay = 31;
        dateObject.currentYear = 10;
        dateObject.endMonth = 10;
        dateObject.endDay = 31;

        cyclicalTrackersObject.recentDayCycle = 1;
        cyclicalTrackersObject.dayCycleTracker = 1;

        onOffLeversObject.gameplayLever = TRUE;
    };

    void userSetGameDifficulty ()
    {
        Utility utilityObject;
        utilityObject.clearScreen ();

        int lever = TRUE;

        while (lever == TRUE)
        {
            FEHIcon::Icon difficultyMenu[3];
            char difficultyLabels[3][20] = {"Novice", "Expert", "Nightmare"};
            FEHIcon::DrawIconArray (difficultyMenu, 3, 1, 20, 20, 20, 20, difficultyLabels, WHITE, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (difficultyMenu[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(difficultyMenu[0], x, y);
                
                gameResourcesObject.cigs = 1000;
                dateObject.endYear = 11;

                lever = FALSE;
            }
            else if (difficultyMenu[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(difficultyMenu[1], x, y);
                
                gameResourcesObject.cigs = 750;
                dateObject.endYear = 12;

                lever = FALSE;
            }
            else if (difficultyMenu[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(difficultyMenu[2], x, y);
                
                gameResourcesObject.cigs = 500;
                dateObject.endYear = 13;

                lever = FALSE;
            }
        }
    }

    void printGameInterface ()
    {   
        Utility utilityObject;
        GameHudButtons gameHudObject;
        Turns turnsObject;
        GameEvents gameEventsObject;

        FEHIcon::Icon hudButtons[5];
        char hudLabels[5][20] = {"Advance", "Health", "Supplies", "Scout", "Stop"};
        int tumbleweedPosition = 0;
        int zombiehandPosition = 160;
        int carPosition = 1;

       while (onOffLeversObject.gameplayLever == TRUE)
        {
            /*int checkforTouchLever = TRUE;
            FEHImage tumbleweed;
            FEHImage zombiehand;
            tumbleweed.Open("tumbleweed.pic");
            zombiehand.Open("zombiehand.pic");

            while (checkforTouchLever == TRUE)
            {
                if (!LCD.Touch(&x, &y))
                {
                    turnsObject.printRecentBackdrop();

                    if (carPosition == 1)
                    {
                        FEHImage car;
                        car.Open("car.pic");
                        car.Draw(107, 23);
                        car.Close();
                        //carPosition++;
                    }
                    else
                    {
                        FEHImage car2;
                        car2.Open("car2.pic");
                        car2.Draw(107, 23);
                        car2.Close();
                        carPosition = 1;
                    }

                    FEHImage wood_hud_backdrop;
                    wood_hud_backdrop.Open("wood_hud_backdrop.pic");
                    wood_hud_backdrop.Draw(0, 130);
                    wood_hud_backdrop.Close();

                    LCD.SetFontColor(BLACK);
                    LCD.DrawHorizontalLine(129, 0, X_MAX);

                    FEHIcon::DrawIconArray(hudButtons, 5, 1, 130, 0, 215, 0, hudLabels, BLACK, DARKRED);

                    switch (recentButton) 
                    {
                        case 1: 
                        gameHudObject.healthButton();
                        break;
                    
                        case 2: 
                        gameHudObject.suppliesButton();
                        break;

                        case 3: 
                        gameHudObject.scoutButton();
                        break;
                    }

                    tumbleweed.Draw(tumbleweedPosition, 75);
                    tumbleweedPosition++;

                    //zombiehand.Draw(zombiehandPosition, 75);
                    zombiehandPosition++;
                }

                if (LCD.Touch(&xTrash,&yTrash))
                {
                    checkforTouchLever = FALSE;
                }
            }

            tumbleweed.Close();
            zombiehand.Close();
        }*/

            turnsObject.printRecentBackdrop();

            FEHImage car;
            car.Open("car.pic");
            car.Draw(107, 23);
            car.Close();

            FEHImage wood_hud_backdrop;
            wood_hud_backdrop.Open("wood_hud_backdrop.pic");
            wood_hud_backdrop.Draw(0, 130);
            wood_hud_backdrop.Close();

            LCD.SetFontColor(BLACK);
            LCD.DrawHorizontalLine(129, 0, X_MAX);

            FEHIcon::DrawIconArray(hudButtons, 5, 1, 130, 0, 215, 0, hudLabels, BLACK, DARKRED);

            switch (recentButton) 
            {
                case 1: 
                gameHudObject.healthButton();
                break;
            
                case 2: 
                gameHudObject.suppliesButton();
                break;

                case 3: 
                gameHudObject.scoutButton();
                break;
            }

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (hudButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hudButtons[0], x, y);

                gameEventsObject.eventGenerator();
                turnsObject.cycleDayBackdrop();
                turnsObject.turnIncrementer();
                LCD.Update();
            }
            else if (hudButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hudButtons[1], x, y);

                gameHudObject.healthButton();

                recentButton = 1;
            }
            else if (hudButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hudButtons[2], x, y);

                gameHudObject.suppliesButton ();

                recentButton = 2;
            }
            else if (hudButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hudButtons[3], x, y);

                gameHudObject.scoutButton ();

                recentButton = 3;
            }
            else if (hudButtons[4].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(hudButtons[4], x, y);

                gameHudObject.stopButton ();
            }

            turnsObject.checkForWinLoss();
        }
    }
};

class MainMenu
{
    private:
    int x, y, xTrash, yTrash;

    public:
    void playIntro ()
    {
        FEHImage intro;
        intro.Open("intro.pic");
        intro.Draw(0, 0);
        intro.Close();

        Sleep (5.0); 
    }

    void printMenuInterface ()
    {
        Utility utilityObject;

        int mainMenuLever = TRUE;

        while (mainMenuLever == TRUE)
        {
            FEHImage menu_backdrop;
            menu_backdrop.Open("menu_backdrop.pic");
            menu_backdrop.Draw(0, 0);
            menu_backdrop.Close();

            FEHIcon::Icon mainMenuButtons[4];
            char mainMenuLabels[4][20] = {"Play", "Stats", "Credits", "Quit"};
            FEHIcon::DrawIconArray(mainMenuButtons, 4, 1, 40, 65, 90, 90, mainMenuLabels, BLACK, DARKRED);

            while (!LCD.Touch(&x, &y)) {}
            while (LCD.Touch(&xTrash, &yTrash)) {}

            if (mainMenuButtons[0].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(mainMenuButtons[0], x, y);

                mainMenuLever = FALSE;

                startGame();
            }
            else if (mainMenuButtons[1].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(mainMenuButtons[1], x, y);

                showStats();
            }
            else if (mainMenuButtons[2].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(mainMenuButtons[2], x, y);

                showCredits();
            }
            else if (mainMenuButtons[3].Pressed(x, y, 0))
            {
                utilityObject.highlightPressedButton(mainMenuButtons[3], x, y);

                mainMenuLever = FALSE;
                onOffLeversObject.programLever = FALSE;
            }
        }
    }

    void startGame ()
    {
        Utility utilityObject;
        Gameplay gameplayObject;
        Shop shopObject;
        Turns turnsObject;

        gameplayObject.initializeGameVariables();
        gameplayObject.userSetGameDifficulty();
        shopObject.printShopInterface();
        gameplayObject.printGameInterface();
    }

    void showStats ()
    {
        Utility utilityObject;
        utilityObject.clearScreen ();

        LCD.SetFontColor(WHITE);
        LCD.WriteRC("Wins:", 0, 2);
        LCD.WriteRC(mainMenuStatsObject.wins, 0, 7);
        LCD.WriteRC("Losses:", 1, 0);
        LCD.WriteRC(mainMenuStatsObject.losses, 1 , 7);

        utilityObject.printReturnButton ();
    }

    void showCredits ()
    {
        Utility utilityObject;
        utilityObject.clearScreen ();
        
        LCD.SetFontColor(WHITE);
        LCD.WriteRC("Developed By:", 0 , 0);
        LCD.WriteRC("Nicholas McCracken", 1, 0);
        LCD.WriteRC("Abbey Kashani Motlagh", 2, 0);
        LCD.WriteRC("Special Thanks To:", 4, 0);
        LCD.WriteRC("Dr K. & TA's", 5, 0);
        LCD.WriteRC("Credits To:", 7, 0);
        LCD.WriteRC("u.osu.edu/fehproteus", 8 , 0);
        LCD.WriteRC("cplusplus.com", 9 , 0);
        LCD.WriteRC("visitoregon.com", 10 , 0);

        utilityObject.printReturnButton ();
    }
};

int main () 
{
    MainMenu mainMenuObject;
    mainMenuObject.playIntro();

    while (onOffLeversObject.programLever == TRUE) 
    {
        mainMenuObject.printMenuInterface ();
    }
    return FALSE;
}