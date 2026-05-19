// ScoreDisplay.cpp: ScoreDisplay Class implementation
//
//////////////////////////////////////////////////////////////////////
#include "ScoreDisplay.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_items_score_font_bg.h"
#include "bn_regular_bg_items_scorebg.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_utility.h"

namespace
{
	constexpr int TEXT_MAP_COLUMNS = 32;
	constexpr int TEXT_MAP_ROWS = 64;
	constexpr int VISIBLE_TEXT_COLUMNS = 30;
	constexpr int LEADING_BLANK_ROWS = 12;
	constexpr int DIFFICULTY_ROW_COUNT = 13;
	constexpr int SCROLL_FRAME_DIVIDER = 3;

	const char* DIFFICULTY_NAMES[] = {
		"EASY",
		"NORMAL",
		"MASTER",
		"DEATH"
	};

	void _fill_score_line(char* line)
	{
		for(int i = 0; i < VISIBLE_TEXT_COLUMNS; ++i)
		{
			line[i] = ' ';
		}

		line[VISIBLE_TEXT_COLUMNS] = '\0';
	}

	void _write_text(char* line, int x, const char* text, int width)
	{
		for(int i = 0; i < width && text[i] != '\0'; ++i)
		{
			line[x + i] = text[i];
		}
	}

	void _write_int_right(char* line, int x, int width, int value)
	{
		char tmp[16];
		compat_sprintf(tmp, "%d", value);
		int len = compat_strlen(tmp);
		if(len > width)
		{
			len = width;
		}

		const int start = x + width - len;
		for(int i = 0; i < len; ++i)
		{
			line[start + i] = tmp[i];
		}
	}
}

//////////////////////////////////////////////////////////////////////
// construction/extinction
//////////////////////////////////////////////////////////////////////
char* FrameCountToStr(int,char*);
ScoreDisplay :: ScoreDisplay()
{
	count = -51;
	remove = 0;
	FadeLevel = 0;
	spaceCell = 0;

	for(int i = 0; i < 128; ++i)
	{
		glyphCells[i] = 0;
	}
}

void ScoreDisplay :: Main()
{
	Sound::ChangeBgm(7);
	DrawBackground();
	InitializeTextBg();

	if(count < 0)
	{
		Fade_In();
	}

	if(count == 0)
	{
		FadeLevel = 254;
	}

	if(count >= 0)
	{
		DrawScoreText();
	}

	if(input->GetKeyState(0,2) & BUTTON[0])
	{
		remove = 1;
	}

	if(remove)
	{
		count = Fade_Out();
	}
	else
	{
		++count;
	}
}

void ScoreDisplay ::DrawBackground()
{
	if(! scoreBg)
	{
		scoreBg = bn::regular_bg_items::scorebg.create_bg(0, 0);
		scoreBg->set_priority(3);
		scoreBg->set_z_order(1);
		scoreBg->set_top_left_x(0);
		scoreBg->set_top_left_y(0);
	}
}

void ScoreDisplay ::InitializeTextBg()
{
	if(textBg)
	{
		return;
	}

	bn::regular_bg_map_ptr map = bn::regular_bg_map_ptr::allocate(
			bn::size(TEXT_MAP_COLUMNS, TEXT_MAP_ROWS),
			bn::regular_bg_items::score_font_bg.tiles_item().create_tiles(false),
			bn::regular_bg_items::score_font_bg.palette_item().create_palette());
	const int font_palette_id = map.palette().id();

	for(int i = 0; i < 128; ++i)
	{
		const int cell_x = i % 16;
		const int cell_y = i / 16;
		const int map_index = cell_y * TEXT_MAP_COLUMNS + cell_x;
		bn::regular_bg_map_cell_info info(score_font_bg_bn_gfxMap[map_index]);
		info.set_palette_id(font_palette_id);
		glyphCells[i] = info.cell();
	}

	spaceCell = glyphCells[0];
	bn::optional<bn::span<bn::regular_bg_map_cell>> vram = map.vram();
	if(vram)
	{
		for(bn::regular_bg_map_cell& cell : *vram)
		{
			cell = spaceCell;
		}
	}

	textBg = bn::regular_bg_ptr::create(0, 0, bn::move(map));
	textBg->set_priority(0);
	textBg->set_z_order(0);
	textBg->set_top_left_x(0);
	textBg->set_top_left_y(0);
	PopulateTextMap();
}

void ScoreDisplay ::ClearTextMap()
{
	if(! textBg)
	{
		return;
	}

	bn::regular_bg_map_ptr map = textBg->map();
	bn::optional<bn::span<bn::regular_bg_map_cell>> vram = map.vram();
	if(! vram)
	{
		return;
	}

	for(bn::regular_bg_map_cell& cell : *vram)
	{
		cell = spaceCell;
	}
}

void ScoreDisplay ::DrawScoreText()
{
	if(! textBg)
	{
		return;
	}

	const int scroll_units = (count / SCROLL_FRAME_DIVIDER) % (TEXT_MAP_ROWS * 8);
	textBg->set_top_left_y(-scroll_units);
}

void ScoreDisplay ::PopulateTextMap()
{
	ClearTextMap();

	char line[40];
	for(int row = 0; row < TEXT_MAP_ROWS; ++row)
	{
		BuildScrollLine(row - LEADING_BLANK_ROWS, line);
		DrawTextRow(row, line);
	}
}

void ScoreDisplay ::DrawTextRow(int row, const char* text)
{
	if(! textBg || row < 0 || row >= TEXT_MAP_ROWS)
	{
		return;
	}

	bn::regular_bg_map_ptr map = textBg->map();
	bn::optional<bn::span<bn::regular_bg_map_cell>> vram = map.vram();
	if(! vram)
	{
		return;
	}

	const int len = compat_strlen(text);
	int col = (VISIBLE_TEXT_COLUMNS - len) / 2;
	if(col < 0)
	{
		col = 0;
	}

	for(int i = 0; i < len && col + i < TEXT_MAP_COLUMNS; ++i)
	{
		const char chr = text[i];
		if(chr == ' ')
		{
			continue;
		}

		int glyph = static_cast<unsigned char>(chr) - 32;
		if(glyph < 0 || glyph >= 128)
		{
			glyph = 0;
		}

		(*vram)[row * TEXT_MAP_COLUMNS + col + i] = glyphCells[glyph];
	}
}

void ScoreDisplay ::BuildScrollLine(int contentRow, char* line)
{
	line[0] = '\0';
	if(contentRow < 0)
	{
		return;
	}

	const int difficulty = contentRow / DIFFICULTY_ROW_COUNT;
	if(difficulty < 0 || difficulty >= 4)
	{
		return;
	}

	const int row = contentRow % DIFFICULTY_ROW_COUNT;
	if(row == 1)
	{
		compat_strcpy(line, DIFFICULTY_NAMES[difficulty]);
		return;
	}

	if(row >= 3 && row < 3 + SCORE_RECORD_COUNT)
	{
		char time[9];
		const int record_index = row - 3;
		const ScoreManager::Record& record = Score.scoreList[difficulty].record[record_index];
		_fill_score_line(line);
		_write_int_right(line, 0, 2, record_index + 1);
		line[2] = '.';
		_write_text(line, 5, record.name, 3);
		_write_int_right(line, 9, 7, record.score);
		_write_int_right(line, 17, 4, record.level);
		_write_text(line, 22, FrameCountToStr(record.time, time), 8);
	}
}

void ScoreDisplay ::Fade_In()
{
	dxg->TexturePos();
	dxg->ColorChange(1,true);
	dxg->Draw(image->i[BLANK_IMG],0,0,true,255-FadeLevel);
	FadeLevel+=5;
}

int ScoreDisplay::Fade_Out()
{
	dxg->TexturePos();
	dxg->ColorChange(1,true);
	dxg->Draw(image->i[BLANK_IMG],0,0,true,255-FadeLevel);
	FadeLevel-=5;
	if(FadeLevel <= 0)
	{
		textBg.reset();
		scoreBg.reset();
		*scene = TITLE_SCENE;
	}
	return ++count;
}
