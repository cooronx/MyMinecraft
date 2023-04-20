#ifndef CHUNK_H
#define CHUNK_H

#include <array>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QDebug>
#include <QMap>
#include <QOpenGLFunctions>
#include <memory>
#include <set>
#include <QPair>
#include "PerlinNoise.hpp"
#include "block.h"
#include "SimplexNoise.h"

constexpr int CHUNK_SIZE = 16*16;

class Chunk
{
private:
    QOpenGLFunctions* core;
    QVector< std::shared_ptr<Block> > blocks;
    QMap < QPair<int,int>,int > blocksPos;
public:
    Chunk(QOpenGLFunctions*);
    void draw(const QMatrix4x4 &model, const QMatrix4x4& view, const QMatrix4x4 &projection);
    bool checkPosHasBlock(const QVector3D& pos);
private:
    void getHeightMap();
};

#endif // CHUNK_H
