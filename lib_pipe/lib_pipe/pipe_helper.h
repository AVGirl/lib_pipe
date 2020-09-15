#pragma once
#include <Windows.h>
#include <iostream>
#include "pipe_interface.h"

namespace lib_pipe
{
	// ���ݶ���
//-------------------------------------------------------------------------------------------------------------------
	

	/*
	* @brief: һЩö�ٲ���
	*/
	enum 
	{
		// �ܵ���������С
		pipe_buf_size_4096 = 4096,
	};

	
	/*
	* @brief: �̲߳���
	*/
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
	// �̲߳���
	typedef pipe_param_thread_win_ pipe_param_thread_win;

	

	/*
	* @brief:  pipe ��Ҫ�Ĳ���
	*/
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
	// ������Ҫ�Ĳ�����װ���ṹ��,
	typedef pipe_param_win_	pipe_param_win;


	// �ඨ��
//-------------------------------------------------------------------------------------------------------------------

	


	// �ܵ���
	class pipe_helper : public ipipe_interface
	{
	public:
		/*
		* @brief: ���캯��
		*/
		pipe_helper();

		/*
		* @brief: ��������
		*/
		virtual ~pipe_helper();


		/* 
		*  @ brief: ��ʼ���ܵ�
		*  @ const pipe_param_base - ��ʼ������
		*  @ irecv_data *precv_data - ���պ�������
		*  @ return - lib_pipe::ret_type
		*/
		ret_type init(const pipe_param_base& param, irecv_data *precv_data);

		/* 
		*  @ brief: ��ܵ���������
		*  @ const char * pdata_send - ���͵���������
		*  @ const unsigned int len_data_send - ���͵����ݳ���
		*  @ return - lib_pipe::ret_type
		*/
		ret_type write(const char *pdata_send, const unsigned int len_data_send);

		/* 
		*  @ brief: �ر�
		*  @ return - lib_pipe::ret_type
		*/
		ret_type uninit();

		/* 
		*  @ brief: ���س�ʼ���������̵߳���ʹ��
		*  @ return - lib_pipe::pipe_param_win& 
				
		*/
		pipe_param_win& get_pipe_param() { return _pipe_param; }

	private:

		/*
		* @brief: ���ο������캯��
		*/ 
		pipe_helper(const pipe_helper& instance) {}

		/*
		* @brief: ���������=
		*/
		pipe_helper & operator = (const pipe_helper& instance) { return *this; }

		/* 
		*  @ brief: �ܵ�������
		*  @ return - bool
				
		*/
		bool is_connected();

	private:
		/*
		* @brief: �����ܵ�
		*/
		ret_type create_pipe_name();

		/* 
		*  @ brief: �ر�. �ͷ���Դ
		*  @ return - void
				
		*/
		void pre_uninit();

		
		/* 
		*  @ brief: �򿪹ܵ�
		*  @ return - lib_pipe::ret_type
				
		*/
		ret_type open();

	private:
		// �ܵ�����
		pipe_param_win	_pipe_param;

	};
}

