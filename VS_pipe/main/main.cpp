// main.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>


#include "../lib_pipe/pipe_interface.h"
using namespace std;
using namespace lib_pipe;


// �����ӽ���
void new_pro(const int index)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	std::cout << "main_thread, ���ڴ����ӽ��̣�id=" << index << std::endl;

	std::string exe_path_current = utils::get_cwd();
	std::wstring wexe_path = utils::str2wstr_win(exe_path_current);
	std::wstring wexe_name(L"demo_create.exe");

	std::wstring param_wstr;
	std::wstring param_wstr_2;

	switch (index)
	{
	case 0:
		{
		param_wstr = wexe_path + L"\\1\\" + wexe_name;
		param_wstr_2 = L"A";
		}
		break;

	case 1:
	{
		param_wstr = wexe_path + L"\\2\\" + wexe_name;
		param_wstr_2 = L"B";
	}
	break;

	case 2:
	{
		param_wstr = wexe_path + L"\\3\\" + wexe_name;
		param_wstr_2 = L"C";
	}
	break;

	}


	LPCWSTR lp_p1	= param_wstr.c_str();
	LPWSTR lp_p2	= LPWSTR(param_wstr_2.c_str());


	//����һ���½���
	if (CreateProcess(lp_p1,
		lp_p2,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE, // CREATE_NEW_CONSOLE
		NULL,
		NULL,
		&si,
		&pi)

		)
	{
		// ���ӽ����������̷��룬��������д
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else
	{
		cout << "\n\n����ʧ��, error id = " << GetLastError() << "\n\n\n";
	}
}


//-------------------------------------------------------------------------------------------------------------------
// ��ΪҪ�������ݣ���Ҫ�̳� irecv_data ��ʵ�ֺ��� on_recv_data
class pipe_win : public irecv_data
{
public:
	pipe_win()
	{
		_ppipe = pipe_create();
	}

	~pipe_win()
	{
		pipe_release(_ppipe);
	}

	//-------------------------------------------------------------------------------------------------------------------

	//
	// @ brief: ��ʼ���ܵ�
	// @ const pipe_param_base - ��ʼ������
	// @ irecv_data *precv_data - ���պ�������
	//
	int init(const pipe_param_base& param)
	{
		if (NULL != _ppipe)
			return _ppipe->init(param, NULL);
		else
			return -1;
	}

	//
	// @ brief: ��ܵ���������
	// @ const char * pdata_send - ���͵���������
	// @ const unsigned int len_data_send - ���͵����ݳ���
	// @ return - int
	//
	int write(const char *pdata_send, const unsigned int len_data_send, unsigned int& len_written)
	{
		if (NULL != _ppipe)
			return _ppipe->write(pdata_send, len_data_send, len_written);
		else
			return -1;
		
	}

	//
	// @ brief: �ر�
	// @ return - int
	//
	int uninit()
	{
		if (NULL != _ppipe)
			return _ppipe->uninit();
		else
			return -1;
	}

	// ���յ�����������
	void on_recv_data(const char *pdata, const unsigned int len_recv_data)
	{
		cout << "\n\n�յ�������: " << len_recv_data << "\n";
	}

private:
	ipipe_interface * _ppipe = NULL;
};
//-------------------------------------------------------------------------------------------------------------------


// ��ں���
int main()
{
	// �ܵ��������´������ӽ��̵���������ȵģ� �ж��ٸ����̾���Ҫ�������ٸ��ܵ�
	const int pipe_count_3 = 2;

	// 1. ��Ҫ�����ܵ�������
	pipe_win pipe_arr[pipe_count_3];

	// ����׼��
	pipe_param_base base_param;
	base_param._to_create_pipe = true;
	base_param._name = std::string("\\\\.\\pipe\\ReadPipe");

	// �����ܵ�
	for (int i = 0; i < pipe_count_3; i++)
	{
		int ret_val = pipe_arr[i].init(base_param);

		if (0 != ret_val)
		{
			std::cout << "error id = " << ret_val << "\n\n";// << ", str = " << ret_val.str().c_str() << "\n\n";
		}
	}

	// 
	cout << "���̴߳����ܵ����� \n\n";

	// 2. �����������ӽ���
	for (int i = 0; i < pipe_count_3; i++)
		new_pro(i);
	
	cout << "���̴߳������̽����� ��������ȴ�3�룺���ӽ������� \n\n";
	Sleep(1000 * 3);

	// 3. ���Է������ݸ����߳�
	const char arr_send[] = "Q";	/// �ӽ���Լ���յ� Q �ͽ�������
	for (int i = 0; i < pipe_count_3; i++)
	{
		cout << "\n\n���ڷ���:";
		unsigned int len_has_written = 0;
		int ret_val = pipe_arr[i].write(arr_send, sizeof(arr_send), len_has_written);

		if (0 != ret_val)
			cout << "i = " << i << ", ����ʧ�ܣ�id = " << ret_val << "\n\n";
		else
			cout << "i = " << i << ", ���ͳɹ���id = " << len_has_written << "\n\n";
	}


	Sleep(2);

	// 4. �ͷ���Դ
	for (int i = 0; i < pipe_count_3; i++)
	{
		pipe_arr[i].uninit();
		Sleep(10);
	}

	cout << "���߳��ͷ��Լ������Ĺܵ���Դ���� \n\n";

	cout << "\n\n\n\n";
	system("pause");
	return 0;
}
