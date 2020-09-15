#pragma once
#include <string>
#include <utility>




// windows
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32)

//----------------------------------------------------------------------
	// ���� .dll  ��������
	#ifndef _lib_pipe_api_
		#define _lib_pipe_api_	__declspec(dllexport)
	#else
		#define _lib_pipe_api_	__declspec(dllimport)
	#endif /// !_lib_pipe_api_
//----------------------------------------------------------------------
#elif defined(_unix_) || defined(_linux_)
//----------------------------------------------------------------------
	// ���� .dll  ��������
	#ifndef _lib_pipe_api_
		#define _lib_pipe_api_	__attribute__((visibility ("default")))
	#endif /// !_lib_pipe_api_
//----------------------------------------------------------------------
#endif /// !



// _UNICODE����C���п⡣
#ifdef _UNICODE
	#ifndef UNICODE
		#define UNICODE
	#endif ///! UNICODE
#endif /// !_UNICODE

// UNICODE����WINAPI
#ifdef UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif///!_UNICODE
#endif ///!UNICODE

#if defined(__clang__) || defined(__GNUC__)

#elif defined(_MSC_VER)
	#ifndef compiler_is_vs
		#define compiler_is_vs 1
	#else 
		#define compiler_is_vs 0
	#endif /// !os_is_win
#endif /// 



#ifdef compiler_is_vs
#include <windows.h>
#endif //!



namespace lib_pipe
{
#ifdef __cplusplus              
	extern "C" {
#endif ///! __cplusplus



	/* �������Ͷ��� */
//-------------------------------------------------------------------------------------------------------------------


	/*
	* @brief: ��������ֵ�� ��ʽ��<������룬������Ϣ�ַ���>
				˵����
						1. �������
							1.1 ��������Ϊ�������Զ��巵�صĴ�����룬�Ҵ� -20��ʼ�ݼ��� -21�� -22�� -23��
							1.2 ��������Ϊ���á�GetLastError���ķ���ֵ���ҡ�������Ϣ�ַ�����Ϊ��"failure"��
						2. ������Ϣ�ַ���
							2.1 "success" - �ɹ��� �ҡ�������롿Ϊ0
							2.2 ��Ϊ "success"	- ʧ�ܣ���ʾ������ʾ�ַ������ҡ�������롿��Ϊ0
						3. ����
							�ɹ� <0, "success">
							ʧ�� <-20, "failure, XXXX">
							ʧ�� <GetLastError(), "GetLastError">
	*/
	typedef std::pair<int, std::string> pair_int_str;

	/*
	* @brief: ��������ֵ
	*/
	struct ret_type_
	{
		// ����
		void set(const int id, std::string str)
		{
			_value = std::make_pair(id, str);
		}

		// ���ش������
		int id() { return _value.first; }

		// ���ش�����Ϣ�ַ���
		std::string str() { return _value.second; }

		void zero()
		{
			_value = std::make_pair(0, "success");
		}

		ret_type_()
		{
			zero();
		}

	private:
		pair_int_str	_value;
	};

	// ��������ֵ
	typedef ret_type_ ret_type;


	/*
	* @brief: �ܵ���Ҫ�Ĳ���
	*/
	struct pipe_param_base_
	{
		// ����
		std::string _name;

		// �Ƿ񴴽�pipe, true-������false-������
		bool		_to_create_pipe;

		void zero()
		{
			_name = std::string("");
			_to_create_pipe = false;
		}

		pipe_param_base_()
		{
			zero();
		}
	};

	// pipe����
	typedef pipe_param_base_ pipe_param_base;


		/* �ඨ�� */
//-------------------------------------------------------------------------------------------------------------------

	/*
	* @brief: ��������,��Ҫ�̳и��ಢʵ�֡�on_recv_data���ӿ�
	*/
	class irecv_data
	{
	public:
		/*
		*  @ brief: ���յײ��յ�������
		*  @ const char * pdata - �յ�������
		*  @ const unsigned int len_recv_data - �յ������ݳ���
		*  @ return - void
		*/
		virtual void on_recv_data(const char *pdata, const unsigned int len_recv_data) = 0;
	};

	/*
	* @brief: �ܵ��ӿ�

	*  @ std::string str_pipe_name - �ܵ����� �� ���� "\\\\.\\pipe\\ReadPipe"
	*  @ bool to_create_pipe - true-������ false - ��������ֱ����
	*/
	class ipipe_interface
	{
	public:
		virtual ~ipipe_interface() {}

		/*
		*  @ brief: ��ʼ���ܵ�
		*  @ const pipe_param_base - ��ʼ������
		*  @ irecv_data *precv_data - ���պ�������
		*  @ return - lib_pipe::ret_type
		*/
		virtual ret_type init(const pipe_param_base& param, irecv_data *precv_data) = 0;

		/*
		*  @ brief: ��ܵ���������
		*  @ const char * pdata_send - ���͵���������
		*  @ const unsigned int len_data_send - ���͵����ݳ���
		*  @ return - lib_pipe::ret_type
		*/
		virtual ret_type write(const char *pdata_send, const unsigned int len_data_send) = 0;

		/*
		*  @ brief: �ر�
		*  @ return - lib_pipe::ret_type
		*/
		virtual ret_type uninit() = 0;

	};

#ifdef __cplusplus              
	}
#endif ///! __cplusplus


		/* ������������ */
//-------------------------------------------------------------------------------------------------------------------
		
		
	/* 
	*  @ brief: ����֧��Windows��pipe����, ʧ�ܷ��� nullptr����Ҫ�ֶ����ú�����release_pipe���ͷ�
	*  @ return - pipe_interface *
			nullptr - ����ʧ��
	*/
	extern "C" _lib_pipe_api_ ipipe_interface *pipe_create_win();


	/* 
	*  @ brief: �ͷš�ipipe_interface*������, �� pobj ����Ϊ nullptr
	*  @ ipipe_interface * pobj - ��create_pipe_win�����������Ķ���
	*  @ return - extern " _lib_pipe_api_ void
	*/
	extern "C" _lib_pipe_api_ void pipe_release(ipipe_interface* pobj);


//-------------------------------------------------------------------------------------------------------------------
	/*
	* @brief: ���ù�����
	*/
	class _lib_pipe_api_ utils
	{
	public:
		// ��std::string תΪ std::wstring
		static std::wstring str2wstr_win(const std::string &str);
		
		// ��ȡ��ǰ����Ĺ�⣬
		static std::string get_cwd();

#ifdef compiler_is_vs
		static void tchar2char(const TCHAR *tchar, char * _char);
#endif/// !complier_is_vs

	private:
		utils(){}
		virtual ~utils(){}
		utils &operator = (const utils& instance) { return *this; }
		utils(const utils& instance) {}
	};
}/// !lib_pipe

