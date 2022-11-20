#ifndef RAWMODEL_H
#define RAWMODEL_H

#include <vector>
#include <iostream>
#include <string>

#include "BounderVoxel.h"

using namespace std;

typedef unsigned char  BYTE;

typedef struct InfData_t {
	int resolution[3];//inf size
	char sampleType[15];
	float voxelSize[3];
	char endian[10];
    int type; // 0 unsigned char, 1 float, 2 double
}InfData_t;

class RAWmodel{
public:
    RAWmodel();
    ~RAWmodel();
    void LoadFile(const char* infFileName,const char* rawFileName);

    InfData_t infdata;
    VoxData_b* bounderVoxelData;
    int*** voxelData; // -1 air, 0 bounder, 1 inside 
    int bounderNum;
    int maxVoxelData = 0;
    
private:
    bool LoadINFfile(const char* infFileName);
    void CreateVoxel();
    bool LoadRAWfile(const char*rawFileName);
    bool SetSampleType(const char* type);
    bool ReadRawFile(FILE *file);
    void SetVoxelData();
    void CreateBounderVoxelLocate();
    void SetbounderVoxelFaceAir(int i, int j, int k, int num, int donotouch);
    void checkComputerEndian();
    void computMaxVoxelData(int num);

    BYTE* uc_voxelData;
    float* f_voxelData;
    double* d_voxelData;

};

#endif