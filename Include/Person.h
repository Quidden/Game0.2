#pragma once
#include <string>
#include <iostream>
#include <memory>
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
    int damage;
    int die_bonus;

public:
    virtual ~PersonC() = default;
    bool IsAlive();

    void DisplayStatus();
    PersonC(int hp, int damage, int die_bonus);

    virtual const std::string& GetClassName() const = 0;
    virtual const std::string& GetClassDescription() const = 0;

    int GetHP() const { return hp; }
    int GetDamage() const { return damage; }
    int GetDieBonus() const { return die_bonus; }
};

class PlayerC : public PersonC
{
protected:
    int wallet;
    int max_num_of_weapons;

    InventoryC inventory;
    std::vector<Equipted_Items_Base*> equipped_items;

    bool exit_the_battle = false;
    bool skip_hod = false;

    CharacterType charter_type;

public:

    PlayerC(std::string name, std::string description, int hp, int damage, int die_bonus,
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

    const std::vector<Equipted_Items_Base*>& GetEquippedItems() const { return equipped_items; }
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
public:
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
    static const std::string className;
    static const std::string classDescription;

public:
    using PlayerC::PlayerC;

    void Abilites() override
    {
        std::cout << GetClassName() << " blocks and skips the turn!" << std::endl;
        SetSkipTurn(true);
    }

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return item_equip_seccess == Equipted_Item_Type::Sword || item_equip_seccess == Equipted_Item_Type::Armor;
    }

    const std::string& GetClassName() const override { return className; }
    const std::string& GetClassDescription() const override { return classDescription; }
};

class BanditC : public PlayerC
{
public:
    using PlayerC::PlayerC;

    void Abilites() override;

    bool CanEquip(Equipted_Item_Type item_equip_seccess) const override
    {
        return item_equip_seccess == Equipted_Item_Type::Dagger;
    }
};

class ArcherC : public PlayerC
{
public:
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
public:
    int reward;

    EnemyC(std::string name, std::string description, int hp, int damage, int die_bonus, int reward);
};
