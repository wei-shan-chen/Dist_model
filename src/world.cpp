#include "world.h"

struct World world = {};

void create_world(VoxData_b* bounderVoxelData, int bounderNum,int* numVoxelFace) {
    world.cube = {
        //       position                  normal
        Vertex{  { 0.0f,  0.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },
        Vertex{  { 1.0f,  0.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },
        Vertex{  { 1.0f,  1.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },
        Vertex{  { 1.0f,  1.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },
        Vertex{  { 0.0f,  1.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },
        Vertex{  { 0.0f,  0.0f,  0.0f, } , { 0.0f,  0.0f, -1.0f} },

        Vertex{  { 0.0f,  0.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },
        Vertex{  { 1.0f,  0.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },
        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },
        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },
        Vertex{  { 0.0f,  1.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },
        Vertex{  { 0.0f,  0.0f,  1.0f, } , { 0.0f,  0.0f,  1.0f} },

        Vertex{  { 0.0f,  1.0f,  1.0f, } , {-1.0f,  0.0f,  0.0f} },
        Vertex{  { 0.0f,  1.0f,  0.0f, } , {-1.0f,  0.0f,  0.0f} },
        Vertex{  { 0.0f,  0.0f,  0.0f, } , {-1.0f,  0.0f,  0.0f} },
        Vertex{  { 0.0f,  0.0f,  0.0f, } , {-1.0f,  0.0f,  0.0f} },
        Vertex{  { 0.0f,  0.0f,  1.0f, } , {-1.0f,  0.0f,  0.0f} },
        Vertex{  { 0.0f,  1.0f,  1.0f, } , {-1.0f,  0.0f,  0.0f} },

        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 1.0f,  0.0f,  0.0f} },
        Vertex{  { 1.0f,  1.0f,  0.0f, } , { 1.0f,  0.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  0.0f, } , { 1.0f,  0.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  0.0f, } , { 1.0f,  0.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  1.0f, } , { 1.0f,  0.0f,  0.0f} },
        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 1.0f,  0.0f,  0.0f} },

        Vertex{  { 0.0f,  0.0f,  0.0f, } , { 0.0f, -1.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  0.0f, } , { 0.0f, -1.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  1.0f, } , { 0.0f, -1.0f,  0.0f} },
        Vertex{  { 1.0f,  0.0f,  1.0f, } , { 0.0f, -1.0f,  0.0f} },
        Vertex{  { 0.0f,  0.0f,  1.0f, } , { 0.0f, -1.0f,  0.0f} },
        Vertex{  { 0.0f,  0.0f,  0.0f, } , { 0.0f, -1.0f,  0.0f} },

        Vertex{  { 0.0f,  1.0f,  0.0f, } , { 0.0f,  1.0f,  0.0f} },
        Vertex{  { 1.0f,  1.0f,  0.0f, } , { 0.0f,  1.0f,  0.0f} },
        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 0.0f,  1.0f,  0.0f} },
        Vertex{  { 1.0f,  1.0f,  1.0f, } , { 0.0f,  1.0f,  0.0f} },
        Vertex{  { 0.0f,  1.0f,  1.0f, } , { 0.0f,  1.0f,  0.0f} },
        Vertex{  { 0.0f,  1.0f,  0.0f, } , { 0.0f,  1.0f,  0.0f} },
    };
   
    for(int i = 0; i < bounderNum; i++){
        int x = bounderVoxelData[i].bounderVoxelLocate.x;
        int y = bounderVoxelData[i].bounderVoxelLocate.y;
        int z = bounderVoxelData[i].bounderVoxelLocate.z;
        // if(x +y+z == 0) std::cout <<i << std::endl;
        if(bounderVoxelData[i].bounderVoxelFaceAir[0] == 0){
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f,  0.0f, -1.0f} });
            (*numVoxelFace)++;
            // std::cout << 1 << std::endl;
        }
        if(bounderVoxelData[i].bounderVoxelFaceAir[1] == 0){
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f,  0.0f,  1.0f} });
            (*numVoxelFace)++;
        }
        if(bounderVoxelData[i].bounderVoxelFaceAir[2] == 0){
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  1.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  0.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  1.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  1.0f + z, } , {-1.0f,  0.0f,  0.0f} });
            (*numVoxelFace)++;
        }
        if(bounderVoxelData[i].bounderVoxelFaceAir[3] == 0){
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  0.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  0.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  0.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  1.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 1.0f,  0.0f,  0.0f} });
            (*numVoxelFace)++;
        }
        if(bounderVoxelData[i].bounderVoxelFaceAir[4] == 0){
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  1.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  0.0f + y,  0.0f + z, } , { 0.0f, -1.0f,  0.0f} });
            (*numVoxelFace)++;
        }
        if(bounderVoxelData[i].bounderVoxelFaceAir[5] == 0){
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 1.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  1.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            world.voxelBall.push_back(Vertex{  { 0.0f + x,  1.0f + y,  0.0f + z, } , { 0.0f,  1.0f,  0.0f} });
            (*numVoxelFace)++;
        }
    }


    world.square = {
            Vertex{ { 0.0f, 0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
            Vertex{ { 1.0f, 0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
            Vertex{ { 1.0f, 0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},

            Vertex{ { 1.0f, 0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
            Vertex{ { 0.0f, 0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
            Vertex{ { 0.0f, 0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
    };
    world.squ_indices = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };
    world.tri= {
		Vertex{ { 0.5f, -0.5f, 0.0f}},
        Vertex{ {-0.5f, -0.5f, 0.0f}},
        Vertex{ { 0.0f,  0.5f, 0.0f}}
	};
}

void destroy_world() {
    world.voxelBall.clear();
    world.square.clear();
    world.squ_indices.clear();
    world.tri.clear();
    world.cube.clear();
}