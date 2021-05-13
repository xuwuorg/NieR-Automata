/*
    作者：阿虚
    博客：http://www.xuwu.org
    mail：xuwuorg@163.com
    QQ ：474500991
*/

#pragma once 

#include <vector>
#include <list>
#include <set>
#include <map>
 
#include "xcommon.h" 

/*
    概述：
        xstring.h 封装强大的字符串工具类。尽可能的提供了各种场景下字符串操作的接口封装
        ，各种基础类型和字符串的转换，字符串搜索，比较，切割函数的封装。避免多次写重复
        复冗余的字符串处理逻辑。

        内置使用wchar而非char类型，默认以Unicode类型字符串为准

    用法：
        xuwuorg::string str(L"hello world");
        .....
        get_str()负责返回wchar指针。以提供给WindowsAPI使用。
        length()返回字符串长度
*/

namespace xuwuorg
{
    /*
        string：基础字符串类型，实现了字符串查找，替换，比较，拼接等基本操作。
        string_split：字符串分割合并工具类，把stl组合成一个字符串或者把字符串切割stl存储。
        string_file_path：文件路径工具类，提供了一些文件路径所需要的功能操作。
    */
    class string;  
    class string_split;
    class string_file_path;
    
    //MAX_STRING_SIZE 默认最大存储多少个字符数组，找过这个定义则使用堆
#define MAX_STRING_SIZE 16
    
    /*
        全世界最好用且强大的字符串基础类。满足各种需求。
        实现了字符串查找，替换，比较，拼接等，支持流操作
    */
    class xexp string
    { 
    public:
        /*
            字符串支持对wchar*，char*, bool, int, 等基础类型的构造使用
        */
        string();
        string(const string& obj); 
        string(const wchar_t*);
        string(DWORD);
        virtual ~string();
           
        string& operator = (const string& obj);
        
        //字符串拼接，一样的结果不同的操作方式 
        string& operator << (const string& obj);  
         
        //获取字符串指针
        inline const wchar_t* get_str() const
        { 
            return (m_max_size > MAX_STRING_SIZE)
                ? m_str.m_point_string 
                : m_str.m_string_buffer;
        }
         
        //两个字符串是否相等，不大小写敏感
        bool is_equal(const string&) const; 
           
          
    private:
        TYPEDEF_SINGLE_VALUE; 
        //字符串长度
        size_t m_string_length;
        //最大buffer
        size_t m_max_size;

        union
        {
            wchar_t* m_point_string;
            wchar_t m_string_buffer[MAX_STRING_SIZE];
        }m_str;
    };
     

    /*
        专门对文件路径操作的工具类
    */
    class xexp string_file_path
    {
    public: 
        static string get_short_name(const string&); 
    };
};

