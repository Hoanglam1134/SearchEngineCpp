#pragma once
#include "InOut.h"
using namespace std;

struct VectorInt{
	int cap = 0;
    int size = 0;
	int* a = nullptr;
	void pushBack(int x){
		a[size++] = x;
	}
	void del(){
        if(a) delete[]a;
	}
    void init(int sz, int val = 0){
        del();
        cap = sz;
        size = 0;
        a = new int[cap];
        for (int i = 0; i < cap; i++)
            a[i] = val;
    }
    void quickSort(int left, int right) {
        int i = left, j = right;
        int mid = a[(left + right) / 2];
        do {
            while (a[i] < mid && i < right) i++;
            while (a[j] > mid && j > left) j--;
            if (i <= j) {
                swap(a[j], a[i]);
                i++; j--;
            }
        } while (i <= j);
        if (left < j) quickSort(left, j);
        if (i < right) quickSort(i, right);
    }
    VectorInt& operator =(const VectorInt& cur)
    {
        cap = cur.cap;
        size = cur.size;
        a = cur.a;
        return *this;
    }
};

struct VectorStr {
	int cap = 0;
	int size = 0;
	string *s = nullptr;
	void del() {
        if(s) delete[]s;
	}
    void init(int sz, string val = ""){
        del();
        cap = sz;
        size = 0;
        s = new string[cap];
        for (int i = 0; i < cap; i++)
            s[i] = val;
    }
	void pushBack(string x){
		s[size++] = x;
	}
    void quickSort(int left, int right){
        int i = left, j = right;
        string mid = s[(left + right) / 2];
        do {
            while (s[i] < mid && i < right) i++;
            while (s[j] > mid && j > left) j--;
            if (i <= j) {
                swap(s[j], s[i]);
                i++; j--;
            }
        } while (i <= j);
        if (left < j) quickSort(left, j);
        if (i < right) quickSort(i, right);
    }
    int lowerBound(string x){
        int l = 0, r = size - 1, res = size;
        while(l <= r){
            int mid = (l + r) / 2;
            if (s[mid] >= x) {
                res = mid;
                r = mid - 1;
            }else{
                l = mid + 1;
            }
        }
        return res;
    }
    void unique(){
        if (size <= 1) return;
        int j = 0;
        for (int i = 0; i < size-1; i++)
            if (s[i] != s[i+1])
                s[j++] = s[i];
        s[j++] = s[size-1];
        size = j;
    }
    VectorStr& operator =(const VectorStr& cur) {
        cap = cur.cap;
        size = cur.size;
        s = cur.s;
        return *this;
    }
};

struct PairSI {
    string s;
    int a;
    int cmpPSI(PairSI y) {
        if (a > y.a) return 1;
        if (a < y.a) return -1;
        if (a == y.a) return 0;
    }
};

struct VectorPSI {
    int cap = 0;
    int size = 0;
    PairSI* p = nullptr;
    void del(){
        if(p) delete[]p;
    }
    void init(int sz, PairSI val = { "", 0 }) {   
        cap = sz;
        size = 0;
        p = new PairSI[cap];
        for (int i = 0; i < cap; i++)
            p[i] = val;
    }
    void pushBack(PairSI x) {
		p[size++] = x;
	}
    void quickSort(int left, int right){
        int i = left, j = right;
        PairSI mid = p[(left + right) / 2];
        do {
            while (p[i].cmpPSI(mid) == 1 && i < right) i++;
            while (p[j].cmpPSI(mid) == -1 && j > left) j--;
            if (i <= j) {
                swap(p[j], p[i]);
                i++; j--;
            }
        } while (i <= j);
        if (left < j) quickSort(left, j);
        if (i < right) quickSort(i, right);
    }
};