// ScoreManager.cpp: ScoreManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScoreManager.h"
#include <string.h>
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

    bool read_name(char* name, const int max_size)
    {
        _skip_white_space();

        if(_current >= _end || max_size <= 0)
        {
            if(max_size > 0)
            {
                name[0] = '\0';
            }

            return false;
        }

        int index = 0;

        while(_current < _end)
        {
            const char chr = *_current;

            if(chr == ' ' || chr == '\n' || chr == '\r' || chr == '\t')
            {
                break;
            }

            if(index < max_size - 1)
            {
                name[index] = chr;
                ++index;
            }

            ++_current;
        }

        name[index] = '\0';
        return index > 0;
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

ScoreManager::ScoreManager()
{
	// Butano が自動生成する generic_data_bin から読み込み
	data_parser parser(generic_data_bin, generic_data_bin_size);

	for(int i=0 ; i<10 ; i++){
		parser.read_int(scoreList[0].record[i].score);
		parser.read_int(scoreList[0].record[i].time);
		parser.read_int(scoreList[0].record[i].level);
		parser.read_name(scoreList[0].record[i].name, sizeof(scoreList[0].record[i].name));
	}

	for(int i=0 ; i<10 ; i++){
		parser.read_int(scoreList[1].record[i].score);
		parser.read_int(scoreList[1].record[i].time);
		parser.read_int(scoreList[1].record[i].level);
		parser.read_name(scoreList[1].record[i].name, sizeof(scoreList[1].record[i].name));
	}

	for(int i=0 ; i<10 ; i++){
		parser.read_int(scoreList[2].record[i].score);
		parser.read_int(scoreList[2].record[i].time);
		parser.read_int(scoreList[2].record[i].level);
		parser.read_name(scoreList[2].record[i].name, sizeof(scoreList[2].record[i].name));
	}

	for(int i=0 ; i<10 ; i++){
		parser.read_int(scoreList[3].record[i].score);
		parser.read_int(scoreList[3].record[i].time);
		parser.read_int(scoreList[3].record[i].level);
		parser.read_name(scoreList[3].record[i].name, sizeof(scoreList[3].record[i].name));
	}
}

ScoreManager::~ScoreManager()
{
}

int ScoreManager::JudgeOrder(int score, int difficulty)
{
	for(int i=0 ; i<10 ; i++){
		if(scoreList[difficulty].record[i].score < score){
			return i;
		}
	}
	return -1;
}

void ScoreManager::EntryScore(int order, int difficulty, int score, int level, int time, char *name)
{
	for(int i=8 ; i>=order ; i--){
		scoreList[difficulty].record[i+1] = scoreList[difficulty].record[i];
	}
	scoreList[difficulty].record[order].score = score;
	scoreList[difficulty].record[order].level = level;
	scoreList[difficulty].record[order].time = time;
	compat_strcpy(scoreList[difficulty].record[order].name,name);

	// ROM 上の generic_data_bin は書き換えできないため、保存は実施しない。
}
