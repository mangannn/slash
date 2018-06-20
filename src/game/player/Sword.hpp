#ifndef _Sword_
#define _Sword_

#include <math.h>

#include "Weapon.hpp"

#include "WeaponMove.hpp"

#include "../Collision.hpp"
#include "../Functions.hpp"

#include "../Animation.hpp"

#include "SwordGraphics.hpp"


#include "../obj/graphicaleffects/Flash.hpp"

using namespace sf;

class Sword: public Weapon {

public:

	Animation ani;

	CollisionBox swordBox;

	Vector2f swordTip, swordOri;

	SwordGraphics swordGraphics;

	Vector2f swordDirectionVec;

	const float swordLength = 32;


	WeaponMove currentSwordMovement = WeaponMove(0,0,0,0,0,100);


	enum class State {
		Idle,
		DrawBack,
		Pause,
		Hurt,
		Slash,
		SlashBack,
		SlashBig,
		Stab,
		StabBig,
		Swirl
	};

	/*
	Combos
	A+A+A - Slash + SlashBack + SlashBig
	A + B - Slash + Stab
	B + A - Stab + Swirl
	*/

	State state = State::Idle;
	State comboState = State::Idle;

	Sword(Vector2f *offset):
		Weapon(offset),
		ani(offset, Vector2f(0, 0), "media/animation/Sword1/a", 16),
		swordBox(offset, Vector2f(0, 0), 4),
		swordTip(0, swordLength)
	{

	}

	virtual ~Sword() {}
	
	virtual void update(float elapsedTime, float characterDirection) {

		if (state != State::Idle) {
			if (currentSwordMovement.update(elapsedTime)) {
				if (state == State::DrawBack) {
					setState(State::Idle);
				} else if (state == State::Pause || state == State::Hurt) {
					setState(State::DrawBack);
				} else {
					setState(State::Pause);
				}
			}
		}

		float swordDirection = currentSwordMovement.getDirection() + characterDirection;
		swordDirectionVec = Vector2f(cos(swordDirection), sin(swordDirection));
		float swordDist = currentSwordMovement.getLength();
		swordTip = swordDist * swordDirectionVec;
		swordOri = (swordDist - swordLength) * swordDirectionVec;


		int discDir = discreteDirectionFromAngle(swordDirection, 16);
		ani.setFrame(discDir);
		ani.pos = swordOri;

		swordBox.pos = swordTip;

		swordGraphics.update(elapsedTime, swordTip);

		//tellState();
	}

	virtual void draw(RenderTarget *target) {

		if (state != State::Idle && state != State::DrawBack && state != State::Hurt) {
			
			Vector2f center = *offset;

			swordGraphics.draw(target, center, swordTip);
			ani.draw(target);


			/*sf::Vertex line[] = {
			    sf::Vertex(center + swordOri),
			    sf::Vertex(center + swordTip)};
			target->draw(line, 2, sf::Lines);*/

			//swordBox.draw(target);
		}
	}

	virtual bool isBehind() {
		return (swordTip.x + swordTip.y <= 0);
	}

	void setState(State s) {

		if (s == State::Pause) {
			currentSwordMovement = WeaponMove(currentSwordMovement.reachEnd, currentSwordMovement.reachEnd, 
											currentSwordMovement.dirEnd, currentSwordMovement.dirEnd, 0, 0.15);
		} else if (s == State::DrawBack) {
			// combobreak
			comboState = State::Idle;

			currentSwordMovement = WeaponMove(0, 0, 0, 0, 0, 0.1);
		} else if (s == State::Hurt) {
			// combobreak
			comboState = State::Idle;

			currentSwordMovement = WeaponMove(0, 0, 0, 0, -300, 0.15);
		}  else {

			// start combo
			comboState = s;

			if (s == State::Slash) {
				currentSwordMovement = WeaponMove(swordLength*1.2f, swordLength*1.2f, -M_PI/2.f, M_PI/2.f, 300, 0.15);
			} else if (s == State::SlashBack) {
				currentSwordMovement = WeaponMove(swordLength*1.2f, swordLength*1.2f, M_PI/2.f, -M_PI/2.f, 350, 0.15);
			} else if (s == State::SlashBig) {
				currentSwordMovement = WeaponMove(swordLength*1.4f, swordLength*1.4f, -M_PI/2.f, M_PI/2.f, 400, 0.25);
			} else if (s == State::Stab) {
				currentSwordMovement = WeaponMove(swordLength/4.f, swordLength*1.4f, 0, 0, 400, 0.15);
			} else if (s == State::StabBig) {
				currentSwordMovement = WeaponMove(swordLength/4.f, swordLength*2.f, 0, 0, 600, 0.2);
			} else if (s == State::Swirl) {
				currentSwordMovement = WeaponMove(swordLength*1.8f, swordLength*1.2f, 0, 7.f*M_PI/4.f, 100, 0.35);
			}
		}

		state = s;
	}

	virtual void setAction(Action a) {
		if (a == Action::GetHit) {
			setState(State::Hurt);
		} else if (state == State::Idle) {
			if (a == Action::Slash) {
				setState(State::Slash);
			} else if (a == Action::Stab) {
				setState(State::Stab);
			}
		} else if (state == State::Pause) {
			// combo!
			if (a == Action::Slash) {
				if (comboState == State::Slash) {
					setState(State::SlashBack);
				} else if (comboState == State::SlashBack) {
					setState(State::SlashBig);
				} else if (comboState == State::Stab) {
					setState(State::Swirl);
				}
			} else if (a == Action::Stab) {
				if (comboState == State::Slash) {
					setState(State::StabBig);
				}
			}
		} else {
			// combobreak
			comboState = State::Idle;
		}
	}

	virtual bool isHitCircle(CollisionBox c) {
		if (state == State::Idle || state == State::DrawBack) {
			return false;
		} else if (state == State::Stab || state == State::StabBig) {
			// check collision with box
			return CollisionBox::check(swordBox, c);
		} else {

			Vector2f line_dir = swordDirectionVec;
			Vector2f p1 = *offset + swordOri;
			Vector2f p2 = *offset + swordTip;
			float line_length = swordLength;


			float along_line = dot((c.getPosition() - p1), line_dir);

			if (along_line > 0) {
				if (along_line > line_length) {
					if (along_line < line_length + c.r) {
						// maybe collide with p2
						Vector2f collision_vec = c.getPosition() - p2;
						return (size(collision_vec) < c.r);
					}
				} else {
					// maybe collide with line
					Vector2f collision_vec = c.getPosition() - ((along_line * line_dir) + p1);
					return (size(collision_vec) < c.r);
				}
			}
			
			return false;
		}
	}

	virtual bool isBusy() {
		return (state != State::Idle);
	}

	virtual float getCharacterVelocity() {
		return currentSwordMovement.getVelocity();
	}

	virtual bool canParay() {
		return (state == State::Slash || state == State::SlashBack || state == State::SlashBig || state == State::Swirl);
	}
	virtual CollisionBox getParayBox() {
		return swordBox;
	}
	virtual void paray() {
		setState(State::DrawBack);
	}

	virtual bool canHurt() {
		return !((state == State::Idle || state == State::DrawBack || state == State::Hurt || state == State::Pause));
	}
	virtual bool canGetHurt() {
		return !(state == State::Hurt);
	}
};

#endif
