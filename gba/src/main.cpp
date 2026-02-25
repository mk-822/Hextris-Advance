#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_to_string.h"
#include "bn_vector.h"

#include "common_variable_8x8_sprite_font.h"

namespace
{
    constexpr int board_width = 8;
    constexpr int board_height = 14;

    struct piece
    {
        int x = board_width / 2;
        int y = -2;
        int colors[3] = { 1, 2, 3 };
    };

    class game
    {
    public:
        game() : _text_generator(common::variable_8x8_sprite_font)
        {
            _text_generator.set_center_alignment();
            reset();
        }

        void update()
        {
            if(_game_over)
            {
                if(bn::keypad::start_pressed())
                {
                    reset();
                }

                draw();
                return;
            }

            if(bn::keypad::left_pressed() && !collides(_current.x - 1, _current.y))
            {
                _current.x -= 1;
            }

            if(bn::keypad::right_pressed() && !collides(_current.x + 1, _current.y))
            {
                _current.x += 1;
            }

            if(bn::keypad::a_pressed())
            {
                const int temp = _current.colors[0];
                _current.colors[0] = _current.colors[2];
                _current.colors[2] = _current.colors[1];
                _current.colors[1] = temp;
            }

            ++_fall_counter;
            const int gravity = bn::keypad::down_held() ? 3 : _fall_speed;
            if(_fall_counter >= gravity)
            {
                _fall_counter = 0;
                if(!collides(_current.x, _current.y + 1))
                {
                    _current.y += 1;
                }
                else
                {
                    lock_piece();
                }
            }

            draw();
        }

    private:
        int _board[board_height][board_width] = {};
        piece _current;
        int _score = 0;
        int _lines = 0;
        int _fall_counter = 0;
        int _fall_speed = 30;
        bool _game_over = false;
        bn::random _random;
        bn::sprite_text_generator _text_generator;
        bn::vector<bn::sprite_ptr, 768> _text_sprites;

        [[nodiscard]] bool collides(int x, int y) const
        {
            for(int i = 0; i < 3; ++i)
            {
                const int py = y + i;
                if(x < 0 || x >= board_width)
                {
                    return true;
                }

                if(py >= board_height)
                {
                    return true;
                }

                if(py >= 0 && _board[py][x])
                {
                    return true;
                }
            }

            return false;
        }

        void spawn_piece()
        {
            _current.x = board_width / 2;
            _current.y = -2;
            for(int& color : _current.colors)
            {
                color = int(_random.get_int(3)) + 1;
            }

            if(collides(_current.x, _current.y))
            {
                _game_over = true;
            }
        }

        void lock_piece()
        {
            for(int i = 0; i < 3; ++i)
            {
                const int py = _current.y + i;
                if(py < 0)
                {
                    _game_over = true;
                    return;
                }

                _board[py][_current.x] = _current.colors[i];
            }

            const int erased = erase_full_rows();
            if(erased > 0)
            {
                _lines += erased;
                _score += erased * erased * 100;
                const int next_speed = 30 - _lines / 3;
                _fall_speed = next_speed > 8 ? next_speed : 8;
            }

            spawn_piece();
        }

        int erase_full_rows()
        {
            int erased = 0;
            for(int y = board_height - 1; y >= 0; --y)
            {
                bool full = true;
                for(int x = 0; x < board_width; ++x)
                {
                    if(! _board[y][x])
                    {
                        full = false;
                        break;
                    }
                }

                if(full)
                {
                    ++erased;
                    for(int sy = y; sy > 0; --sy)
                    {
                        for(int x = 0; x < board_width; ++x)
                        {
                            _board[sy][x] = _board[sy - 1][x];
                        }
                    }
                    for(int x = 0; x < board_width; ++x)
                    {
                        _board[0][x] = 0;
                    }
                    ++y;
                }
            }

            return erased;
        }

        void reset()
        {
            for(auto& row : _board)
            {
                for(int& cell : row)
                {
                    cell = 0;
                }
            }

            _score = 0;
            _lines = 0;
            _fall_counter = 0;
            _fall_speed = 30;
            _game_over = false;
            spawn_piece();
        }

        [[nodiscard]] char cell_char(int value) const
        {
            switch(value)
            {
                case 1: return 'A';
                case 2: return 'B';
                case 3: return 'C';
                default: return '.';
            }
        }

        void draw()
        {
            _text_sprites.clear();

            _text_generator.generate(0, -74, "HEXTRIS ADVANCE (GBA)", _text_sprites);

            bn::string<32> hud("SCORE ");
            hud.append(bn::to_string<10>(_score));
            hud.append("  LINE ");
            hud.append(bn::to_string<5>(_lines));
            _text_generator.generate(0, -62, hud, _text_sprites);

            for(int y = 0; y < board_height; ++y)
            {
                bn::string<32> line;
                if(y & 1)
                {
                    line.append(' ');
                }

                for(int x = 0; x < board_width; ++x)
                {
                    int value = _board[y][x];

                    for(int i = 0; i < 3; ++i)
                    {
                        const int py = _current.y + i;
                        if(_current.x == x && py == y)
                        {
                            value = _current.colors[i];
                        }
                    }

                    line.append(cell_char(value));
                    line.append(' ');
                }

                _text_generator.generate(0, -44 + y * 8, line, _text_sprites);
            }

            _text_generator.generate(0, 72, "L/R: MOVE  A: ROTATE  DOWN: FAST", _text_sprites);

            if(_game_over)
            {
                _text_generator.generate(0, 56, "GAME OVER - PRESS START", _text_sprites);
            }
        }
    };
}

int main()
{
    bn::core::init();

    game g;

    while(true)
    {
        g.update();
        bn::core::update();
    }
}
