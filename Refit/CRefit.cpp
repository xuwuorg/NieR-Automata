#include <stdlib.h> 
#include <tchar.h>  

#include <iostream> 

#include <xstring.h>
#include <xprocess.h>
#include <xmodule.h>
#include <xprocess_memeory.h>
#include <xmemory.h> 
  
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
    m_attack = false;
    m_speed = false;
    m_flyin = false;  
    
    xuwuorg::memory_management::mem_zero(m_old_character, 0x6);
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
        << L"0. 启动辅助      " << CURRENT_STATUS(m_start) << L" -- 外挂为非注入插件，所有操作都在当前控制台启动关闭" << NEW_LINE << NEW_LINE
        << L"1. 人物免伤无敌  " << CURRENT_STATUS(m_invincible) << L" -- 不会被怪物攻击到" << NEW_LINE
        << L"2. HP 9999       " << L"          -- 都有无敌了HP随便意思下" << NEW_LINE
        << L"3. Hack无敌      " << CURRENT_STATUS(m_hack_invincible) << L" -- 9S人物使用hack时不会掉血" << NEW_LINE
        << L"4. 攻击 999999   " << CURRENT_STATUS(m_attack) << L" -- 人物初始攻击给 999999" << NEW_LINE
        << L"5. 召唤翔哥附体  " << CURRENT_STATUS(m_speed) << NEW_LINE
        << L"6. 跳跃无限制    " << CURRENT_STATUS(m_flyin) << L" -- 不停的按跳跃你能上天" << NEW_LINE
        << L"7. 芯片占1       " << L"          -- 当前背包的所有芯片大小设置为1" << NEW_LINE
        << L"8. 背包物品99    " << L"          -- 当前背包中已有的物品数量设置为99" << NEW_LINE;
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
 
bool 
CRefit::open_game()
{
    do
    {
        std::list<xuwuorg::xprocess_tools::xprocess_info> pi_table;
        size_t size = xuwuorg::xprocess_tools::enum_process_from_name(L"NieRAutomata.exe", pi_table);
        if (size != 0)
        {
            auto pi = pi_table.begin();
            size_t pid = pi->get_proc_id();
             
            if (!m_proc_mem.open_process(pid))
            {
                MessageBox(0, L"游戏打开失败，请用管理员权限启动辅助", 0, 0);
                break;
            }

            std::list<xuwuorg::xmodule::xmodule_info> mi;
            size_t ret = xuwuorg::xmodule::enum_process_module(pid, mi);
            for (auto& it : mi)
            {
                xuwuorg::string module_name = xuwuorg::string_file_path::get_short_name(it.get_full_name());
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
CRefit::hp_9999()
{
    if (!m_start)
    {
        return;
    }

    //00000001405DC5F0 | E8 7B19AEFF             | call nierautomata.1400BDF70                  | 0. 最开始搜索人物HP时的地方 [[[0x000000014160DF98] + 0xC0 + 0x8] + 0x60]
    //00000001400BDF8E | 48:C1E3 04 | shl rbx, 4 | 1. hp2 = [[[0x000000014160DF98]+ 0xC0 + 0x8] + 0x60] + 0x10668
    //00000001400BDF92 | 48 : 031D FFFF5401 | add rbx, qword ptr ds : [14160DF98] | 1. hp人物基址[0x000000014160DF98] + 0xC0
    
    static LONGLONG hp_address = 0;
    if (hp_address == 0)
    {
        bool bret = m_proc_mem.read_offset(0x160DF98, (unsigned char*)&hp_address, 0x8);
        if (bret)
        {
            bret = m_proc_mem.read(hp_address + 0xC0 + 0x8, (unsigned char*)&hp_address, 0x8);
            if (bret)
            {
                bret = m_proc_mem.read(hp_address + 0x60, (unsigned char*)&hp_address, 0x8);
                if (bret)
                {
                    hp_address += 0x10668;
                }
            }
        }

        if (!bret)
        {
            hp_address = 0;
        }
    }

    if (hp_address != 0)
    {
        ULONGLONG hp = 9999;
        m_proc_mem.write(hp_address, (unsigned char*)&hp, 0x8, nullptr);
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
    xuwuorg::memory_management::mem_set(nop, 0x90, 6);
    m_hack_invincible = m_proc_mem.write_offset(0x20DB0A, nop, 6, nullptr);
}

void 
CRefit::close_hack_invincible()
{ 
    unsigned char szbuferr[6] = { 0xFF, 0x89, 0xDC, 0x28, 0x01, 0x00 };
    m_hack_invincible = !m_proc_mem.write_offset(0x20DB0A, szbuferr, 6, nullptr);
}

void
CRefit::attack_9999()
{
    if (!m_start)
    {
        return;
    }

    //0000000140684CF | E8 7792A3FF             | call nierautomata.1400BDF70                      | 7. 攻击 [[[0x000000014160DF98] + 0xC0 + 0x8] + 0x60] + 0xAB8 + 0x20A0
  
    ULONGLONG attack_address = 0;
    bool bret = m_proc_mem.read_offset(0x160DF98, (unsigned char*)&attack_address, 0x8);
    if (bret)
    {
        bret = m_proc_mem.read(attack_address + 0xC0 + 0x8, (unsigned char*)&attack_address, 0x8);
        if (bret)
        {
            bret = m_proc_mem.read(attack_address + 0x60, (unsigned char*)&attack_address, 0x8);
            if (bret)
            {
                attack_address += (0xAB8 + 0x20A0);
            }
        }
    }

    if (!bret)
    {
        return;
    }

    DWORD attack = 999999;
    m_proc_mem.write(attack_address, (unsigned char*)&attack, 0x4, nullptr);
    return;
}
   
void 
CRefit::speed_max()
{
    if (!m_start)
    {
        return;
    }
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
    m_flyin = m_proc_mem.write_offset(0x1BBA7E, szbuferr, 6, nullptr);
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
    if (!m_start)
    {
        return;
    }

#define READ_ERR_RET(status) if (!status) return;

    //00000001405EA6A3 | 4C:8D05 0AF5B000        | lea r8,qword ptr ds:[1410F9BB4]              |
    //0000000140000000
     
    int countaa = 0;
    ULONGLONG buffer1 = 0;
    bool bret = m_proc_mem.read(m_proc_mem.get_base_address() + 0x10F9B90, (unsigned char*)&buffer1, 8);
    for (size_t index = 0, count = 0; count < 0x12C; index += 0x30, count++)
    {
        DWORD tmp = 0;
        bret = m_proc_mem.read(buffer1 + index + 0x1F54, (unsigned char*)&tmp, 4);
        READ_ERR_RET(bret);
        if (tmp == -1)
        {
            continue;
        }

        bret = m_proc_mem.read(buffer1 + index + 0x1F7C, (unsigned char*)&tmp, 4);
        READ_ERR_RET(bret);
        if (tmp != 0)
        {
            continue;
        }

        DWORD module_size = 1;
        bret = m_proc_mem.write(buffer1 + index + 0x1F60, (unsigned char*)&module_size, 4);
        READ_ERR_RET(bret);
    }  

    return;
}

void 
CRefit::max_props()
{
    if (!m_start)
    {
        return;
    }
    //00000001407EA7A3 | 48:8D15 B6171601        | lea rdx,qword ptr ds:[14194BF60]             | 5. 这里应该是物品道具背包已存在物品数量的偏移算法出
    //00000001405DCFF6 | 44:0140 08              | add dword ptr ds:[rax+8],r8d                 | 5. 这里是对物品扣除或增加数量的地方

    ULONGLONG props_base = m_proc_mem.get_base_address() + 0x194BF60;
     
    for (size_t i = 0; i < 0xFF; i++)
    {
        size_t pos = (i + i * 2) + 0xC159;
        size_t props = props_base + (pos * 4) + 8;

        DWORD max = 99;
        bool bret = m_proc_mem.write(props, (unsigned char*)&max, 4);
        if (!bret)
        {
            return;
        }
    }

    return;
}
