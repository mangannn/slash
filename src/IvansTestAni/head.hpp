#ifndef _ROTANI_H_
#define _ROTANI_H_

#include "../Object.hpp"

using namespace sf;

class RotAni: public Object {

private:
    
    Color *tintColor(Color *c) {
        c->r -= 30;
        return c;
    }

public:
    
    double rot = 0;
    
    Color c_mantel = Color(220, 30, 40);
    const static int CAPECOUNT = 6;
    Vertex vertices[CAPECOUNT] = {
        Vertex(Vector2f(-10, 0), c_mantel),
        Vertex(Vector2f( 10, 0), c_mantel),
        Vertex(Vector2f(-10, 20), c_mantel),
        Vertex(Vector2f( 10, 20), *tintColor(&c_mantel)),
        Vertex(Vector2f(10, 20), *tintColor(&c_mantel)),
        Vertex(Vector2f( 10, 20), *tintColor(&c_mantel))
    };
    
	RotAni(Vector2f position):
        Object(position)
    {
        
    }

	~RotAni() {}


	virtual void update(float elapsedTime) {
        rot += elapsedTime;
    }

	virtual void draw(RenderTarget *window, RenderTarget *monitor) {
        window->draw(vertices, CAPECOUNT, TrianglesStrip);
    }
};

#endif
