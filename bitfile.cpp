#include "bitfile.h"

bitfile::bitfile() {
	mMode = MODE_N;
	mfile = NULL;
	mcht = 0;
}
bitfile::~bitfile() {
	if (mMode == MODE_N && NULL != mfile)
		fclose(mfile);
}
bool bitfile::open(char* filename, int mode) {
	if (mode == MODE_R) {
		mfile = fopen(filename, "rb");
		mMode = mode;
		mmask = 9;
		mcht = 0;
	} else if (mode == MODE_W) {
		mfile = fopen(filename, "wb");
		mMode = mode;
		mmask = 1;
		mcht = 0;
	} else {
		mMode = MODE_N;
	}
	if (mMode != MODE_N && NULL != mfile)
		return 1;
	else {
		if (NULL == mfile)
			printf("Failed Open\n");
		return 0;
	}
}
bool bitfile::close() {
	if (NULL != mfile) {
		if (mMode == MODE_W && mmask != 1)
			fwrite(&mcht, 1, 1, mfile);
		return 1;
	} else
		return 0;
}
bool bitfile::readbit(int& bit) {
	if (NULL == mfile || mMode != MODE_R)
		return 0;
	if (mmask > 8) {
		if (fread(&mcht, 1, 1, mfile) != 1)
			return 0;
		mmask = 1;
	}
	bit = (mcht & (1 << (8 - mmask++))) ? 1 : 0;
	return 1;
}
bool bitfile::writebit(int bit) {
	if (NULL == mfile || mMode != MODE_W)
		return 0;
	if (mmask > 8) {
		fwrite(&mcht, 1, 1, mfile);
		mcht = 0;
		mmask = 1;
	}
	unsigned char b1 = 1;
	unsigned char b0 = 0;
	mcht |= ((bit) ? b1 : b0) << (8 - mmask++);
	return 1;
}
bool bitfile::readbyte(unsigned char& byte) {
	unsigned char result = 0;
	int bit;
	if (NULL == mfile || mMode != MODE_R)
		return 0;
	for (int i = 1; i <= 8; i++) {
		if (!this->readbit(bit))
			return 0;
		result |= ((bit) ? 1 : 0) << (8 - i);
	}
	byte = result;
	return 1;
}
bool bitfile::writebyte(unsigned char byte) {
	if (NULL == mfile || mMode != MODE_W)
		return 0;
	for (int i = 1; i <= 8; i++)
		this->writebit(((1 << (8 - i))&byte) ? 1 : 0);
	return 1;
}
bool bitfile::writemem(void* psrc, int len) {
	if (NULL == mfile || mMode != MODE_W)
		return 0;
	unsigned char* byte = (unsigned char*) psrc;
	for (int i = 1; i <= len; i++) {
		writebyte(*byte);
		byte++;
	}
	return 1;
}
bool bitfile::readmem(void* pdst, int len) {
	if (NULL == mfile || mMode != MODE_R)
		return 0;
	unsigned char* byte = (unsigned char*) pdst;
	for (int i = 1; i <= len; i++) {
		if (!readbyte(*byte))
			return 0;
		byte++;
	}
	return 1;
}
long bitfile::getfsize(char* fname) {
	long size;
	open(fname, MODE_R);
	fseek(mfile, 0, SEEK_SET);
	fseek(mfile, 0, SEEK_END);
	size = ftell(mfile);
	close();
	return size;
}
bool bitfile::prin(char* fmt, ...) {
	if (NULL == mfile || mMode != MODE_W)
		return 0;
	char buff[1024] = {'\0'};
	va_list args;
	int n;
	va_start(args, fmt);
	n = vsnprintf(buff, sizeof(buff), fmt, args);
	va_end(args);
	for (int i = 0; buff[i] != '\0' && i < 1024; i++)
		this->writebyte(buff[i]);
	return 1;
}
