#include "LoadingLayer.hpp"

extern "C" {
    void loadLevel(char* string);
};

class LevelLayer;
class Level;

class LevelLoadingLayer : public LoadingLayer {
public:
    Level* m_level;

    bool m_done = false;
    LevelLayer* m_levelLayer;

    LevelLoadingLayer(Level* level);

    void update(float delta);
};