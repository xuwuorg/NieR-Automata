/*
    ���ߣ�����
    ���ͣ�http://www.xuwu.org
    mail��xuwuorg@163.com
    QQ ��474500991
*/

#pragma once 

#include <vector>
#include <list>
#include <set>
#include <map>
 
#include "xcommon.h" 

/*
    ������
        xstring.h ��װǿ����ַ��������ࡣ�����ܵ��ṩ�˸��ֳ������ַ��������Ľӿڷ�װ
        �����ֻ������ͺ��ַ�����ת�����ַ����������Ƚϣ��и���ķ�װ��������д�ظ�
        ��������ַ��������߼���

        ����ʹ��wchar����char���ͣ�Ĭ����Unicode�����ַ���Ϊ׼

    �÷���
        xuwuorg::string str(L"hello world");
        .....
        get_str()���𷵻�wcharָ�롣���ṩ��WindowsAPIʹ�á�
        length()�����ַ�������
*/

namespace xuwuorg
{
    /*
        string�������ַ������ͣ�ʵ�����ַ������ң��滻���Ƚϣ�ƴ�ӵȻ���������
        string_split���ַ����ָ�ϲ������࣬��stl��ϳ�һ���ַ������߰��ַ����и�stl�洢��
        string_file_path���ļ�·�������࣬�ṩ��һЩ�ļ�·������Ҫ�Ĺ��ܲ�����
    */
    class string;  
    class string_split;
    class string_file_path;
    
    //MAX_STRING_SIZE Ĭ�����洢���ٸ��ַ����飬�ҹ����������ʹ�ö�
#define MAX_STRING_SIZE 16
    
    /*
        ȫ�����������ǿ����ַ��������ࡣ�����������
        ʵ�����ַ������ң��滻���Ƚϣ�ƴ�ӵȣ�֧��������
    */
    class xexp string
    { 
    public:
        /*
            �ַ���֧�ֶ�wchar*��char*, bool, int, �Ȼ������͵Ĺ���ʹ��
        */
        string();
        string(const string& obj); 
        string(const wchar_t*);
        string(DWORD);
        virtual ~string();
           
        string& operator = (const string& obj);
        
        //�ַ���ƴ�ӣ�һ���Ľ����ͬ�Ĳ�����ʽ 
        string& operator << (const string& obj);  
         
        //��ȡ�ַ���ָ��
        inline const wchar_t* get_str() const
        { 
            return (m_max_size > MAX_STRING_SIZE)
                ? m_str.m_point_string 
                : m_str.m_string_buffer;
        }
         
        //�����ַ����Ƿ���ȣ�����Сд����
        bool is_equal(const string&) const; 
           
          
    private:
        TYPEDEF_SINGLE_VALUE; 
        //�ַ�������
        size_t m_string_length;
        //���buffer
        size_t m_max_size;

        union
        {
            wchar_t* m_point_string;
            wchar_t m_string_buffer[MAX_STRING_SIZE];
        }m_str;
    };
     

    /*
        ר�Ŷ��ļ�·�������Ĺ�����
    */
    class xexp string_file_path
    {
    public: 
        static string get_short_name(const string&); 
    };
};

