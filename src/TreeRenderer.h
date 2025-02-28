#ifndef DESIREPATHS_TREERENDERER_H
#define DESIREPATHS_TREERENDERER_H

#include <raylib.h>

class TreeRenderer {
public:
	TreeRenderer();
	void Draw() const;
private:
	RenderTexture2D m_renderTexture;
};

#endif //DESIREPATHS_TREERENDERER_H
