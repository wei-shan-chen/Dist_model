#ifndef RAWMODEL_H
#define RAWMODEL_H

#include <vector>
#include <iostream>
#include <string>

#include "SurfaceVoxels.h"

using namespace std;
typedef unsigned char  BYTE;

typedef struct InfData_t {
	int resolution[3];//inf size
	char sampleType[15];
	float voxelSize[3];
	char endian[10];
	// float min[3];
	// float max[3];
    int type; // 0 unsigned char, 1 float, 2 double
}InfData_t;

typedef struct RawData_l
{
    short int layer;
    bool air = false;
    RawData_l *p_x;
    RawData_l *n_x;
    RawData_l *p_y;
    RawData_l *n_y;
    RawData_l *p_z;
    RawData_l *n_z;
}RawData_l;

typedef struct RawDataY_l
{
    short int y;
    RawData_l *p_y;
    RawData_l *n_y;
}RawDataY_l;

typedef struct RawData_t
{
    short int layer;
    bool air;
}RawData_t;


class RAWmodel_cls{
public:
    RAWmodel_cls();
    ~RAWmodel_cls();
    // var
    SurfaceVoxModel_t voxelModel;
    InfData_t infdata;

    // fun
    void LoadFile(const char* infFileName,const char* rawFileName,const char* newrawFileName);
    std::vector<glm::ivec3> Voxel_Position(int layer);

private:
    RawData_t*** rawData; // 0 air, 1 bounder, 2 inside
    RawData_l* head;
    bool LoadINFfile(const char* infFileName);
    bool SetSampleType(const char* type);

    void CreateRawData();

    bool LoadRAWfile(const char*rawFileName);
    bool ReadRawFile(FILE *file);
    void FindOutterLayer(short int x, short int y, short int z);

    void SetVoxelData();
    void findSurfaceVoxel(int z, int y, int x, int num, int layer);

    void checkComputerEndian();
    void setMaxbounder(int i, int j, int k);

    BYTE* uc_voxelData;
    float* f_voxelData;
    double* d_voxelData;

    int layernum = 0;

};
extern RAWmodel_cls rawmodel;
#endif