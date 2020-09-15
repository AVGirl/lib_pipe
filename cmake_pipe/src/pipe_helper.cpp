
#include "pipe/pipe_helper.h"

namespace lib_pipe
{
	/*
	* @brief: �̺߳���
	*/
	DWORD WINAPI thread_recv_data(LPVOID lpParam)
	{
		// ׼������
		pipe_helper* pipe_obj	= NULL;
		pipe_obj				= reinterpret_cast<pipe_helper*>(lpParam);
		
		// 1. û�ж���
		if (NULL				== pipe_obj)
			return 0;

		

		// 2. ��ʼ�������ݣ���׼������
		pipe_param_win& pipe_param = pipe_obj->get_pipe_param();

		// ���ջ���
		char arr_recv_data[pipe_buf_size_4096 + 1] = { 0 };

		// �ɹ���ȡ������
		DWORD len_real_read = 0;


		OVERLAPPED over_lapped = { 0, 0, 0, 0, pipe_param._thread._hevent};

		//if (INVALID_HANDLE_VALUE != pipe_param._thread._hevent)
		//	return 0;


		// �߳�����
		while (pipe_param._thread._is_running)	/// ͨ����־λ �����߳�����
		{
			// �ȴ�����ܵ�, ���� ��Ϣд��overlap��
			// ConnectNamedPipe(pipe_param._handle, &over_lapped);
			//�ȴ�
			// WaitForSingleObject(pipe_param._thread._hevent, INFINITE);

			//if (!GetOverlappedResult(pipe_param._handle, &over_lapped, &len_real_read, TRUE))
			//{
			//	break;
			//}

			//std::cout << "\n\n�������while\n\n";

			// ��ȡ�ܵ����� �� ����
			int ret_val			= (int)ReadFile(	pipe_param._handle,	// �ܵ���ʶ��
													arr_recv_data,		// ���ջ���
													pipe_buf_size_4096, // ���ջ�������С
													&len_real_read,		// �������볤��
													NULL				// Ĭ�ϴ���NULL
												);
			// 2.1 ��ȡ�ɹ�
			if (1 == ret_val)
			{
				// 2.1.1 ����������������ȣ�����, ׼��������һ������
				if (pipe_buf_size_4096 < len_real_read || 0 == len_real_read)
				{
				}
				else
				{
					// 2.1.2 ���Ⱥ��ʣ����Ͻ�����
					arr_recv_data[len_real_read] = '\0';
					
					// ���ý��պ���
					pipe_param._precv_data->on_recv_data(arr_recv_data, len_real_read);
				}
			}
			// 2.2 ��ȡʧ��
			else 
			{
				// ������ȡ��һ��
			}

			// ����CPU��һֱæ�ڴ���ǰ�߳�
			Sleep(2);

			memset(arr_recv_data, 0, pipe_buf_size_4096 + 1);
			// DisconnectNamedPipe(pipe_param._handle);
		}

		return 0;
	}


	/*
	*	@brief: ��ʼ�������ʹ����ܵ�
	*/
	lib_pipe::ret_type pipe_helper::init(const pipe_param_base& param, irecv_data *precv_data)
	{
		// ��������
		pipe_param_base& param_base = _pipe_param._base;
		int error_id_20				= -20;

		// ���巵��ֵ
		ret_type ret_val;

		// 1. pipe����Ϊ��
		if (0 == param._name.length())
		{
			ret_val.set(error_id_20, std::string("failure, pipe's name is null"));//  = std::make_pair());
			_pipe_param.zero();

			return ret_val;
		}

		// 2.�������
		//_pipe_param._base._is_server		= is_server;
		//_pipe_param._base._name			= str_pipe_name;
		param_base = param;
		// 
		if (NULL != precv_data || nullptr != precv_data)
			_pipe_param._precv_data = precv_data;

		// 3. ����Ƿ����п��õĹܵ�
		if (true == param_base._to_create_pipe)
		// if (FALSE == WaitNamedPipe(LPTSTR(param_base._name.c_str()), NMPWAIT_WAIT_FOREVER) )
		{
			// �������ܵ��������򴴽�������Ϊ����ˣ�
			ret_val = create_pipe_name();
		}
		else
		{
			// �������ܵ����������ӣ�����Ϊ�ͻ��ˣ�
			ret_val = open();
		}

		// ��鴴�����ߴ򿪹ܵ�����ֵ, ����������ߴ򿪹ܵ�ʧ�ܣ��򷵻�ʧ��
		if (0 != ret_val.id())
		{
			_pipe_param.zero();
			return ret_val;
		}


		// 4. ���������߳�
		pipe_param_thread_win& thread_param = _pipe_param._thread;

		// ��Ҫ�������ݣ��ٴ����߳�
		if (NULL != _pipe_param._precv_data)
		{
			// 4.1 ����߳����б��Ϊtrue
			thread_param._is_running = true;

			// �����¼�
			//thread_param._hevent = CreateEvent(NULL, FALSE, FALSE, FALSE);

			// 4.2 �����߳�
			thread_param._handle = CreateThread(NULL,	// Ĭ�� NULL
												0,		// ջ��С��Ĭ��
												thread_recv_data, // �̺߳�����������ʽΪ��DWORD WINAPI ThreadProc (PVOID pParam) ;
												this,	// ���ݵ��̺߳����еĲ���
												0,		// ͨ��Ϊ0�������������̲߳�����ִ��ʱΪCREATE_SUSPENDED���߳̽���ֱͣ������ResumeThread���ָ��̵߳�ִ��Ϊֹ��
												NULL	// ͨ���� 0
												);

			// ���������߳�ʧ�ܣ����ش���
			if (INVALID_HANDLE_VALUE == thread_param._handle)
			{
				ret_val.set(error_id_20 - 1, std::string("failure, recv data thread created failure"));// = std::make_pair(error_id_20 - 1, std::string("failure, recv data thread created failure"));

				return ret_val;
			}
			else
			{
				// �������ս��̳ɹ�
			}
		}
		else
		{
			// ����Ҫ��������
		}

		return ret_val;
	}


	/*
	*	@brief: ��ܵ�д������
	*/
	lib_pipe::ret_type pipe_helper::write(const char *pdata_send, const unsigned int len_data_send)
	{
		// ��������ֵ
		ret_type ret_val;

		// 0. ����Ϊ��
		if (NULL == pdata_send || nullptr == pdata_send)
		{
			ret_val.set(-21, std::string("failure, pdata_send is null"));
			return ret_val;
		}

		// 1. û�����ӣ��򷵻�ʧ��
		if (!is_connected())
		{
			ret_val.set(-20, std::string("failure, pipe doesnt created"));
			return ret_val;
		}

		// 2. ��ܵ�д������

		// 2.1 ����д���������
		DWORD len_has_written = 0;
		int ret = (int)WriteFile(	_pipe_param._handle,	// �ܵ���ʶ
									pdata_send,				// �����͵�����
									len_data_send,			// �������ݳ���
									&len_has_written,		// �������ݳɹ�д�� 
									NULL );					// �ص��ṹ�� Ĭ�ϴ���NULL��
		// д��ʧ��
		if ( 0 == ret || 0 == len_has_written)
		{
			ret_val.set(GetLastError(), std::string("GetLastError"));
		}
		
		return ret_val;
	}

	/*
	*	@brief: �رչܵ�
	*/
	lib_pipe::ret_type pipe_helper::uninit()
	{
		ret_type ret_val;

		// 1.��û�д򿪣��򷵻� 
		if (!is_connected())
		{
			ret_val.set(-20, std::string("failure, pipe doesnt open"));
			return ret_val;
		}

		// 2. �ͷ���Դ
		pre_uninit();

		return ret_val;
	}

	/*
	*	@brief: ���캯������ʼ������
	*/
	pipe_helper::pipe_helper()
	{
		_pipe_param.zero();
	}

	/*
	*	@brief: ���ܵ��Ƿ��Ѿ�����
	*/
	bool pipe_helper::is_connected()
	{
		return (INVALID_HANDLE_VALUE == _pipe_param._handle) ? false : true;
	}

	/*
	*	@brief:�����ܵ�
	*/
	lib_pipe::ret_type pipe_helper::create_pipe_name()
	{
		// ��������
		pipe_param_base& param_base = _pipe_param._base;
		pipe_param_win& param		= _pipe_param;

		// ָ������ֵ
		ret_type ret_val;///  = std::make_pair(0, std::string("success"));

		// 1. �Ѿ����������ȹر�
		if (INVALID_HANDLE_VALUE != param._handle)
		{
			DisconnectNamedPipe(_pipe_param._handle);
			CloseHandle(param._handle);
			param._handle = INVALID_HANDLE_VALUE;
		}

		// Ϊ��ͳһʹ��Windows��Unicode��api
		TCHAR  * tc_exe_path = NULL;
#ifdef UNICODE
		std::wstring wstr = utils::str2wstr_win(_pipe_param._base._name);
		tc_exe_path = const_cast<TCHAR*>(wstr.c_str());
#else
		tc_exe_path = const_cast<TCHAR*>(_pipe_param._base._name.c_str());
#endif

		// 2. ���������ܵ�
		param._handle = CreateNamedPipe(		tc_exe_path,//LPTSTR(param_base._name.c_str()),		// �ܵ���
												PIPE_ACCESS_DUPLEX |
												FILE_FLAG_OVERLAPPED,				// �ܵ�ͨ�ţ�ȫ˫��ͨ��
												PIPE_TYPE_MESSAGE |					// ��Ϣ��������ʽ
												PIPE_READMODE_MESSAGE |				// ��Ϣ�飺֡
												PIPE_WAIT,							// ͬ�������ڵȴ���ʱ������߳�
												PIPE_UNLIMITED_INSTANCES,			// ���������ʵ������
												pipe_buf_size_4096,					// �������������
												pipe_buf_size_4096,					// ���뻺��������
												0,									// �ܵ���Ĭ�ϵȴ���ʱ�� 0��Ĭ��
												NULL);								// ʹ�ò�����̳е�һ��Ĭ��������
		
		// 2.1 ����ʧ��
		if ( INVALID_HANDLE_VALUE == param._handle)
		{
			ret_val.set(GetLastError(), std::string("GetLastError"));// = std::make_pair(GetLastError(), std::string("failure"));
			return ret_val;
		}

		// 3. �����ɹ�
		return ret_val;
	}

	/*
	*	@brief: �رգ��ͷ���Դ
	*/
	void pipe_helper::pre_uninit()
	{
		// 2. ���������߳�
		if (NULL != _pipe_param._precv_data)
		{
			_pipe_param._thread._is_running = false;

			// �ȴ��̷߳��ؽ�� 
			WaitForSingleObject(_pipe_param._thread._handle, 1000 * 3);	/// ��һ�������� �̵߳��ں˾�����ڶ����������ȴ�ʱ�䣺3��

			// �ر��ں��߳̾��
			CloseHandle(_pipe_param._thread._handle);

			// ���߳̾������Ϊ��Ч
			_pipe_param._thread._handle = INVALID_HANDLE_VALUE;
		}
		else
		{
			// û�д����߳�
		}

		// 3. �ر�
		if (INVALID_HANDLE_VALUE != _pipe_param._handle)
		{
			DisconnectNamedPipe(_pipe_param._handle);
			CloseHandle(_pipe_param._handle);
			_pipe_param._handle = INVALID_HANDLE_VALUE;
		}

		if (INVALID_HANDLE_VALUE != _pipe_param._thread._hevent)
		{
			CloseHandle(_pipe_param._thread._hevent);
			_pipe_param._thread._hevent = INVALID_HANDLE_VALUE;
		}

		
		

		// 4. ���ñ�־
		_pipe_param.zero();
	}

	/*
	*	@brief:
	*/
	lib_pipe::ret_type pipe_helper::open()
	{
		ret_type ret_val;

		// 1. ����Ѿ����ӣ���ر�
		if (INVALID_HANDLE_VALUE != _pipe_param._handle)
		{
			DisconnectNamedPipe(_pipe_param._handle);
			CloseHandle(_pipe_param._handle);
			_pipe_param._handle = INVALID_HANDLE_VALUE;
		}

		TCHAR  * tc_exe_path = NULL;
#ifdef UNICODE
		std::wstring wstr = utils::str2wstr_win(_pipe_param._base._name);
		tc_exe_path = const_cast<TCHAR*>(wstr.c_str());
#else
		tc_exe_path = const_cast<TCHAR*>(_pipe_param._base._name.c_str());
#endif
		// 2. �򿪿��õ������ܵ� , ����������˽��̽���ͨ��  
		_pipe_param._handle = CreateFile(	tc_exe_path,
											GENERIC_READ | GENERIC_WRITE,
											FILE_SHARE_READ | FILE_SHARE_WRITE, // ��������
											NULL, 
											OPEN_EXISTING, 
											0, 
											NULL);

		// 2.1 ����ʧ��
		if (INVALID_HANDLE_VALUE == _pipe_param._handle)
		{
			ret_val.set(GetLastError(), std::string("GetLastError"));
		}

		return ret_val;
	}

	/*
	*	@brief:��������
	*/
	pipe_helper::~pipe_helper()
	{
		// 1.���򿪣���ر�
		if (is_connected())
		{
			pre_uninit();
		}
		else
		{
			// 2. û�д򿪹ܵ�	
		}
	}

}/// !lib_pipe

