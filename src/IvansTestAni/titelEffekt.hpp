#ifndef _TITELEFFEKT_H_
#define _TITELEFFEKT_H_

#include "../Object.hpp"
#include "../Functions.hpp"
#include "../Resources.hpp"
#include <math.h>

using namespace sf;

class TitelEffekt: public Object {

public:

    float anim = 0;

    Text grafText;
    RenderTexture mytexture;
    Vector2i textureSize;
    Sprite sprite;
    
    

	TitelEffekt(Vector2f position):
		Object(position)
	{
        textureSize = Vector2i(70, 70);
        mytexture.create(textureSize.x, textureSize.y);
        grafText.setFont(*Resources::getMainFont());
        grafText.setCharacterSize(16);
        grafText.setString(String("teststring.\nTESTSTRING!\n:0123456789;"));
        grafText.setPosition(0, 0);
        mytexture.draw(grafText);
        mytexture.display();
        sprite = Sprite(mytexture.getTexture());
	}

	virtual ~TitelEffekt() {}

	virtual void update(float elapsedTime) {
        anim += elapsedTime / 3;
        if (anim > 2 * M_PI) anim -= 2 * M_PI;
	}

	virtual void draw(RenderTarget *window, RenderTarget *monitor) {
        
        // mytexture.clear();
        
        
        sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
        sprite.setPosition(pos.x, pos.y);
        
        sprite.setScale(1,1);
        sprite.setRotation(0);
        monitor->draw(sprite);
        sprite.move(100, 200);
        sprite.setScale(6,6);
        //sprite.setRotation(sin(anim) * 15);
        monitor->draw(sprite);
        
        sprite.setPosition(pos.x - 40, pos.y - 185);
        sprite.setScale(1 / 3.0, 1 / 3.0);
        sprite.setRotation(0);
        window->draw(sprite);
        sprite.move(30, 90);
        sprite.setScale(2,2);
        //sprite.setRotation(sin(anim) * 15);
        window->draw(sprite);
        
	}
};

#endif
