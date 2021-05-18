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
                refit.character_invincible();
                break;
            case '2':
                refit.hp_9999();
                break;
            case '3':
                refit.hack_invincible(); 
                break;
            case '4':
                refit.attack_9999();
                break;
            case '5':
                refit.jmp_flyin();
                break;
            case '6':
                refit.module_1();
                break;
            case '7':
                refit.max_props();
                break; 
            default:
                break;
            } 
             
            refit.help();
        }

        Sleep(300);
    } while (true);

    return 0;
} 