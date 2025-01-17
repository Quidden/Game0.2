#pragma once
#include <string>
#include <iostream>
#include "Item.h"
#include "Inventory.h"
#include <vector>

enum class CharacterType { Knight, Drow, Bandit, Bastard };
struct EquipResult
{
    bool success;
    Equipted_Items_Base* Item;
};

class PersonC
{
protected:
    int hp;
    int max_hp;
    int damage;
    int die_bonus;
    std::string className;
    std::string classDescription;
    std::vector<Equipted_Items_Base*> equipped_items;

public:
    virtual ~PersonC() = default;
    bool IsAlive();

    void DisplayStatus(PersonC& player);
    PersonC(int hp, int damage, int die_bonus);

    int GetHP() const { return hp; }
    void SetHP(int new_hp)
    {
        if (new_hp > max_hp)
            hp = max_hp;

        else if (new_hp <= 0)
        {
            hp = 0;
            std::cout << "The character has died!" << std::endl;
            exit(0);
        }
        else
            hp = new_hp;
    }
    int GetDamage() const { return damage; }
    int GetDieBonus() const { return die_bonus; }

    virtual const std::string& GetCName() const = 0;
    virtual const std::string& GetCDescription() const = 0;
    const std::vector<Equipted_Items_Base*>& GetEquippedItems() const { return equipped_items; }
};

class PlayerC : public PersonC
{
protected:
    int wallet;
    int max_num_of_weapons;

    InventoryC inventory;
    //std::vector<Equipted_Items_Base*> equipped_items;

    bool exit_the_battle = false;
    bool skip_hod = false;

    CharacterType charter_type;

public:
    PlayerC(int hp, int damage, int die_bonus,
            int wallet, int max_num_of_weapons, CharacterType charter_type);

    int GetWallet() const { return wallet; }
    void AddToWallet(int amount)
    {
        if (amount < 0 && wallet + amount < 0)
            wallet = 0;
        else
            wallet += amount;
    }

    int GetMaxNumOfWeapons() const { return max_num_of_weapons; }

    bool IsExitingBattle() const { return exit_the_battle; }
    void SetExitBattle(bool value) { exit_the_battle = value; }

    bool IsSkippingTurn() const { return skip_hod; }
    void SetSkipTurn(bool value) { skip_hod = value; }

    const InventoryC& GetInventory() const { return inventory; }
    void AddToInventory(ItemC* item) { inventory.AddItem(item); }
    void DisplayPlayerInventory() {
        inventory.DisplayInventory();
    }

    EquipResult EquipError(int index);
    void EquipAction(Equipted_Items_Base* Equipted_Items, int index);

    bool UseItem(int index);
    bool SellItem(int index);

    void TryAbility(int chanceThreshold, bool& effectFlag, const std::string& abilityMessage);
    int AdjustDamage(bool add);

    virtual void Abilites() = 0;
    virtual bool CanEquip(Equipted_Item_Type item_equip_success) const = 0;
};

class BastardC : public PlayerC
{
private:
    const std::string className = "Bastard"; // make a name/description getter's
    const std::string classDescription = "DBastard";

public:


    const std::string& GetCName() const override { return className; }
    const std::string& GetCDescription() const override { return classDescription; }

    using PlayerC::PlayerC;

    std::vector<Equipted_Items_Base*> equpted_item;

    void Abilites() override;

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return true;
    }
};

class KnightC : public PlayerC
{
private:
    const std::string className = "Knight";
    const std::string classDescription = "DKnight";

public:

    const std::string& GetCName() const override { return className; }
    const std::string& GetCDescription() const override { return classDescription; }

    using PlayerC::PlayerC;

    void Abilites() override;

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return item_equip_seccess == Equipted_Item_Type::Sword || item_equip_seccess == Equipted_Item_Type::Armor;
    }
};

class BanditC : public PlayerC
{
private:
    const std::string className = "Bandit";
    const std::string classDescription = "DBandit";

public:

    const std::string& GetCName() const override { return className; }
    const std::string& GetCDescription() const override { return classDescription; }

    using PlayerC::PlayerC;

    void Abilites() override;

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return item_equip_seccess == Equipted_Item_Type::Dagger;
    }
};

class ArcherC : public PlayerC
{
private:
    const std::string className = "Archer";
    const std::string classDescription = "DArcher";

public:

    const std::string& GetCName() const override { return className; }
    const std::string& GetCDescription() const override { return classDescription; }

    using PlayerC::PlayerC;

    PersonC *target;

    void Abilites() override;

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return item_equip_seccess == Equipted_Item_Type::Bow || item_equip_seccess == Equipted_Item_Type::Armor;
    }
};

class EnemyC : public PersonC
{
private:
    std::string name;
    std::string description;
public:
    int reward;
    EnemyC(std::string name, std::string description, int hp, int damage, int die_bonus, int reward);

    const std::string& GetCName() const override { return className; }
    const std::string& GetCDescription() const override { return classDescription; }
};
