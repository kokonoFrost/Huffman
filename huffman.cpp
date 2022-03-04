#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <queue>
#include "bitfile.h"

using namespace std;

struct word {
	int key, num;
	word *left, *right;
	word(): key(-1), num(-1), left(NULL), right(NULL) {}
	word(int x, int y): key(x), num(y), left(NULL), right(NULL) {}
	word(int x, int y, word *m, word *n): key(x), num(y), left(m), right(n) {}
}*head;
struct cmp {
	bool operator()(const word* a, const word* b) const {
		return a->num > b->num;
	}
};
string str[256];
bool encode(char *, char *);
bool decode(char *, char *);
void tree(int *);
void dfs(word *, string);

int main(int argc, char const *argv[]) {
	string en = "-encode", de = "-decode";
	if (argc == 4) {
		string s = argv[1];
		if (s == en) {
			if (!encode((char *)argv[2], (char *)argv[3]))
				printf("Encode Failed\n");
		} else if (s == de) {
			if (!decode((char *)argv[2], (char *)argv[3]))
				printf("Decode Failed\n");
		} else {
			printf("Please Check The Command\n");
		}
	} else {
		printf("Please Check The Command\n");
	}
	return 0;
}
bool encode(char *infile, char *outfile) {
	bitfile ifile, ofile;
	if (!ifile.open(infile, MODE_R)) return 0;
	ofile.open(outfile, MODE_W);
	unsigned char byte;
	int fre[256];
	memset(fre, 0, sizeof(fre));
	while (ifile.readbyte(byte))
		fre[(int)byte]++;
	for (int i = 0; i < 256; i++)
		ofile.writemem(&fre[i], sizeof(int));
	tree(fre);
	ifile.close();
	ifile.~bitfile();
	if (!ifile.open(infile, MODE_R)) return 0;
	long filelength = 0;
	while (ifile.readbyte(byte))
		filelength += str[byte].size();
	ofile.writemem(&filelength, sizeof(long));
	ifile.close();
	ifile.~bitfile();
	if (!ifile.open(infile, MODE_R)) return 0;
	while (ifile.readbyte(byte)) {
		for (int i = 0; i < str[byte].size(); i++)
			ofile.writebit(str[byte][i] - '0');
	}
	ifile.close(); ofile.close();
	return 1;
}
bool decode(char *infile, char *outfile) {
	bitfile ifile, ofile;
	if (!ifile.open(infile, MODE_R)) return 0;
	ofile.open(outfile, MODE_W);
	int fre[256];
	for (int i = 0; i < 256; i++)
		ifile.readmem(&fre[i], sizeof(int));
	long filelength;
	ifile.readmem(&filelength, sizeof(long));
	tree(fre);
	string s = "";
	int bit;
	word *node = head;
	while (ifile.readbit(bit) && filelength > 0) {
		filelength--;
		if (bit)
			node = node->right;
		else
			node = node->left;
		if (node->key != -1) {
			ofile.writebyte(node->key);
			node = head;
		}
	}
	ifile.close(); ofile.close();
	return 1;
}
void tree(int *fre) {
	priority_queue<word *, vector<word *>, cmp> q;
	for (int i = 0; i < 256; i++)
		if (fre[i] != 0) {
			word *temp = new word(i, fre[i]);
			q.push(temp);
		}
	if (q.size() == 1) {
		word *temp1 = q.top();
		head->left = temp1;
		str[temp1->key] = "0";
	} else {
		while (q.size() > 1) {
			word *temp1 = q.top(); q.pop();
			word *temp2 = q.top(); q.pop();
			word *temp3 = new word(-1, temp1->num + temp2->num);
			temp3->left = temp1; temp3->right = temp2;
			q.push(temp3);
		}
		string t = "";
		head = q.top();
		dfs(head, t);
	}
}
void dfs(word *node, string s) {
	if (-1 != node->key) {
		str[node->key] = s;
	}
	if (NULL != node->left) {
		s.push_back('0');
		dfs(node->left, s);
		s.erase(s.size() - 1, 1);
	}
	if (NULL != node->right) {
		s.push_back('1');
		dfs(node->right, s);
		s.erase(s.size() - 1, 1);
	}
}
