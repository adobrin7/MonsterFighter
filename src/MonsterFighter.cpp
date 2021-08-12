#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Creature
{
protected:
	std::string m_name;
	char m_symbol;
	int m_health;
	int m_damage;
	int m_gold;

public:
	Creature(std::string& name, char symbol, int health, int damage, int gold)
		: m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
	{
	}

	const std::string& getName() { return m_name; }
	char getSymbol() { return m_symbol; }
	int getHealth() { return m_health; }
	int getDamage() { return m_damage; }
	int getGold() { return m_gold; }

	void reduceHealth(int health) { m_health -= health; }
	bool isDead() { return m_health <= 0; }
	void addGold(int gold) { m_gold += gold; }
};

class Player : public Creature
{
private:
	int m_level = 1;

public:
	Player(std::string& name)
		: Creature(name, '@', 10, 1, 0)
	{
	}

	int getLevel() { return m_level; }

	void levelUp() { ++m_level; ++m_damage; }
	bool hasWon() { return m_level >= 20; }
};

class Monster : public Creature
{
public:
	enum Type
	{
		TYPE_DRAGON,
		TYPE_ORC,
		TYPE_SLIME,
		MAX_TYPES
	};

	struct MonsterData
	{
		std::string name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	static MonsterData monsterData[MAX_TYPES];

	Monster(Type type)
		: Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health,
			monsterData[type].damage, monsterData[type].gold)
	{
	}

	static Monster getRandomMonster()
	{
		return Monster(static_cast<Type>(rand() % (MAX_TYPES)));
	}
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};




void attackMonster(Player& player, Monster& monster)
{
	monster.reduceHealth(player.getDamage());
	std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";

	if (monster.isDead())
	{
		std::cout << "You killed the " << monster.getName() << ".\n";
		player.levelUp();
		std::cout << "You are now level " << player.getLevel() << ".\n";
		std::cout << "You found " << monster.getGold() << " gold.\n";
		player.addGold(monster.getGold());
	}
	else
		std::cout << "Monster has " << monster.getHealth() << " hp.\n";
}

void attackPlayer(Player& player, Monster& monster)
{
	if (monster.isDead())
		return;

	player.reduceHealth(monster.getDamage());
	std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
	if(!player.isDead())
		std::cout << "You have " << player.getHealth() << " hp.\n";
}

void fightMonster(Player& player)
{
	Monster monster(static_cast<Monster::Type>(rand() % Monster::MAX_TYPES));
	std::cout << "You have encountered a " << monster.getName() << " " << monster.getSymbol() << ".\n";

	while (!player.isDead() && !monster.isDead())
	{
		std::cout << "(r)un or (f)ight: ";
		char input;
		std::cin >> input;
		if (input == 'r')
		{			
			if (rand() % 2 == 1)
			{
				std::cout << "You successfully fled.\n";
				return;
			}
			else
			{
				std::cout << "You failed to flee.\n";
				attackPlayer(player, monster);
				continue;
			}
		}
		if (input == 'f')
		{
			attackMonster(player, monster);
			attackPlayer(player, monster);
		}
	}
}

bool askToContinue()
{
	// asks player to continue, checks input

	while (true)
	{
		std::cout << "Play again? (y/n): ";
		char choise;
		std::cin >> choise;
		std::cin.ignore(32767, '\n');
		if (choise == 'n')
			return false;
		else if (choise == 'y')
			return true;
	}
}




int main()
{
	srand(time(NULL));	

	do
	{
		std::cout << "Enter your name: ";
		std::string playerName;
		std::cin >> playerName;
		Player player(playerName);
		std::cout << "Welcome, " << playerName << "\n";
		
		while (!player.isDead() && !player.hasWon())
			fightMonster(player);

		if (player.isDead())		
			std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";		
		else
			std::cout << "You won the game with " << player.getGold() << " gold!\n";		

	} while (askToContinue());

	return 0;
}
