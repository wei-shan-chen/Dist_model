#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SOM.h"
#include "RAWmodel.h"

struct World {

    std::vector<Vertex> square;
    std::vector<unsigned int> squ_indices;
    std::vector<Vertex> tri;
    std::vector<Vertex> cube;
    std::vector<Vertex> voxel;
};

void create_world(SurfaceVoxModel_t  voxelModel);
void destroy_world();
// extern int voxelBallPointNum;
extern struct World world;

#endif