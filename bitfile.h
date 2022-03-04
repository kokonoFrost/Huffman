#ifndef __BITFILE_H
#define __BITFILE_H
#include <cstdio>
#include <stdarg.h>
#include <iostream>

//=============读取模式=========================
#define MODE_R 1
//=============写入模式=========================
#define MODE_W 2
//=============没有打开=========================
#define MODE_N 3

using namespace std;

class bitfile {
public:
	bitfile();
	virtual~bitfile();
	//=================写入1bit====================
	bool writebit(int bit);
	//=================读取1bit====================
	bool readbit(int &bit);
	//=================写入1byte===================
	bool writebyte(unsigned char byte);
	//=================读取1byte===================
	bool readbyte(unsigned char& byte);
	bool writemem(void* psrc, int len);
	bool readmem(void* pdst, int len);
	bool prin(char*, ...);
	//=================打开文件====================
	bool open(char* filename, int mode);
	//=================关闭文件====================
	bool close();
	long getfsize(char* fname);
private:
	//=================操作模式====================
	int mMode;
	//=================寄存器======================
	unsigned char mcht;
	//===============bit的位置指针=================
	int mmask;
	//=================文件指针====================
	FILE* mfile;
};

#endif