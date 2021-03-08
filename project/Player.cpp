#include <iostream>
#include "Player.hpp"

void Player::setWeapon(Item* sword) {
	swords.push_back(sword);
	//std::cout << "setWeapon" << std::endl;
}
void Player::setArmor(Item* armor) {
	armors.push_back(armor);
	//std::cout << "setArmor" << std::endl;
}

void Player::setItem(Item* item) {
	items.push_back(item);
}

std::vector<Item*>& Player::getWeapons() {
	return swords;
}

std::vector<Item*>& Player::getArmors() {
	return armors;
}

std::vector<Item*>& Player::getItems() {
	return items;
}

void Player::removeWeapon(Item* item) {
	//swords.pop_back();
	swords.erase(std::remove(swords.begin(), swords.end(), item), swords.end());
}

void Player::removeArmor(Item* item) {
	//armors.pop_back();
	armors.erase(std::remove(armors.begin(), armors.end(), item), armors.end());
}

void Player::removeItem(Item* item) {
	//int i = items.size() - 1 - num;
	//items.erase(items.begin() + i);
	items.erase(std::remove(items.begin(), items.end(), item), items.end());
}

void Player::removeScroll() {
	scroll = NULL;
}

void Player::wearWeapon(Item* _weapon) {
	sword = _weapon;
}

void Player::wearArmor(Item* _armor) {
	armor = _armor;
}

void Player::offWeapon(Item* _sword) {
	if (sword != NULL) {
		std::string sword1 = sword->getName();
		std::string sword2 = _sword->getName();
		if (sword1.compare(sword2) == 0) {
			sword = NULL;
		}
	}
}

void Player::offArmor(Item* _armor) {
	if (armor != NULL) {
		std::string armor1 = armor->getName();
		std::string armor2 = _armor->getName();
		if (armor1.compare(armor2) == 0) {
			armor = NULL;
		}
	}
}

Item* Player::getWeapon() {
	return sword;
}

Item* Player::getArmor() {
	return armor;
}

void Player::setScroll(Item* _scroll) {
	scroll = _scroll;
}

Item* Player::getScroll() {
	return scroll;
}