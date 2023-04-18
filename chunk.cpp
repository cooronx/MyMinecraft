#include "chunk.h"

Chunk::Chunk(QOpenGLFunctions* op):
    core(op)
{
    getHeightMap();
}

void Chunk::draw(const QMatrix4x4 &model, const QMatrix4x4 &view, const QMatrix4x4 &projection)
{
    //to do 顶层的方块要区分画图
    for(auto& x:blocks){
        x->draw(model,view,projection);
    }

}

void Chunk::getHeightMap()
{
    for (int x = 0; x < 20; x++) {
        for (int z = 0; z < 20; z++) {
            const siv::PerlinNoise::seed_type seed = 1632213u;
            const siv::PerlinNoise perlin{ seed };
            int h = int(perlin.octave2D_01((x * 0.005), (z * 0.005), 100)*100+20)%10;

            this->heightMap[QPair<int,int>{x,z}] = h;
            Block *bl = new Block(core,Block::DirtWithGrass,true);
            bl->setPos(QVector3D(x,h,z));
            for(int h = 0;h<bl->getPos().y();++h){
                Block *c_bl = new Block(core,Block::DirtWithGrass);
                QVector3D temp_pos = bl->getPos();
                temp_pos.setY(h);
                c_bl->setPos(temp_pos);
                blocks.push_back(c_bl);
            }
            blocks.push_back(bl);
        }
    }

}
