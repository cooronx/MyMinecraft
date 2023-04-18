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

constexpr int CHUNK_SIZE = 16*16;

class Chunk
{
private:
    QVector<Block*> blocks;
    QMap <QPair<int,int>,int> heightMap;
    QOpenGLFunctions* core;
    std::shared_ptr< std::set<QVector<int>> > blockSet;
public:
    Chunk(QOpenGLFunctions*);
    void draw(const QMatrix4x4 &model, const QMatrix4x4& view, const QMatrix4x4 &projection);
private:
    void getHeightMap();
};

#endif // CHUNK_H
