#pragma once
#include <vector>
#include <iostream>
using namespace std; 

class Tile
{
private:
	int xpos;
	int ypos; 
	bool hasflag;
	bool hasmine;
	bool revealed;
	int surrondingmines;
	vector<Tile*> adjacenttiles;

public:

	Tile() {
		xpos = 0;
		ypos = 0;
	    hasflag = 0;
		hasmine = 0;
		revealed = 0;
		surrondingmines = 0;
		
	}
	Tile(int& x, int& y, bool hasflag, bool hasmine, bool revealed) {
		xpos = x;
		ypos = y;
		this->hasflag = hasflag;
		this->hasmine = hasmine;
		this->revealed = revealed;
		surrondingmines = 0;
		
	}
	bool gethasflag(){
		return hasflag;
	}
	bool gethasmine(){
		return hasmine;
	}
	bool getrevealed() {
		return revealed;
	}
	int getsurrondingmines() {
		return surrondingmines;
	}
	 void sethasflag(bool x) {
		 hasflag = x;
	}
	 void sethasmine() {
		 hasmine = true;
	 }
	 void setrevealed() {
		 revealed = true; 
	 }
	 void setsurrondingmines(int x) {
		 surrondingmines = x;
	 }
	 
     vector<Tile*>& getVector() {
		 return adjacenttiles;
	 }
	 int getsizeofneighbors() {
		 return adjacenttiles.size();
	 }

	 void setNeighbors(Tile**& tilegrid, int rows, int columns, int posx, int posy) {
		 int positiony[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		 int positionx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		 for (int i = 0; i < 8; i++) {
			 int row = posy + positiony[i];
			 int column = posx + positionx[i];
			 if (row >= 0 && column >= 0 && row < rows && column < columns) {
				 adjacenttiles.push_back(&tilegrid[row][column]);
			 }
		 }
	 }

};

