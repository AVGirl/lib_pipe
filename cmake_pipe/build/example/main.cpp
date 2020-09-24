// demo_create.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include <iostream>
#include <windows.h>
using namespace std;
#include "pipe/pipe_interface.h"
using namespace lib_pipe;


// �����̺߳����߻���ʹ�ã������ٽ�����process_is_end��
CRITICAL_SECTION cs_proc_is_end;

// �ٽ���,���ڽ������̣�������Դû���ͷš� 
bool process_is_end = false;


// ���� main�������ĵ�һ�������������Ϳ����ڽ������ݵĺ�����ʹ�ø��������
std::string str_process_name;

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

// ��ΪҪ�������ݣ���Ҫ�̳� irecv_data ��ʵ�ֺ��� on_recv_data
class pipe_win : public irecv_data
{
public:
	pipe_win()
	{
		_ppipe = pipe_create_win();
	}

	~pipe_win()
	{
		pipe_release(_ppipe);
	}

	//-------------------------------------------------------------------------------------------------------------------

	/*
	*  @ brief: ��ʼ���ܵ�
	*  @ const pipe_param_base - ��ʼ������
	*  @ irecv_data *precv_data - ���պ�������
	*  @ return - lib_pipe::ret_type
	*/
	ret_type init(const pipe_param_base& param)
	{
		return _ppipe->init(param, this);
	}

	/*
	*  @ brief: ��ܵ���������
	*  @ const char * pdata_send - ���͵���������
	*  @ const unsigned int len_data_send - ���͵����ݳ���
	*  @ return - lib_pipe::ret_type
	*/
	ret_type write(const char *pdata_send, const unsigned int len_data_send)
	{
		return _ppipe->write(pdata_send, len_data_send);
	}

	/*
	*  @ brief: �ر�
	*  @ return - lib_pipe::ret_type
	*/
	ret_type uninit()
	{
		return _ppipe->uninit();
	}

	void on_recv_data(const char *pdata, const unsigned int len_recv_data)
	{
		cout << "\n\n���̣� " << str_process_name.c_str() << " �յ�������: " << len_recv_data << "\n";
		cout << "���̣� " << str_process_name.c_str() << "�յ��������ǣ�";
		for (unsigned int i = 0; i < len_recv_data; i++)
		{
			cout << pdata[i];
		}
		
		// Լ�� ��һ���ַ�ΪQ���˳�
		if ('Q' == pdata[0])
		{
			EnterCriticalSection(&cs_proc_is_end);
			process_is_end = true;
			LeaveCriticalSection(&cs_proc_is_end);
		}
	}

private:
	ipipe_interface * _ppipe = nullptr;
};


//-------------------------------------------------------------------------------------------------------------------


int main(int argc, char *argv[])
{
	// ��ʼ���������
	InitializeCriticalSection(&cs_proc_is_end);

	// �����һ���������ݶ�λ���̵Ĵ���
	str_process_name = std::string(argv[0]);

	std::cout << "hello world" << std::endl;
	std::cout << "1 = " << argv[0] << endl;

	cout << "�ӽ��� " << argv[0] << " �����ܵ���...\n";
	// ��ʼ�����ܵ�
	pipe_win pipe;// [pipe_count_3];

	// ����
	pipe_param_base base_param;
	base_param._to_create_pipe = false;
	base_param._name = std::string("\\\\.\\pipe\\ReadPipe");

	ret_type ret_val = pipe.init(base_param);
	if (0 != ret_val.id())
	{
		std::cout << "error id = " << ret_val.id() << ", str = " << ret_val.str().c_str() << "\n\n";
	}
	else
	{
		cout << "�ӽ��� " << argv[0] << " �����ܵ��ɹ�...\n";
	}


	// ����һֱwhile, 15�뵽����û�յ���Ϣ���˳�while
	int index = 0;
	while (true)
	{
		bool is_end = false;
		// �����ٽ���
		EnterCriticalSection(&cs_proc_is_end);
		is_end = process_is_end;
		// �뿪�ٽ�����
		LeaveCriticalSection(&cs_proc_is_end);

		if (is_end)
		{
			cout << "����while�� �����ͷ���Դ\n\n";
			break;
		}
		else
		{
			if (15 < index++)
			{
				cout << "�ӽ���" << argv[0] << ", 15s ���ˣ� ��û�գ��˳�while�������ͷ���Դ...\n\n"; 
				break;
			}

			// ˯��1�룬�ȴ����
			Sleep(1000 * 1);
			cout << "�ӽ���" << argv[0] << "˯��" << index + 1 << "��...\n\n";
		}
	}

	// �ͷſռ�
	pipe.uninit();

	cout << "��Դ�ͷ����\n\n";

	system("pause");
    return 0;
}

