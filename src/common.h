#ifndef COMMONHEADER
#define COMMONHEADER

#include <cstdarg>
#include "bn_timer.h"
#include "bn_timers.h"
#include "Sound.h"

inline int __attribute__((optimize("O0"))) compat_strlen(const char* text){
	if(! text){
		return 0;
	}

	int i=0;
	for(;*text;i++){
		text++;
	}

	return i;
}

inline char* compat_strcpy(char* destination, const char* source){
	if(! destination){
		return destination;
	}

	if(! source){
		destination[0] = '\0';
		return destination;
	}

	int i = 0;
	while(source[i] != '\0'){
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';

	return destination;
}

inline int compat_append_char(char* destination, int index, char value){
	destination[index] = value;
	return index + 1;
}

inline int compat_append_text(char* destination, int index, const char* text){
	if(! text){
		text = "(null)";
	}

	for(int i = 0; text[i] != '\0'; ++i){
		destination[index++] = text[i];
	}

	return index;
}

inline int compat_append_int(char* destination, int index, int value, int min_width, bool zero_pad){
	char tmp[16];
	int tmp_len = 0;
	int current = value;
	bool negative = current < 0;

	if(negative){
		current = -current;
	}

	do{
		tmp[tmp_len++] = static_cast<char>('0' + (current % 10));
		current /= 10;
	} while(current > 0);

	if(negative){
		tmp[tmp_len++] = '-';
	}

	while(tmp_len < min_width){
		tmp[tmp_len++] = zero_pad ? '0' : ' ';
	}

	for(int i = tmp_len - 1; i >= 0; --i){
		destination[index++] = tmp[i];
	}

	return index;
}

inline int compat_vsprintf(char* destination, const char* format, va_list arguments){
	int index = 0;

	for(int i = 0; format && format[i] != '\0'; ++i){
		if(format[i] != '%'){
			index = compat_append_char(destination, index, format[i]);
			continue;
		}

		++i;
		if(format[i] == '%'){
			index = compat_append_char(destination, index, '%');
			continue;
		}

		bool zero_pad = false;
		int min_width = 0;
		if(format[i] == '0'){
			zero_pad = true;
			++i;
		}

		while(format[i] >= '0' && format[i] <= '9'){
			min_width = min_width * 10 + (format[i] - '0');
			++i;
		}

		switch(format[i]){
		case 'd':
			index = compat_append_int(destination, index, va_arg(arguments, int), min_width, zero_pad);
			break;
		case 'c':
			index = compat_append_char(destination, index, static_cast<char>(va_arg(arguments, int)));
			break;
		case 's':
			index = compat_append_text(destination, index, va_arg(arguments, const char*));
			break;
		default:
			index = compat_append_char(destination, index, '%');
			index = compat_append_char(destination, index, format[i]);
			break;
		}
	}

	destination[index] = '\0';
	return index;
}

inline int compat_sprintf(char* destination, const char* format, ...){
	va_list arguments;
	va_start(arguments, format);
	const int result = compat_vsprintf(destination, format, arguments);
	va_end(arguments);
	return result;
}

inline bool compat_parse_int(const char* text, const char* text_end, int& value, const char*& parse_end){
	if(! text){
		value = 0;
		parse_end = text;
		return false;
	}

	const char* current = text;
	const bool has_range_limit = text_end != nullptr;

	if(has_range_limit && current >= text_end){
		value = 0;
		parse_end = text;
		return false;
	}

	int sign = 1;
	if(*current == '+' || *current == '-'){
		sign = (*current == '-') ? -1 : 1;
		++current;
	}

	int parsed_value = 0;
	bool has_digit = false;

	while(current && (! has_range_limit || current < text_end)){
		const char chr = *current;

		if(chr < '0' || chr > '9'){
			break;
		}

		has_digit = true;
		parsed_value = parsed_value * 10 + (chr - '0');
		++current;
	}

	if(! has_digit){
		value = 0;
		parse_end = text;
		return false;
	}

	value = parsed_value * sign;
	parse_end = current;
	return true;
}

inline unsigned long GetTickCount(){
	static bn::timer timer;
	const int elapsed_ticks = timer.elapsed_ticks();
	const int ticks_per_second = bn::timers::ticks_per_second();

	return static_cast<unsigned long>(
		(static_cast<long long>(elapsed_ticks) * 1000) / ticks_per_second
	);
}

//画面モード定義
static const int WINDOW_WIDE = 320;
static const int WINDOW_HEIGHT = 240;
extern bool WINDOW_FULL;
extern int WINDOW_ZOOM;

//画像ファイル定義
static const int RANK_IMG = 0;
static const int BLANK_IMG = 1;
static const int FONT_IMG = 2;
static const int FONTEX_IMG = 3;
static const int BLOCK_IMG = 4;
static const int LOGO_IMG = 5;
static const int TITLE_IMG = 6;
static const int WORDS_IMG = 7;
static const int FRAME_IMG = 8;
static const int BG_IMG[] = {9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
static const int SCOREBG_IMG = 26;
static const int BIGINT_IMG = 27;
static const int BIGFONT_IMG = 28;
static const int WHITEFONT_IMG = 29;
static const int OP_BG_IMG = 30;
static const int GRAYFONT_IMG = 31;
static const int MULTI_IMG = 32;

//シーン管理定義
static const int LOGO_SCENE = 0;			// ロゴ画面
static const int TITLE_SCENE = 1;			// タイトル画面
//static const int OPTION_SCENE = 2;			//オプション
static const int SCORE_SCENE = 3;			// スコアランキング
static const int SINGLE_SCENE = 4;		// シングルプレイ
//static const int MULTI_SCENE = 5;		// マルチプレイ

//ブレンド方法定義
static const int NORMAL_BLEND = 0;	// 通常ブレンド
static const int TRANS_BLEND = 1;	// 透過付きブレンド
static const int ADD_BLEND = 2;		// 加算合成

#endif
