#pragma once
#include <string>

class Character;
// Abstract base class for all items. getType()/getPower() let the SaveSystem
// recreate the correct subclass from a text file.
class Item {
protected:
    std::string name;
    int value;

public:
    Item(std::string n, int v);
    virtual ~Item() = default;

    std::string getName() const;
    int getValue() const;

    virtual void use(Character& player) = 0;
    virtual std::string getDescription() const = 0;

    virtual std::string getType() const = 0;
    virtual int getPower() const = 0;
};

class WeaponItem : public Item {
private:
    int attackBonus;

public:
    WeaponItem(std::string n, int v, int bonus);

    void use(Character& player) override;
    std::string getDescription() const override;

    std::string getType() const override;
    int getPower() const override;
};

class HealingItem : public Item {
private:
    int healAmount;

public:
    HealingItem(std::string n, int v, int heal);

    void use(Character& player) override;
    std::string getDescription() const override;

    std::string getType() const override;
    int getPower() const override;
};

class ArmorItem : public Item {
private:
    int hpBonus;

public:
    ArmorItem(std::string n, int v, int hp);

    void use(Character& player) override;
    std::string getDescription() const override;

    std::string getType() const override;
    int getPower() const override;
};
