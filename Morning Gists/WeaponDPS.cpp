#include <iostream>
#include <assert.h>

//Write a function that calculates the damage per second of a given weapon.
//Requirements: Function must take a parameter of type that inherits from Weapon.
//Return value must be to the nearest second decimal place.Ex : 35.6

class Weapon {
public:
	float GetMinDam();
	float GetMaxDam();
	float GetAttackTime();
	void SetMinDam(float);
	void SetMaxDam(float);
	void SetAttackTime(float);

private:
	float maxDamage;
	float minDamage;
	float attackTime;	//In seconds
};

float Weapon::GetMaxDam() {
	return maxDamage;
}
float Weapon::GetMinDam() {
	return minDamage;
}
float Weapon::GetAttackTime() {
	return attackTime;
}

void Weapon::SetMaxDam(float md) {
	if (md >= 0)
		maxDamage = md;
	else
		maxDamage = 0;
}
void Weapon::SetMinDam(float md) {
	if (md >= 0)
		minDamage = md;
	else
		minDamage = 0;
}
void Weapon::SetAttackTime(float t) {
	if (t >= 0)
		attackTime = t;
	else
		attackTime = 0;
}

float GetDPS(Weapon w) {
	//Dps is defined as damage per second. In this class, attackTime will be how long that attack
	//will take to do the damage in seconds. Ex: it will take 2 second to hit once and that hit does 30 damage to a target.
	//In that case, DPS will be how much damage the weapon does divided by the / time it takes to complete the attack so another 
	//attack can be made
	
	//Averge DPS
	return ((w.GetMinDam() + w.GetMaxDam()) / 2) / w.GetAttackTime();
}

//Inheriting class
class Sword : public Weapon {
public:
	Sword(const float maxd, const float mind, const float t);
};

Sword::Sword(const float maxd, const float mind, const float t) {
	SetAttackTime(t);
	SetMinDam(mind);
	SetMaxDam(maxd);
}

int main() {
	Sword s(10, 5, 1);

	//Test Cases
	assert(GetDPS(s) == 7.5f);

	s.SetAttackTime(0.1f);
	assert(GetDPS(s) == 75.f);

	Sword t(50, 20, 2);
	assert(GetDPS(t) == 17.5f);

	system("pause");
	return 0;
}