// DataFileLoader.cpp: DataFileLoader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFileLoader.h"
#include "common.h"
#include "generic_data_bin.h"
#include <cstdlib>

namespace
{

class data_parser
{
public:
    data_parser(const unsigned char* data, const int size) :
        _current(reinterpret_cast<const char*>(data)),
        _end(reinterpret_cast<const char*>(data) + size)
    {
    }

    bool read_int(int& value)
    {
        _skip_white_space();

        if(_current >= _end)
        {
            value = 0;
            return false;
        }

        char* parse_end = nullptr;
        value = static_cast<int>(strtol(_current, &parse_end, 10));

        if(parse_end == _current)
        {
            value = 0;
            return false;
        }

        _current = parse_end;
        return true;
    }

private:
    void _skip_white_space()
    {
        while(_current < _end)
        {
            const char chr = *_current;

            if(chr == ' ' || chr == '\n' || chr == '\r' || chr == '\t')
            {
                ++_current;
            }
            else
            {
                break;
            }
        }
    }

    const char* _current;
    const char* _end;
};

}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

static const int CENTER_X = 6;
static const int CENTER_Y = 2;

DataFileLoader::DataFileLoader()
{
	// Butano が自動生成する generic_data_bin から読み込み
	data_parser parser(generic_data_bin, generic_data_bin_size);

	for(int i=0 ; i<19 ; i++){
		parser.read_int(posData[i].x);
		parser.read_int(posData[i].y);
		parser.read_int(posData[i].next);
		parser.read_int(posData[i].previous);
	}

	for(int i=0 ; i<10 ; i++){
		parser.read_int(blockData[i].pos[0]);
		parser.read_int(blockData[i].pos[1]);
		parser.read_int(blockData[i].pos[2]);
		parser.read_int(blockData[i].pos[3]);
		blockData[i].center_x = CENTER_X;
		blockData[i].center_y = CENTER_Y;
		blockData[i].color = i;
	}

	for(int i=0 ; i<8 ; i++){
		parser.read_int(borderScore[i]);
	}

	for(int i=0 ; i<4 ; i++){
		for(int j=0 ; j<10 ; j++){
			parser.read_int(difficultyData[i][j].fix);
			parser.read_int(difficultyData[i][j].erase);
			parser.read_int(difficultyData[i][j].appear);
			parser.read_int(difficultyData[i][j].fall);
			parser.read_int(difficultyData[i][j].g);
		}
	}
}
