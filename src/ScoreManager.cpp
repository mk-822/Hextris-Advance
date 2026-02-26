// ScoreManager.cpp: ScoreManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScoreManager.h"
#include <string.h>
#include "common.h"
#include "easyscore_bin.h"
#include "normalscore_bin.h"
#include "masterscore_bin.h"
#include "deathscore_bin.h"

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

void _load_score_list(const unsigned char* data, const int size, ScoreManager::Scorelist& score_list)
{
    data_parser parser(data, size);

    for(int i=0 ; i<10 ; i++)
    {
        parser.read_int(score_list.record[i].score);
        parser.read_int(score_list.record[i].time);
        parser.read_int(score_list.record[i].level);
        parser.read_name(score_list.record[i].name, sizeof(score_list.record[i].name));
    }
}

}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ScoreManager::ScoreManager()
{
    _load_score_list(easyscore_bin, easyscore_bin_size, scoreList[0]);
    _load_score_list(normalscore_bin, normalscore_bin_size, scoreList[1]);
    _load_score_list(masterscore_bin, masterscore_bin_size, scoreList[2]);
    _load_score_list(deathscore_bin, deathscore_bin_size, scoreList[3]);
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

	// ROM 上の bin データは書き換えできないため、保存は実施しない。
}
