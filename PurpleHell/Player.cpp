#include "Player.h"



Player::Player()
{
	this->initHeroesMenu();
	this->initEquipedItems();
}

Player::Player(bool battle)
{
	if (battle) {
		this->initHeroesBattle();
		this->initEquipedItems();
	}
	else {
		this->initHeroesMenu();
		this->initEquipedItems();
	}
}

Player::~Player()
{
	delete this->equipedItems;
	delete this->units;

	for (int i = 0; i < this->maxUnits; i++) {
		delete this->team[i];
	}

}
void Player::initHeroesMenu()
{
	std::ifstream ifsHeroes("res/Player/Team.txt");
	this->ArquivoHeroesMenu(ifsHeroes, 0);
}

void Player::initHeroesBattle()
{
	std::ifstream ifsHeroes("res/Player/Team.txt");
	this->ArquivoHeroesBattle(ifsHeroes, 0);
}

void Player::initEquipedItems()
{
	this->equipedItems = new EquipedItems();
}

void Player::render(sf::RenderTarget * target)
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]) {
			this->team[i]->render(target);
			if(this->team[i]->GetSpell()){
				this->team[i]->GetSpell()->render(target);
			}
		}
	}
}

void Player::update(sf::Vector2f mousePos,const float &dt)
{

	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (this->team[i]->getSprite()->getGlobalBounds().contains(mousePos)) {
					this->team[i]->setSelected(true);
				}
				else {
					this->team[i]->setSelected(false);
				}
			}
			if (this->team[i]->GetSpell()) {
				this->team[i]->GetSpell()->updateAnimation(dt);
			}
			
			this->team[i]->UpdateAnimation(dt);
			this->team[i]->update(mousePos,dt);

		}
	}
}

void Player::MenuPosition()
{
	for (int i = 0; i < this->maxUnits; i++) {
		this->team[i]->SetPosition(200 + (23 * i), 148);
	}
}

void Player::AddHeroToTeam(Hero* hero)
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]->getName() == "slot") {
			this->team[i] = hero;
			break;
		}
	}
}


void Player::battlePosition()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i])
			this->team[i]->SetPosition(250 + (23 * i), 150);
	}
}


Hero* Player::getHero()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]){
			if (team[i]->getSelected() ) {
				return this->team[i];
			}
		}
	}
	return nullptr;
}

Entity* Player::getTeam(int i)
{
	return this->team[i];
}

Entity * Player::getRandomHero()
{
	int heroIndex = rand() % 3;
	

	if (this->team[heroIndex] != nullptr) {
		if (this->team[heroIndex]->getHp() > 0) {
			return this->team[heroIndex];
		}
	}


	return getRandomHero();
}

int Player::getGold()
{
	return this->gold;
}

void Player::updateGold(int value)
{
	this->gold += value;
}


void Player::setTeam(int i, Hero * hero)
{
	this->team[i] = hero;
}

EquipedItems * Player::getEquipedItems()
{
	return this->equipedItems;
}

bool Player::getClear()
{
	return this->clear;
}
bool Player::canEquipHero()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]) {
			if (this->team[i]->getName() == "slot") return true;
		}
	}
	return false;
}
bool Player::CanBuy(Item* item, Hero* hero)
{
	int tempGold = 0;
	int totalGold = 0;
	if (item) {
		tempGold += 10;
	}
	if (hero) {
		tempGold += 20;
	}
	totalGold = this->gold - tempGold;

	if (totalGold >= 0) {
		this->gold = totalGold;
		return true;
	}
	else {
		return false;
	}


}
void Player::setClear(bool value)
{
	this->clear = value;
}

int Player::teamSize()
{
	int a = 0;
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]) {
			if (this->team[i]->getName() != "slot") {
				a++;
			}
		}
	}
	return a;
}

bool Player::checkPlayed()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if(team[i]){
			if (!team[i]->getPlayed()) {
				return true;
			}
		}
	}
	return false;
}

void Player::setTeamToTrue()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if(this->team[i] && this->team[i]->getName() != "slot")
			this->team[i]->setPlayed(false);
	}
}

void Player::setSpecialToTrue()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i] && this->team[i]->getName() != "slot")
			this->team[i]->setEspecial(true);
	}
}

void Player::RemoveHero(Hero* hero)
{
	sf::Texture emptyTex;
	emptyTex.loadFromFile("res/img/items/slot.png");
	Hero* empty = new Hero(0, 0,"slot", "slot", 0, 0, 0, &emptyTex);
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i] == hero) {
			this->team[i] = empty;
			break;
		}
	}
}

void Player::Save()
{				  

	std::fstream ofsTeam;
	ofsTeam.open("res/Player/Team.txt", std::ofstream::out | std::ofstream::trunc);

	for (int i = 0; i < this->maxUnits; i++) {
		ofsTeam
			<< this->team[i]->getName()
			<< " " << this->team[i]->GetJob()
			<< " " << this->team[i]->getHp()
			<< " " << this->team[i]->getPower()
			<< " " << this->team[i]->getType() << std::endl;
	}
	ofsTeam.close();
	
}

bool Player::checkDeads()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i])
			if (this->team[i]->getHp() > 0) {
				return false;
				break;
			}
	}
	return true;
}

void Player::checkDead()
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i])
			if (this->team[i]->getHp() <= 0 && this->team[i]->getPlayed() == false) {
					this->team[i]->setPlayed(true);
			}
	}
}

int Player::UnitNumber(Entity *hero)
{
	for (int i = 0; i < this->maxUnits; i++) {
		if (this->team[i]->getName() == hero->getName()) {
				return i;
		}
	}
}
//Arquivos
void Player::ArquivoHeroesMenu(std::ifstream &ifsHeroes,int i)
{

	std::string name = " ", job = " ";
	int hp = 0, power = 0, special = 0;

	if (ifsHeroes.is_open())
	{
		if (!ifsHeroes.eof())
		{

			ifsHeroes >> name >> job >> hp >> power >> special;
				if(name != " "){
					sf::Texture *tex;
					tex = new sf::Texture();
					tex->loadFromFile("res/img/Player/" + job + ".png");
					this->team[i] = (new Hero(200 + (23 * i), 148, name, job, hp, power, special, tex));
					i++;
				}
			ArquivoHeroesMenu(ifsHeroes,i);
		}
		else {
			ifsHeroes.close();
		}
	}

}

void Player::ArquivoHeroesBattle(std::ifstream &ifsHeroes, int i)
{
	std::string name = " ", job = " ";
	int hp = 0, power = 0, spell = 0;

	if (ifsHeroes.is_open())
	{

		if (!ifsHeroes.eof())
		{
			ifsHeroes >> name >> job >> hp >> power >> spell;
			if (name != "slot") {
				sf::Texture *tex;
				tex = new sf::Texture();
				tex->loadFromFile("res/img/Player/" + job + ".png");
				this->team[i] = (new Hero(0, 0, name, job, hp, power, spell, tex));
				i++;
			}
			ArquivoHeroesBattle(ifsHeroes,i);
		}
		else {
			ifsHeroes.close();
		}
	}

}