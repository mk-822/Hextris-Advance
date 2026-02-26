// DataFileLoader.cpp: DataFileLoader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFileLoader.h"
#include "common.h"
#include "rotation_bin.h"
#include "blockdef_bin.h"
#include "grade_bin.h"
#include "easy_bin.h"
#include "normal_bin.h"
#include "master_bin.h"
#include "death_bin.h"

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

        const char* parse_end = _current;

        if(! compat_parse_int(_current, _end, value, parse_end))
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
    {
        data_parser parser(rotation_bin, rotation_bin_size);

        for(int i=0 ; i<19 ; i++)
        {
            parser.read_int(posData[i].x);
            parser.read_int(posData[i].y);
            parser.read_int(posData[i].next);
            parser.read_int(posData[i].previous);
        }
    }

    {
        data_parser parser(blockdef_bin, blockdef_bin_size);

        for(int i=0 ; i<10 ; i++)
        {
            parser.read_int(blockData[i].pos[0]);
            parser.read_int(blockData[i].pos[1]);
            parser.read_int(blockData[i].pos[2]);
            parser.read_int(blockData[i].pos[3]);
            blockData[i].center_x = CENTER_X;
            blockData[i].center_y = CENTER_Y;
            blockData[i].color = i;
        }
    }

    {
        data_parser parser(grade_bin, grade_bin_size);

        for(int i=0 ; i<8 ; i++)
        {
            parser.read_int(borderScore[i]);
        }
    }

    {
        data_parser parser(easy_bin, easy_bin_size);

        for(int j=0 ; j<10 ; j++)
        {
            parser.read_int(difficultyData[EASY][j].fix);
            parser.read_int(difficultyData[EASY][j].erase);
            parser.read_int(difficultyData[EASY][j].appear);
            parser.read_int(difficultyData[EASY][j].fall);
            parser.read_int(difficultyData[EASY][j].g);
        }
    }

    {
        data_parser parser(normal_bin, normal_bin_size);

        for(int j=0 ; j<10 ; j++)
        {
            parser.read_int(difficultyData[NORMAL][j].fix);
            parser.read_int(difficultyData[NORMAL][j].erase);
            parser.read_int(difficultyData[NORMAL][j].appear);
            parser.read_int(difficultyData[NORMAL][j].fall);
            parser.read_int(difficultyData[NORMAL][j].g);
        }
    }

    {
        data_parser parser(master_bin, master_bin_size);

        for(int j=0 ; j<10 ; j++)
        {
            parser.read_int(difficultyData[MASTER][j].fix);
            parser.read_int(difficultyData[MASTER][j].erase);
            parser.read_int(difficultyData[MASTER][j].appear);
            parser.read_int(difficultyData[MASTER][j].fall);
            parser.read_int(difficultyData[MASTER][j].g);
        }
    }

    {
        data_parser parser(death_bin, death_bin_size);

        for(int j=0 ; j<10 ; j++)
        {
            parser.read_int(difficultyData[DEATH][j].fix);
            parser.read_int(difficultyData[DEATH][j].erase);
            parser.read_int(difficultyData[DEATH][j].appear);
            parser.read_int(difficultyData[DEATH][j].fall);
            parser.read_int(difficultyData[DEATH][j].g);
        }
    }
}
