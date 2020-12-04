#pragma once
#include "pipe_interface.h"

#ifdef compiler_is_vs
#include <Windows.h>
#include <iostream>


// ���������ã�
#define is_debug

#ifdef is_debug
#include <iostream>
// ʹ��va_start��Ҫ��ͷ�ļ�
#include <stdarg.h>

#endif //!is_debug

namespace lib_pipe
{
	// ���ݶ���
//-------------------------------------------------------------------------------------------------------------------
	

	//
	// @brief: һЩö�ٲ���
	//
	enum 
	{
		// �ܵ���������С
		pipe_buf_size_4096 = 4096,
	};


	
	//
	// @brief: �̲߳���
	//
	struct pipe_param_thread_win_
	{
		// �߳̾��
		HANDLE		_handle;

		// �Ƿ�����
		bool		_is_running;

		HANDLE		_hevent;

		void zero()
		{
			_handle			= INVALID_HANDLE_VALUE;
			_is_running		= false;
			_hevent			= INVALID_HANDLE_VALUE;
		}

		pipe_param_thread_win_()
		{
			zero();
		}
	};

	//
	// @brief: �̲߳���
	//
	typedef pipe_param_thread_win_ pipe_param_thread_win;

	

	//
	// @brief: pipe����Ҫ�Ĳ���
	//
	struct  pipe_param_win_
	{
		// pipe����
		pipe_param_base			_base;

		// �̲߳���
		pipe_param_thread_win	_thread;

		// �߳��յ����ݽ����ݷ���˭
		irecv_data				*_precv_data;

		// �ܵ���ʶ��
		HANDLE					_handle;

		void zero()
		{
			_base.zero();
			_thread.zero();
			_precv_data			= NULL;
			_handle				= INVALID_HANDLE_VALUE;
		}

		pipe_param_win_()
		{
			zero();
		}
	};
	
	//
	// @brief: ������Ҫ�Ĳ�����װ���ṹ��
	//
	typedef pipe_param_win_	pipe_param_win;


	// �ඨ��
//-------------------------------------------------------------------------------------------------------------------

	


	//
	// @brief: �ܵ���
	//
	class pipe_helper : public ipipe_interface
	{
	public:
		//
		// @brief: ���캯��
		//
		pipe_helper();

		//
		// @brief: ��������
		//
		virtual ~pipe_helper();


		//  
		//  @ brief: ��ʼ���ܵ�
		//  @ const pipe_param_base - ��ʼ������
		//  @ irecv_data *precv_data - ���պ�������,������Ҫ���մ��룬�򴫵�nullptr ���� NULL
		//  @ return - int	
		//			����ֵ X:
		//			0 - ��ʼ���ɹ�
		//			X > 0 - ��ʼ��ʧ�ܣ�XΪ����GetLastError()�����Ľ����
		int init(const pipe_param_base& param, irecv_data *precv_data);


		// 
		// @ brief: ��ܵ���������
		// @ const char * pdata_send - ���͵���������
		// @ const unsigned int len_data_send - ���͵����ݳ���
		// @ const unsigned int& len_written - �Ѿ����͵����ݳ���
		// @ return - int
		//			����ֵ X��
		//			X = 0 - �ܵ�д�����ݳɹ�����len_written��len_data_send���
		//			X > 0 - �ܵ�д������ʧ�ܣ�XΪ����GetLastError�������ؽ������len_writtenֵΪ0
		int write(const char *pdata_send, const unsigned int len_data_send, unsigned int& len_written);

		// 
		// @ brief: �ر�
		// @ return - int
		//			0 - �رճɹ�
		//			-2 - �ر�ʧ�ܣ��ܵ�û�д򿪡�
		int uninit();

		// 
		// @ brief: ���س�ʼ���������̵߳���ʹ��
		// @ return - lib_pipe::pipe_param_win& 	
		//
		pipe_param_win& get_pipe_param() { return _pipe_param; }


		//
		// @brief:  ���չܵ������̺߳���
		//
		static DWORD WINAPI thread_recv_data(LPVOID lpParam);


		//
		// @brief: ��д���������̱߳�־
		//
		bool get_thread_recv_is_running();
		void set_thread_recv_is_running(const bool val);

		//
		// @brief: ���������Ϣ
		//
		void log(const char *pdata, ...);
	private:

		//
		// @brief: ���ο������캯��
		//
		pipe_helper(const pipe_helper& instance) {}

		//
		// @brief: ���������=
		//
		pipe_helper & operator = (const pipe_helper& instance) { return *this; }

		//
		// @brief:  �ܵ�����״̬
		// @return-bool 
		//			true-��
		//			false-�ر�
		//
		bool is_connected();


	private:

		//  
		//  @ brief: �����ܵ�
		//  @ return - int
		//			0 - �����ɹ�
		//			X - ����ʧ�ܣ�XΪ����GetLastError()�����ķ���ֵ
		int create_pipe_name();

		// 
		// @ brief: �ر�. �ͷ���Դ
		// @ return - void
		//	
		void pre_uninit();

		
		// 
		// @ brief: �򿪹ܵ�
		// @ return - int
		//			0 - �����ɹ�
		//			X - ����ʧ�ܣ�XΪ����GetLastError()�����ķ���ֵ
		int open();

	private:
		// �ܵ�����
		pipe_param_win	_pipe_param;
	};
}



#endif // !compiler_is_vs