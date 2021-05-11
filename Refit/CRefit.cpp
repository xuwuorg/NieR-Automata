#include <stdlib.h> 
#include <tchar.h>  

#include <iostream> 

#include <xstring.h>
#include <xprocess.h>
#include <xmodule.h>
#include <xprocess_memeory.h>
#include <xmemory.h>
using namespace xuwuorg;
  
#include "CRefit.h"

#define IS_START(status) if (status) return ;
#define START L" 【启动】"
#define CLOSE L" 【关闭】"
#define LINE

#define CURRENT_STATUS(status)  (status? START : CLOSE)
#define NEW_LINE L"\r\n"

CRefit::CRefit()
{
    m_start = false;
    m_invincible = false;
    m_hack_invincible = false;
    m_spike = false;
    m_speed = false;
    m_flyin = false;
    m_module = false;
    m_max_props = false;
    
    memory_management::mem_zero(m_old_character, 0x6);
}

CRefit::~CRefit()
{

}

void 
CRefit::help()
{ 
    _tsetlocale(LC_ALL, _T("")); 
    system("cls");

    xuwuorg::string help;
    help << L"尼尔机械纪元辅助 - 改装" << NEW_LINE << NEW_LINE
        << L"0. 启动辅助             " << CURRENT_STATUS(m_start) << NEW_LINE
        << L"    -- 游戏内摁键盘数字并没有什么卵用的" << NEW_LINE << NEW_LINE
        << L"1. 人物免伤无敌         " << CURRENT_STATUS(m_invincible) << NEW_LINE
        << L"2. 人物满血             " << CURRENT_STATUS(m_spike) << NEW_LINE
        << L"3. Hack无敌             " << CURRENT_STATUS(m_hack_invincible) << NEW_LINE
        << L"4. 屠龙宝刀 一刀999999  " << CURRENT_STATUS(m_spike) << NEW_LINE
        << L"    -- 在座的各位都是弟弟" << NEW_LINE << NEW_LINE
        << L"5. 召唤翔哥附体         " << CURRENT_STATUS(m_speed) << NEW_LINE
        << L"    -- 翔哥是XXX最快的男人" << NEW_LINE
        << L"6. 武当梯云纵           " << CURRENT_STATUS(m_flyin) << NEW_LINE
        << L"    -- 秘诀，只要跳起来左脚踩右脚就能飞天" << NEW_LINE
        << L"7. 芯片占1              " << CURRENT_STATUS(m_module) << NEW_LINE
        << L"    -- 宝刀未出一刀之下" << NEW_LINE
        << L"8. 店铺开张             " << CURRENT_STATUS(m_max_props) << NEW_LINE
        << L"    -- 只要我已经有的物品都能变成99" << NEW_LINE;
    wprintf(L"%s", help.get_str());
}

void 
CRefit::open_nier_automata()
{
    if (m_start)
    {
        close_game(); 
    }
    else
    {
        open_game();
    } 
}

void 
CRefit::hp_max()
{
    //hp = [[[0x000000014160DF98] + 0xC0 + 0x8] + 0x60] + 0x10668
    //m_proc_mem.read_offset()
}

bool 
CRefit::open_game()
{
    do
    {
        std::list<xprocess_tools::xprocess_info> pi_table;
        size_t size = xprocess_tools::enum_process_from_name(L"NieRAutomata.exe", pi_table);
        if (size != 0)
        {
            auto pi = pi_table.begin();
            size_t pid = pi->get_proc_id();
             
            if (!m_proc_mem.open_process(pid))
            {
                MessageBox(0, L"游戏打开失败，请用管理员权限启动辅助", 0, 0);
                break;
            }

            std::list<xmodule::xmodule_info> mi;
            size_t ret = xmodule::enum_process_module(pid, mi);
            for (auto& it : mi)
            {
                string module_name = string_file_path::get_short_name(it.get_full_name());
                if (module_name.is_equal(L"NieRAutomata.exe"))
                {
                    m_proc_mem.set_base_address((XADDRESS)it.get_base_address()); 
                    m_proc_mem.set_base_size(it.get_module_size()); 
                    m_start = true;
                    break;
                }
            }
        }
        else
        {
            MessageBox(0, L"还未启动Nier Automata", 0, 0);
            break;
        }

    } while (false);

    return m_start;
}

void 
CRefit::close_game()
{ 
    m_proc_mem.close(); 
    m_start = false;
}

void 
CRefit::character_invincible()
{
    if (!m_start)
    {
        return;
    }

    if (m_invincible)
    {
        close_character_invincible();
    }
    else
    {
        open_character_invincible();
    }
}

void 
CRefit::open_character_invincible()
{  
    /*
        00000001401F7B0F     | 0F85 95 220000           | jne nierautomata.1401F9DAA              | 3. 人物免伤无敌 这个改成jmp强跳

        00000001401F7B0F     | E9   96 220000             | jmp nierautomata.1401F9DAA              | 3. 人物免伤无敌 这个改成jmp强跳
        00000001401F7B14     | 90                      | nop                                     |
    */
     
    if (m_old_character[0] == 0x0)
    {
        m_invincible = m_proc_mem.read_offset(0x1F7B0F, m_old_character, 6, nullptr);
        if (!m_invincible)
        {
            return;
        }
    } 
    
    unsigned char szbuferr[6] = { 0xE9, 0x96, 0x22, 0x00, 0x00, 0x90 };
    szbuferr[1] = m_old_character[2] + 1;
    m_invincible = m_proc_mem.write_offset(0x1F7B0F, szbuferr, 6, nullptr);
    return;
}

void 
CRefit::close_character_invincible()
{
    if (m_old_character[0] == 0x0)
    {
        m_invincible = !m_proc_mem.write_offset(0x1F7B0F, m_old_character, 6, nullptr);
    }

    return;
}

void 
CRefit::hack_invincible()
{
    if (!m_start)
    {
        return;
    }

    if (m_hack_invincible)
    {
        close_hack_invincible();
    }
    else
    {
        open_hack_invincible();
    }
}

void 
CRefit::open_hack_invincible()
{
    //000000014020DB0A     | FF89 DC280100           | dec dword ptr ds:[rcx+128DC]            | 2. hk扣血地方分析入口1  真正中弹时扣血的地方，nop掉就是无敌
    unsigned char nop[6];
    memory_management::mem_set(nop, 0x90, 6);
    m_hack_invincible = m_proc_mem.write_offset(0x20DB0A, nop, 6, nullptr);
}

void 
CRefit::close_hack_invincible()
{ 
    unsigned char szbuferr[6] = { 0xFF, 0x89, 0xDC, 0x28, 0x01, 0x00 };
    m_hack_invincible = !m_proc_mem.write_offset(0x20DB0A, szbuferr, 6, nullptr);
}

void 
CRefit::spike_max()
{
    IS_START(m_spike);
}

void 
CRefit::speed_max()
{
    IS_START(m_speed);
}

void 
CRefit::jmp_flyin()
{
    if (!m_start)
    {
        return;
    }

    if (m_flyin)
    {
        close_jmp_flyin();
    }
    else
    {
        open_jmp_flyin();
    }
}

void 
CRefit::open_jmp_flyin()
{
    unsigned char szbuferr[6] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0x90 };
    m_hack_invincible = m_proc_mem.write_offset(0x1BBA7E, szbuferr, 6, nullptr);
    return;
}

void 
CRefit::close_jmp_flyin()
{
    //00000001401BBA7E    | 8B83 A8140000           | mov eax,dword ptr ds:[rbx+14A8]         | 6. 无限跳，武当梯云纵 修改机器码为 mov eax, 0
    unsigned char szbuferr[6] = { 0x8B, 0x83, 0xA8, 0x14, 0x00, 0x00 };
    m_flyin = !m_proc_mem.write_offset(0x1BBA7E, szbuferr, 6, nullptr);
    return;
}

void 
CRefit::module_1()
{
    IS_START(m_module);
}

void 
CRefit::max_props()
{
    IS_START(m_max_props);
}
