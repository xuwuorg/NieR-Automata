#pragma once
#include <xprocess_memeory.h>

class CRefit
{
public:
    CRefit();
    ~CRefit();

    void help();
    void open_nier_automata(); 
    void character_invincible();
    void hp_9999();
    void hack_invincible();
    void attack_9999();
    void speed_max();
    void jmp_flyin();
    void module_1();
    void max_props();

private:
    bool open_game();
    void close_game();

    void open_character_invincible();
    void close_character_invincible();
     
    void open_hack_invincible();
    void close_hack_invincible();


    void open_jmp_flyin();
    void close_jmp_flyin();

private:
    bool m_start;
    bool m_invincible;
    bool m_hack_invincible;
    bool m_attack;
    bool m_speed;
    bool m_flyin;  
      
    xuwuorg::xprocess_memeory m_proc_mem;
    unsigned char m_old_character[6];
}; 