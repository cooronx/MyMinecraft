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

bool Chunk::checkPosHasBlock(const QVector3D &pos)
{
    if(blocksPos[std::make_pair(pos.x(),pos.y())] == pos.z()){
        qDebug()<<"真的";
        return true;
    }
    else return false;
}

void Chunk::getHeightMap()
{
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            /*const siv::PerlinNoise::seed_type seed = 123456u;
            const siv::PerlinNoise perlin{ seed };*/
            //噪声太折磨了，留着吧
            SimplexNoise simpleNoise(0.1f/400.0f,0.5f,1.99f,0.5f);
            int h  = int((simpleNoise.fractal(7,x,z)+0.005)*100)%10;
            //int h = int(perlin.octave2D_01((x * 0.01), (z * 0.01), 4)*100+17)%10;

            std::shared_ptr<Block>bl(new Block(core,Block::DirtWithGrass,true));
            bl->setPos(QVector3D(x,h,z));
            blocksPos[std::make_pair(x,z)] = h;
            for(int h = 0;h<bl->getPos().y();++h){
                std::shared_ptr<Block>c_bl(new Block(core,Block::DirtWithGrass));
                QVector3D temp_pos = bl->getPos();
                temp_pos.setY(h);
                c_bl->setPos(temp_pos);
                blocks.push_back(c_bl);
            }
            blocks.push_back(bl);
        }
    }

}
