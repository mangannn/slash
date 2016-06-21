#ifndef _ROTANI_H_
#define _ROTANI_H_

#include "../Object.hpp"

using namespace sf;

class RotAni: public Object {

private:
    
    

public:
    
    double rot = 0;
    
    Color c_mantel = Color(220, 30, 40);
    const static int CAPECOUNT = 6;
    Vertex vertices[CAPECOUNT] = {
        Vertex(Vector2f(-2, 0), c_mantel),
        Vertex(Vector2f( 2, 0), c_mantel),
        Vertex(Vector2f( 2, 4), c_mantel),
        Vertex(Vector2f(-2, 4), c_mantel),
        Vertex(Vector2f(-2, 8), c_mantel),
        Vertex(Vector2f( 2, 8), c_mantel)
    };
    
	RotAni(Vector2f position):
        Object(position)
    {
        
    }

	~RotAni() {}


	virtual void update(float elapsedTime) {
        rot += elapsedTime;
    }

	virtual void draw(RenderWindow *window) {
        window->draw(vertices, CAPECOUNT, Quads);
    }
};

#endif
