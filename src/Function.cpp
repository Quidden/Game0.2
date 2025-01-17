#pragma once
#include "Function.h"
#include <iostream>
#include <conio.h>
#include "Person.h"
#include <Windows.h>
#include "Inventory.h"
#include "Untill.h"
#include <limits>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
using namespace std;
bool ErrorOutput(std::string error)
{
    system("cls");
    std::cout << RED << error << RESET;
    std::cin.get();
    return true;
}

void Inventory(PlayerC &Player)
{
    while (true)
    {

        Player.DisplayPlayerInventory();
        cout << "What do you want to do?" << endl;
        cout << "1.Equip an item   2.Use item   3.Sell an item   4.Exit inventory" << endl;

        int choice1;
        cin >> choice1;

        if (choice1 == 4)
        {
            system("cls");
            return;
        }
        if (choice1 > 4 || choice1 < 1)
        {
            ErrorOutput("invalid index");
            continue;
        }

        system("cls");
        Player.DisplayPlayerInventory();

        cout << "Select the item you need by its number" << endl;
        int index;
        cin >> index;

        if (choice1 == 1)
        {
            EquipResult result = Player.EquipError(index);
            Player.EquipError(index);
            if(result.success)
            {
                system("cls");
                continue;
            }
            Player.EquipAction(result.Item, index);
            break;
        }
        if (choice1 == 2)
        {
            if(!Player.UseItem(index))
            {
                system("cls");
                continue;
            }
            Player.UseItem(index);
            break;
        }
        if (choice1 == 3)
        {
            if(!Player.SellItem(index))
            {
                system("cls");
                continue;
            }
            Player.SellItem(index);
            break;
        }
    }
}

void InventoryRealization(PlayerC &Player)
{
    cout << "If you want to go to inventory press 'y'" << endl;
    string choice;
    cin >> choice;

    if (tolower(choice[0]) != 'y')
        return;

    system("cls");

    Player.DisplayStatus(Player);
    Inventory(Player);
}

void SetTextColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void TextToConfirmTheSelection()
{
    cout << "If you really want to play as this character press 1" << endl
         << "If you want to change your character press 2" << endl;
}

void HeroInfoSelecting(PlayerC *temp, int &Subverification)
{
    temp->DisplayStatus(*temp);
    cout << temp->GetCDescription() << endl;
    cout << endl;
    TextToConfirmTheSelection();

    cin >> Subverification;
    if (Subverification != 1)
    {
        delete temp;
    }
}

PlayerC *SelectingGameCharacter()
{
    int Subverification = 2;
    PlayerC *temp = nullptr;

    while (Subverification == 2)
    {
        system("cls");

        cout << "Please choose your character:" << endl;
        cout << "1. Bastard" << endl;
        cout << "2. Knight" << endl;
        cout << "3. Bandit" << endl;
        cout << "4. Drow" << endl;
        cout << "Enter your choice (1-4): ";
        cout << endl;

        int choice;
        cin >> choice;

        system("cls");

        switch (choice)
        {
            case 1:
            {
                temp = new BastardC(100, 15, 5, 0, 2, CharacterType::Bastard);
                break;
            }
            case 2:
            {
                temp = new KnightC(110, 20, 10, 0, 1,CharacterType::Knight);
                break;
            }
            case 3:
            {
                temp = new BanditC(90, 10, 5, 0, 1,CharacterType::Bandit);
                break;
            }
            case 4:
            {
                temp = new ArcherC(120, 10, 5, 0, 1,CharacterType::Drow);
                break;
            }
            default:
                system("cls");
                break;
        }
        if (temp != nullptr)
        {
            cout << "You've chosen a hero " << temp->GetCName() << endl;
            HeroInfoSelecting(temp, Subverification);
        }
        system("cls");
    }
    return temp;
}

void TheEnemyMove(PlayerC &Player, EnemyC &Enemy)
{
    if (!Player.IsSkippingTurn())
    {
        cout << "" << endl;
        SetTextColor(12);
        cout << "Enemy's move" << endl;
        cout << "" << endl;

        Player.SetHP(Player.GetHP() - Enemy.GetHP());
        SetTextColor(12);
        Enemy.DisplayStatus(Enemy);
        SetTextColor(10);
        Player.DisplayStatus(Player);
    } else
    {
        SetTextColor(3);
        cout << "Enemy is stunned and cannot take damage this turn" << endl;
    }

    if (!Player.IsAlive())
    {
        SetTextColor(6);
        cout << Enemy.GetCName() << " You are defeated" << endl;
    }
}

void ThePlayerMove(PlayerC &Player, EnemyC &Enemy)
{
    cout << "" << endl;
    SetTextColor(10);
    cout << "Player's move" << endl;
    cout << "" << endl;

    Player.Abilites();

    Enemy.SetHP(Enemy.GetHP() - Player.GetDamage());
    SetTextColor(10);
    Player.DisplayStatus(Player);

    if (Enemy.IsAlive())
    {
        SetTextColor(12);
        Enemy.DisplayStatus(Enemy);
    } else
    {
        SetTextColor(6);
        cout << Enemy.GetCName() << " defeated! You've won!" << endl;
    }
}

void Battle(PlayerC &Player)
{
    system("cls");

    EnemyC Goblin("Goblin", "asdasdasd", 100, 10, 5, 1);
    EnemyC Skeleton("Skeleton", "asdasdasd", 100, 10, 5, 1);
    EnemyC Dark("Dark", "asdasdasd", 100, 10, 5, 1);
    EnemyC Shadow("Shadow", "asdasdasd", 100, 10, 5, 1);
    EnemyC King("King", "asdasdasd", 100, 10, 5, 1);


    EnemyC &Enemy = Goblin;

    cout << "A fight breaks out between " << Player.GetCName() << " end " << Enemy.GetCName() << endl;
    cout << "To make a move in a battle press 'Enter'" << endl;

    cin.get();

    //Initialization_Check(Player, Enemy);

    while (Player.IsAlive() && Enemy.IsAlive() && !Player.IsExitingBattle())
    {
        ThePlayerMove(Player, Enemy);
        cin.get();

        if (Player.GetCName() == "Bastard" && Player.IsExitingBattle())
        {
            break;
        }

        if (Player.GetCName() == "Knight")
        {
            Player.SetSkipTurn(true);
        }

        if (Enemy.IsAlive())
        {
            TheEnemyMove(Player, Enemy);
            cin.get();
        }
        system("cls");
    }
    SetTextColor(7);
}
