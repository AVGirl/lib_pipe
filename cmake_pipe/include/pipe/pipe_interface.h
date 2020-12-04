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
#endif //! compiler_is_vs




namespace lib_pipe
{
#ifdef __cplusplus              
	extern "C" {
#endif ///! __cplusplus


	//
	// @brief: �ܵ�ͨ�ų�ʼ����Ҫ�Ĳ���
	//
	struct pipe_param_base_
	{
		// ����
		std::string _name;

		// �Ƿ񴴽�pipe, true-������false-������
		bool		_to_create_pipe;

		void zero()
		{
			_name			= std::string("");
			_to_create_pipe = false;
		}

		pipe_param_base_()
		{
			zero();
		}
	};

	// pipe����
	typedef pipe_param_base_ pipe_param_base;


	// �ඨ�� ��ʼ 
//-------------------------------------------------------------------------------------------------------------------

	//
	// @brief: ��������,��Ҫ�̳и��ಢʵ�֡�on_recv_data���ӿ�
	//
	class irecv_data
	{
	public:
		//  
		//  @ brief: ���յײ��յ�������
		//  @ const char * pdata - �յ�������
		//  @ const unsigned int len_recv_data - �յ������ݳ���
		//  @ return - void
		virtual void on_recv_data(const char *pdata, const unsigned int len_recv_data) = 0;
	};


//-------------------------------------------------------------------------------------------------------------------
	
	// 
	// @brief: �ܵ�ͨ�Žӿ��࣬������ʼ�����򿪷��ͣ��رսӿ�
	// 
	class ipipe_interface
	{
	public:
		virtual ~ipipe_interface() {}

		//  
		//  @ brief: ��ʼ���ܵ�
		//  @ const pipe_param_base - ��ʼ������
		//  @ irecv_data *precv_data - ���պ�������,������Ҫ���մ��룬�򴫵�nullptr ���� NULL
		//  @ return - int	
		//			����ֵ X:
		//			0 - ��ʼ���ɹ�
		//			X > 0 - ��ʼ��ʧ�ܣ�XΪ����GetLastError������������errno�Ľ��
		virtual int init(const pipe_param_base& param, irecv_data *precv_data) = 0;


		// 
		// @ brief: ��ܵ���������
		// @ const char * pdata_send - ���͵���������
		// @ const unsigned int len_data_send - ���͵����ݳ���
		// @ const unsigned int& len_written - �Ѿ����͵����ݳ���
		// @ return - int
		//			����ֵ X��
		//			X = 0 - �ܵ�д�����ݳɹ�����len_written��len_data_send���
		//			X > 0 - �ܵ�д������ʧ�ܣ�XΪ����GetLastError������������errno�Ľ������len_writtenֵΪ0
		virtual  int write(const char *pdata_send, const unsigned int len_data_send, unsigned int& len_written) = 0;

		// 
		// @ brief: �ر�
		// @ return - int
		//			0 - �رճɹ�
		//			-2 - �ر�ʧ�ܣ��ܵ�û�д򿪡�
		virtual int uninit() = 0;

	};

#ifdef __cplusplus              
	}
#endif ///! __cplusplus


	// ������������ 
//-------------------------------------------------------------------------------------------------------------------
		
		
	// 
	// @ brief: �����ܵ�ͨ�Ŷ���, ʧ�ܷ���NULL(Ϊ�˼��ݵͰ汾�����������򣬷���nullptr)����Ҫ�ֶ����ú�����release_pipe���ͷ�
	// @ return - pipe_interface *
	//			NULL - ����ʧ��
	//
	extern "C" _lib_pipe_api_ ipipe_interface *pipe_create();

	// 
	// @ brief: �ͷš�ipipe_interface*������, �� pobj ����Ϊ NULL(Ϊ�˼��ݵͰ汾����������������Ϊnullptr)
	// @ ipipe_interface * pobj - ��create_pipe_win�����������Ķ���
	// @ return - extern " _lib_pipe_api_ void
	//
	extern "C" _lib_pipe_api_ void pipe_release(ipipe_interface* pobj);


//-------------------------------------------------------------------------------------------------------------------
	// 
	// @brief: ���ù�����
	// 
	class _lib_pipe_api_ utils
	{
	public:
		// ��std::string תΪ std::wstring
		static std::wstring str2wstr_win(const std::string &str);
		
		// ��ȡ��ǰ����Ŀ¼�⣬
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

