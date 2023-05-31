#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include "RAWmodel.h"

RAWmodel::RAWmodel(){

}

RAWmodel::~RAWmodel(){
    free(bounderVoxelData);
    for(int i = 0; i < infdata.resolution[0]; i++){
        for(int j = 0; j < infdata.resolution[1]; j++){
            free(voxelData[i][j]);
        }
    }
    for(int i = 0; i < infdata.resolution[0]; i++){
        free(voxelData[i]);
    }
    free(voxelData);
}
void RAWmodel::LoadFile(const char* infFileName,const char* rawFileName){
    LoadINFfile(infFileName);
    CreateVoxel();
    LoadRAWfile(rawFileName);

}

bool RAWmodel::LoadINFfile(const char* infFileName){
    FILE *file = NULL;
    errno_t err;                    //record error
    file = fopen(infFileName, "r"); // err = 0 success, err != fail
    if(file == NULL){
        std::cout << "Failed to open rawfile" << std::endl;
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
    std::cout << "voxel-size : "<<infdata.voxelSize[0] << ", " << infdata.voxelSize[1] << ", " << infdata.voxelSize[2] << std::endl;

    fgets(buffer, sizeof(buffer), file);//endian=XXX
    sscanf(buffer, "endian=%s",infdata.endian);
    std::cout << infdata.endian << std::endl;
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

void RAWmodel::CreateVoxel(){
    if(infdata.type == 0){
        uc_voxelData = (BYTE*)malloc(sizeof(BYTE) * infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2]);
    }else if(infdata.type == 1){
        f_voxelData = (float*)malloc(sizeof(float)* infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2]);
    }else if(infdata.type == 2){
        d_voxelData = (double*)malloc(sizeof(double)* infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2]);
    }

    voxelData = (int***)malloc(sizeof(int**) * infdata.resolution[2]);
    for(int i = 0; i < infdata.resolution[2]; i++){
        voxelData[i] = (int**)malloc(sizeof(int*) * infdata.resolution[1]);
        for(int j = 0; j < infdata.resolution[1]; j++){
            voxelData[i][j] = (int*)malloc(sizeof(int) * infdata.resolution[0]);
            for(int k = 0; k < infdata.resolution[0]; k++){
                voxelData[i][j][k] = 0;
            }
        }
    }
}

bool RAWmodel::LoadRAWfile(const char* rawFileName){
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
    fp = fopen( "raw/newball67_dist.raw" , "w" );
    if(file == NULL){
        std::cout << "Failed to open empty rawfile" << std::endl;
        return false;
    }
    int size = infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2];
    // fwrite(test , sizeof(int) , sizeof(test) , fp );
    if(infdata.type == 0){
        fwrite(uc_voxelData, sizeof(BYTE) , size, file);
    }
    if(infdata.type == 1){
        fwrite(f_voxelData, sizeof(float), size, file);
    }
    if(infdata.type == 2){
        fwrite(d_voxelData, sizeof(double), size, file);

    }

    fclose(fp);


    return true;
}

bool RAWmodel::ReadRawFile(FILE *file){
    int size = infdata.resolution[0] * infdata.resolution[1] * infdata.resolution[2];
    bounderNum = 0;
    if(infdata.type == 0){
        fread(uc_voxelData, sizeof(BYTE),size, file);
        for(int i = 1; i < infdata.resolution[2]-1; i++){
            for(int j = 1; j < infdata.resolution[1]-1; j++){
                for(int k = 1; k < infdata.resolution[0]-1; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    voxelData[i][j][k] = uc_voxelData[num];
                    computMaxVoxelData(voxelData[i][j][k]);
                    if(voxelData[i][j][k] == 0){
                        bounderNum++;
                    }
                }
            }
        }
        CreateBounderVoxelLocate();
        return true;
    }else if(infdata.type == 1){
        fread(f_voxelData, sizeof(float),size, file);
        for(int i = 1; i < infdata.resolution[2]-1; i++){
            for(int j = 1; j < infdata.resolution[1]-1; j++){
                for(int k = 1; k < infdata.resolution[0]-1; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    voxelData[i][j][k] = f_voxelData[num];
                    computMaxVoxelData(voxelData[i][j][k]);
                    if(voxelData[i][j][k] == 0){
                        bounderNum++;
                    }
                    // cout <<  f_voxelData[num] << ", ";
                }
            }
        }
        // cout << bounderNum << endl;
        CreateBounderVoxelLocate();
        return true;
    }else if(infdata.type == 2){
        fread(d_voxelData, sizeof(double),size, file);
        for(int i = 1; i < infdata.resolution[2]-1; i++){
            for(int j = 1; j < infdata.resolution[1]-1; j++){
                for(int k = 1; k < infdata.resolution[0]-1; k++){
                    int num = k + j*infdata.resolution[0] + i*infdata.resolution[0]* infdata.resolution[1];
                    voxelData[i][j][k] = d_voxelData[num];
                    computMaxVoxelData(voxelData[i][j][k]);
                    if(voxelData[i][j][k] == 0){
                        bounderNum++;
                    }
                }
            }
        }
        CreateBounderVoxelLocate();
        return true;
    }
    return false;

}
void RAWmodel::computMaxVoxelData(int num){
    if(maxVoxelData < num) maxVoxelData = num;
}
void RAWmodel::CreateBounderVoxelLocate(){
    bounderVoxelData = (VoxData_b*)calloc(bounderNum, sizeof(VoxData_b));
}

void RAWmodel::SetVoxelData(){
    bool inner = false;
    int num = 0;
    for(int y = 1;y < infdata.resolution[2] - 1; y++){
        for(int x = 1; x < infdata.resolution[1]-1; x++){
            bool allinair = true;
            for(int z = 1, inner = false; z < infdata.resolution[0]-1; z++){
                if(voxelData[y][x][z] == 0){
                    inner = true;
                    allinair = false;
                }
                if(inner) voxelData[y][x][z] *= -1;

            }
            for(int z = infdata.resolution[0]-2, inner = false; z > 0; z--){
                if(allinair){
                    voxelData[y][x][z] *= -1;
                }else{
                    if(voxelData[y][x][z] == 0) inner = true;
                    if(inner) voxelData[y][x][z] *= -1;
                }
                int num = z + x*infdata.resolution[0] + y*infdata.resolution[0]*infdata.resolution[1];
                if(infdata.type == 0)
                    uc_voxelData[num] = (unsigned char)voxelData[y][x][z];
                if(infdata.type == 1)
                    f_voxelData[num] = (float)voxelData[y][x][z];
                if(infdata.type == 2)
                    d_voxelData[num] = (double)voxelData[y][x][z];
                // cout << "("<<voxelData[y][x][z] << ", "<<f_voxelData[num]<<") ";

            }
            // cout << "\n";

            for(int z = 1; z < infdata.resolution[0]-1; z++){
                // std::cout << voxelData[i][j][k] <<" ";

                if(voxelData[y][x][z] == 0){
                    bounderVoxelData[num].bounderVoxelLocate = {y,x,z};
                    SetbounderVoxelFaceAir(y,x,z, num, -1);
                    num++;
                }
            }
        }
    }
    // cout << num << endl;
}
void RAWmodel::SetbounderVoxelFaceAir(int i, int j, int k, int num, int donoTouch){

    if(i+1 < infdata.resolution[0]){
        if(voxelData[i+1][j][k] == donoTouch){
            bounderVoxelData[num].bounderVoxelFaceAir[3] = 1;
        }
    }

    if(i-1 >= 0){
        if(voxelData[i-1][j][k] == donoTouch){

            bounderVoxelData[num].bounderVoxelFaceAir[2] = 1;
        }
    }

    if(j+1 < infdata.resolution[1]){
        if(voxelData[i][j+1][k] == donoTouch){
            bounderVoxelData[num].bounderVoxelFaceAir[5] = 1;
        }
    }
    if(j-1 >= 0){

        if(voxelData[i][j-1][k] == donoTouch){
            bounderVoxelData[num].bounderVoxelFaceAir[4] = 1;
        }
    }
    if(k+1 < infdata.resolution[2]){

        if(voxelData[i][j][k+1] == donoTouch){
            bounderVoxelData[num].bounderVoxelFaceAir[1] = 1;
        }
    }
    if(k-1 >= 0){
        if(voxelData[i][j][k-1] == donoTouch){
            bounderVoxelData[num].bounderVoxelFaceAir[0] = 1;
        }

    }

}
bool RAWmodel::SetSampleType(const char* type){
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


void RAWmodel::checkComputerEndian(){
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