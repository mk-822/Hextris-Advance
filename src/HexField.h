#ifndef HEXFIELDHEADER
#define HEXFIELDHEADER

class HexField{
private:
	int field[17][24];
	enum{
		NOTHING_BLOCK_NO=0
	};
public:
	HexField(){}
	void Initialize(){
		for(int i=0 ; i<17 ; i++){
			for(int j=0 ; j<24 ; j++){
				field[i][j] = 0;
			}
		}
	}
	int Get(int x, int y){
		if(x%2)
			y -= 1;
		if(y%2)
			return NOTHING_BLOCK_NO;

		x += 2 , y = y / 2 + 2 ;
		return field[x][y];
	}
	int GetN(int x, int y){
		return field[x][y];
	}
	int GetField(int x,int y){
		return field[x+2][y+2];
	}
	void Set(int x, int y, int value){
		if(x%2)
			y -= 1;
		if(y%2)
			return;

		x += 2 , y = y / 2 + 2 ;
		field[x][y] = value;
	}
	void SetN(int x, int y, int value){
		field[x][y] = value;
	}
	void SetField(int x,int y,int value){
		field[x+2][y+2] = value;
	}
};

#endif