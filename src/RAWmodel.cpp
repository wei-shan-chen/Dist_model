#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
// #include <GLFW/glfw3.h>
#include <iomanip>
#include "RAWmodel.h"
RAWmodel_cls rawmodel;

RAWmodel_cls::RAWmodel_cls(){

}

RAWmodel_cls::~RAWmodel_cls(){
    // free(bounderVoxelData);
    for(int i = 0; i < infdata.resolution[2]; i++){
        for(int j = 0; j < infdata.resolution[1]; j++){
            free(rawData[i][j]);
        }
    }
    for(int i = 0; i < infdata.resolution[2]; i++){
        free(rawData[i]);
    }
    free(rawData);
}
void RAWmodel_cls::LoadFile(const char* infFileName,const char* rawFileName, const char* newrawFileName){
    LoadINFfile(infFileName);
    CreateRawData();
    LoadRAWfile(rawFileName, newrawFileName);

}
std::vector<glm::ivec3> RAWmodel_cls::Voxel_Position(int layer){

    std::vector<glm::ivec3> voxelPosition;

    for(int i = 0; i < voxelModel.num; i++){
        voxelPosition.push_back(voxelModel.voxel[i].locate);
    }

    return voxelPosition;
}

bool RAWmodel_cls::LoadINFfile(const char* infFileName){
    FILE *file = NULL;
    errno_t err;                    //record error
    file = fopen(infFileName, "r"); // err = 0 success, err != fail
    if(file == NULL){
        std::cout << "Failed to open inffile" << std::endl;
        return false;
    }

    char buffer[133];
    int lineno = 0; // record line number // total 12 line
    char c[10];
    // start reading
    fgets(buffer, sizeof(buffer), file); // raw-file=XXX.raw
    lineno++;
    fgets(buffer, sizeof(buffer), file);// resolution=XX*XX*XX
    sscanf(buffer, "resolution=%dx%dx%d", &infdata.resolution[0], &infdata.resolution[1], &infdata.resolution[2]);
    std::cout <<  "resolution : "<<infdata.resolution[0] << ", " << infdata.resolution[1] << ", " << infdata.resolution[2] << std::endl;

    fgets(buffer, sizeof(buffer), file);// sample-type=XXXXXX
    sscanf(buffer, "sample-type=%s", c);
    if(SetSampleType(c) == false){
        std::cout << "Failed to set sample type" << std::endl;
        return false;;
    }
    std::cout << infdata.sampleType << std::endl;

    fgets(buffer, sizeof(buffer), file);//voxel-size=XXX:XXX:XXX
    sscanf(buffer, "voxel-size=%f:%f:%f", &infdata.voxelSize[0], &infdata.voxelSize[1], &infdata.voxelSize[2]);
    // std::cout << "voxel-size : "<<infdata.voxelSize[0] << ", " << infdata.voxelSize[1] << ", " << infdata.voxelSize[2] << std::endl;

    fgets(buffer, sizeof(buffer), file);//endian=XXX
    sscanf(buffer, "endian=%s",infdata.endian);
    // std::cout << infdata.endian << std::endl;

    if (feof(file))
    {
        std::cout << "End of file reached!" << std::endl;
    }
    else if (ferror(file))
    {
        std::cout << "Encountered an error while reading the file!" << std::endl;
    }

    fclose(file);

    return true;

}

void RAWmodel_cls::CreateRawData(){
    int rawSize = infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2];
    if(infdata.type == 0){
        uc_voxelData = (BYTE*)malloc(sizeof(BYTE) * rawSize);
        new_uc_voxelData = (BYTE*)malloc(sizeof(BYTE) * rawSize);
    }else if(infdata.type == 1){
        f_voxelData = (float*)malloc(sizeof(float)* rawSize);
        new_f_voxelData = (float*)malloc(sizeof(float)* rawSize);
    }else if(infdata.type == 2){
        d_voxelData = (double*)malloc(sizeof(double)* rawSize);
        new_d_voxelData = (double*)malloc(sizeof(double)* rawSize);
    }
    rawData = (RawData_t***)malloc(sizeof(RawData_t**) * infdata.resolution[2]);
    for(int i = 0; i < infdata.resolution[2]; i++){
        rawData[i] = (RawData_t**)malloc(sizeof(RawData_t*) * infdata.resolution[1]);
        for(int j = 0; j < infdata.resolution[1]; j++){
            rawData[i][j] = (RawData_t*)malloc(sizeof(RawData_t) * infdata.resolution[0]);
            for(int k = 0; k < infdata.resolution[0]; k++){
                rawData[i][j][k].layer = -1;
                rawData[i][j][k].air = false;
            }
        }
    }
}

bool RAWmodel_cls::LoadRAWfile(const char* rawFileName, const char* newrawFileName){
    FILE *file = NULL;
    errno_t err;                    //record error
    file = fopen(rawFileName, "r"); // err = 0 success, err != fail
    if(file == NULL){
        std::cout << "Failed to open rawfile" << std::endl;
        return false;
    }

    //read raw to 3Darray
    if(!ReadRawFile(file)){
        std::cout << "Failed to read raw file" << std::endl;
    }
    //set 0 air, 1 bounder, 2 inside
    SetVoxelData();

    // error detect
    if (feof(file))
        std::cout << "End of file reached!" << std::endl;
    else if (ferror(file))
        std::cout << "Encountered an error while reading the file!" << std::endl;

    fclose(file);

    // write file
    FILE *fp;
    fp = fopen( newrawFileName , "w" );
    if(file == NULL){
        std::cout << "Failed to open empty rawfile" << std::endl;
        return false;
    }
    int size = (infdata.resolution[0]) * (infdata.resolution[1]) * (infdata.resolution[2]);
    // fwrite(test , sizeof(int) , sizeof(test) , fp );
    if(infdata.type == 0){
        fwrite(new_uc_voxelData, sizeof(BYTE) , size, file);
    }
    if(infdata.type == 1){
        fwrite(new_f_voxelData, sizeof(float), size, file);
    }
    if(infdata.type == 2){
        fwrite(new_d_voxelData, sizeof(double), size, file);

    }

    fclose(fp);


    return true;
}

bool RAWmodel_cls::ReadRawFile(FILE *file){

    int size = infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2];
    if(infdata.type == 0){
        fread(uc_voxelData, sizeof(BYTE),size, file);
        for(int i = 0; i < infdata.resolution[2]; i++){
            for(int j = 0; j < infdata.resolution[1]; j++){
                for(int k = 0; k < infdata.resolution[0]; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    rawData[i][j][k].layer = (short int)uc_voxelData[num];
                    // cout << (int)uc_voxelData[num] << ", ";
                    if(rawData[i][j][k].layer == 0){
                        layernum++;
                    }
                }
                // cout << "\n";
            }
            // cout << "\n\n";
        }
        return true;
    }else if(infdata.type == 1){
        fread(f_voxelData, sizeof(float),size, file);
        for(int i = 0; i < infdata.resolution[2]; i++){
            for(int j = 0; j < infdata.resolution[1]; j++){
                for(int k = 0; k < infdata.resolution[0]; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    rawData[i][j][k].layer = (short int)f_voxelData[num];
                    // cout << rawData[i][j][k].layer << " ";
                    if(rawData[i][j][k].layer == 0){
                        layernum++;
                    }
                }
                // cout << "\n";
            }
            // cout << "\n\n";
        }
        return true;
    }else if(infdata.type == 2){
        fread(d_voxelData, sizeof(double),size, file);
        for(int i = 0; i < infdata.resolution[2]; i++){
            for(int j = 0; j < infdata.resolution[1]; j++){
                for(int k = 0; k < infdata.resolution[0]; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    rawData[i][j][k].layer = (short int)d_voxelData[num];
                    if(rawData[i][j][k].layer == 0){
                        layernum++;
                    }
                }
            }
        }
        return true;
    }
    return false;

}

void RAWmodel_cls::SetVoxelData(){

    FindOutterLayer();

    for(int y = 0; y < infdata.resolution[2]; y++){
        for(int x = 0; x < infdata.resolution[1]; x++){
            for(int z = 0; z < infdata.resolution[0]; z++){
                if(rawData[y][x][z].layer == 0){
                    voxelModel.voxel.push_back(USVoxData_t{{x,y,z},{}});
                    // findSurfaceVoxel(y, x, z, voxelModel.voxel.size()-1, 0);
                    setMaxbounder(x, y, z);
                    voxelModel.num++;
                }
                int num = z + x*infdata.resolution[0] + y*infdata.resolution[0]* infdata.resolution[1];
                if(infdata.type == 0){
                    new_uc_voxelData[num] = (BYTE)rawData[y][x][z].layer;
                }
                if(infdata.type == 1){
                    new_f_voxelData[num] = (float)rawData[y][x][z].layer;
                }
                if(infdata.type == 2){
                    new_d_voxelData[num] = (double)rawData[y][x][z].layer;
                }

            }
        }
    }
}

void RAWmodel_cls::FindOutterLayer(){
     for(short int x = 0; x < infdata.resolution[1]; x++){
            for(short int y = 0; y < infdata.resolution[2]; y++){
                bool zero = false;
                for(short int z = 0; z < infdata.resolution[0]; z++){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y-1][x][z].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
            for(short int y = infdata.resolution[2]-1; y >= 0; y--){
                bool zero = false;
                for(short int z = infdata.resolution[0]-1; z >= 0; z--){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y+1][x][z].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
        }
        for(short int y = 0; y < infdata.resolution[2]; y++){
            for(short int z = 0; z < infdata.resolution[0]; z++){
                bool zero = false;
                for(short int x = 0; x < infdata.resolution[1]; x++){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y][x][z-1].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
            for(short int z = infdata.resolution[0]-1; z >= 0; z--){
                bool zero = false;
                for(short int x = infdata.resolution[1]-1; x >= 0; x--){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y][x][z+1].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
        }
        for(short int z = 0; z < infdata.resolution[0]; z++){
            for(short int x = 0; x < infdata.resolution[1]; x++){
                bool zero = false;
                for(short int y = 0; y < infdata.resolution[2]; y++){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y][x-1][z].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
            for(short int x = infdata.resolution[1]-1; x >= 0; x--){
                bool zero = false;
                for(short int y = infdata.resolution[2]-1; y >= 0; y--){
                    if(zero){
                        if(rawData[y][x][z].layer > 0)
                            if(rawData[y][x+1][z].layer < 0)
                                rawData[y][x][z].layer*=-1;
                    }else{
                        if(rawData[y][x][z].layer == 0) zero = true;
                        else if(rawData[y][x][z].layer > 0) rawData[y][x][z].layer*=-1;
                    }
                }
            }
        }
}

void RAWmodel_cls::setMaxbounder(int i, int j, int k){
    if(voxelModel.maxsize[0] < i) voxelModel.maxsize[0] = i;
    if(voxelModel.minsize[0] > i) voxelModel.minsize[0] = i;
    if(voxelModel.maxsize[1] < j) voxelModel.maxsize[1] = j;
    if(voxelModel.minsize[1] > j) voxelModel.minsize[1] = j;
    if(voxelModel.maxsize[2] < k) voxelModel.maxsize[2] = k;
    if(voxelModel.minsize[2] > k) voxelModel.minsize[2] = k;
}
void RAWmodel_cls::findSurfaceVoxel(int y, int x, int z, int num, int layer){

    if(z+1 < infdata.resolution[0]){
        if(rawData[y][x][z+1].layer != layer-1){
            voxelModel.voxel[num].faceAir[0] = true;
        }
    }
    if(z-1 >= 0){
        if(rawData[y][x][z-1].layer != layer-1){
            voxelModel.voxel[num].faceAir[1] = true;
        }
    }
    if(x+1 < infdata.resolution[1]){
        if(rawData[y][x+1][z].layer != layer-1){
            voxelModel.voxel[num].faceAir[2] = true;
        }
    }
    if(x-1 >= 0){
        if(rawData[y][x-1][z].layer != layer-1){
            voxelModel.voxel[num].faceAir[3] = true;
        }
    }
    if(y+1 < infdata.resolution[2]){
        if(rawData[y+1][x][z].layer != layer-1){
            voxelModel.voxel[num].faceAir[4] = true;
        }
    }
    if(y-1 >= 0){
        if(rawData[y-1][x][z].layer != layer-1){
            voxelModel.voxel[num].faceAir[5] = true;
        }
    }

}
bool RAWmodel_cls::SetSampleType(const char* type){
    if (!strcmp(type, "unsigned")){
        memset(infdata.sampleType, '\0', sizeof(infdata.sampleType));
		strcat(infdata.sampleType, "unsigned char");
        infdata.type = 0;
        return true;
    }else if(!strcmp(type, "float")){
        memset(infdata.sampleType, '\0', sizeof(infdata.sampleType));
		strcat(infdata.sampleType, "float");
        infdata.type = 1;
        return true;
    }else if(!strcmp(type, "double")){
        memset(infdata.sampleType, '\0', sizeof(infdata.sampleType));
		strcat(infdata.sampleType, "double");
        infdata.type = 2;
        return true;
    }
    return false;
}


void RAWmodel_cls::checkComputerEndian(){
    short int a = 0x1234;
    char *p = (char *)&a;

    printf("p=%#hhx\n",*p);

    if(*p == 0x34)
        printf("Little endian \n");
    else if(*p == 0x12)
        printf("Big endian \n");
    else
        printf("Unknow endian \n");

}