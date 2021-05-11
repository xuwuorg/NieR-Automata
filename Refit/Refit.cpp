// infection.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <conio.h> 

#include <windows.h>

#include "CRefit.h" 

int main()
{      
    CRefit refit;    
    refit.help();

    do 
    { 
        if (_kbhit())
        { 
            char ch = _getch();
            switch (ch)
            {
            case '0':
                refit.open_nier_automata();
                break;
            case '1':
                refit.hp_max();
                break;
            case '2':
                refit.character_invincible();
                break;
            case '3':
                refit.hack_invincible();
                break;
            case '4':
                refit.spike_max();
                break;
            case '5':
                refit.speed_max();
                break;
            case '6':
                refit.jmp_flyin();
                break;
            case '7':
                refit.module_1();
                break;
            case '8':
                refit.max_props();
                break;
            default:
                break;
            } 
             
            refit.help();
        }

        Sleep(1000);
    } while (true);

    return 0;
} 